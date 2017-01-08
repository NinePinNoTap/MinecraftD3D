#include "VoxelWorld.h"
#include "WindowManager.h"

VoxelWorld::VoxelWorld()
{
	m_chunkGenerator = 0;
	m_threadhandler = 0;
	m_player = 0;
}

VoxelWorld::~VoxelWorld()
{
}

void VoxelWorld::initialise()
{
	ManagedThread<void()>* initialChunkloading;
	SYSTEM_INFO systemInfo;

	//===================
	// initialise Player
	//===================

	m_player = new Player;
	m_player->initialise();
	m_player->setHeight(2.0f);

	//===========================
	// initialise Terrain Layers
	//===========================

	m_chunkGenerator = new ChunkGenerator;

	// LayerType						 (LayerName,         Block,      H,   Freq,   Amp, Exp,       Type)
	m_chunkGenerator->addLayer(TerrainLayer("Bedrock", "bedrock", 1, 0.1f, 2, 1, LayerType::Absolute));
	m_chunkGenerator->addLayer(TerrainLayer("Base Stone", "stone", 39, 64.0f, 9, 1, LayerType::Absolute));
	m_chunkGenerator->addLayer(TerrainLayer("Mountain 1", "stone", 1, 129.0f, 60, 1, LayerType::Absolute));
	m_chunkGenerator->addLayer(TerrainLayer("Mountain 2", "stone", 4, 111.0f, 58, 1, LayerType::Absolute));
	m_chunkGenerator->addLayer(TerrainLayer("Base Dirt 1", "dirt", 60, 16.0f, 4, 1, LayerType::Absolute));
	m_chunkGenerator->addLayer(TerrainLayer("Base Dirt 2", "dirt", 59, 25.0f, 6, 1, LayerType::Absolute));
	m_chunkGenerator->addLayer(TerrainLayer("Surface", "dirt", 1, 1.0f, 1, 1, LayerType::Additive));
	m_chunkGenerator->addLayer(TerrainLayer("cave", "air", 1, 10.0f, 10, 10, LayerType::Absolute));


	//===========================
	// initialise Chunk Building
	//===========================

	// Positions within a radius around us
	for (int y = World::loadRadius; y >= -World::loadRadius; y--)
	{
		for (int z = -World::loadRadius; z <= World::loadRadius; z++)
		{
			for (int x = -World::loadRadius; x <= World::loadRadius; x++)
			{
				m_localChunks.push_back(D3DXVECTOR3(x, y, z));
			}
		}
	}

	generateLocalChunks();

	// set starting stats
	m_averageloadTime = 0.0f;
	m_buildCount = 0;

	//======================
	// initialise Threading
	//======================

	// Retrieve system Information
	GetSystemInfo(&systemInfo);

	// Store how many threads we have, reserve one for thread handler
	m_maxThreads = systemInfo.dwNumberOfProcessors - 2;

	// run a thread to handle initial worker threads
	initialChunkloading = new ManagedThread<void()>;
	initialChunkloading->setFunction(std::bind(&VoxelWorld::handleThreads, this));
	initialChunkloading->begin(true);

	// Force a wait until initial terrain built
	while (true)
	{
		if (m_chunkQueue.empty())
			break;
	}

	initialChunkloading->forceQuit();
	initialChunkloading = 0;
}

bool VoxelWorld::update()
{
	// handle Player Input
	m_player->update();

	//=========================
	// handle Chunk Generation
	//=========================

	generateLocalChunks();

	//=======================
	// handle Chunk Updating
	//=======================

	handleChunks();

	//==================
	// handle Building
	//==================

	handleThreads();

	return true;
}

bool VoxelWorld::render()
{
	//======================
	// render Active Chunks
	//======================

	for (it_wc it = m_chunkMap.begin(); it != m_chunkMap.end(); it++)
	{
		if (it->second)
		{
			m_result = it->second->render();
			if (!m_result)
			{
				return false;
			}
		}
	}

	return true;
}

void VoxelWorld::handleThreads()
{
	handleActiveThreads();
	handleBuildList();
}

void VoxelWorld::handleActiveThreads()
{
	if (m_managedThreads.empty())
	{
		return;
	}

	//=========================
	// Remove isComplete Threads
	//=========================

	for (vector<ManagedThread<void()>*>::iterator it = m_managedThreads.begin(); it != m_managedThreads.end();)
	{
		ManagedThread<void()>* chunkThread = *it;

		// Remove finished threads
		if (chunkThread->tryJoin())
		{
			it = m_managedThreads.erase(it);
		}
		else
		{
			++it;
		}

		// Clean Up
		chunkThread = 0;
	}
}

void VoxelWorld::handleBuildList()
{
	D3DXVECTOR3 buildTarget;
	WorkerType workerType;
	ManagedThread<void()>* chunkThread;

	// Make sure we have chunks to deal with
	if (m_chunkQueue.empty())
	{
		return;
	}

	// Make sure we have available threads
	if (m_managedThreads.size() >= m_maxThreads)
	{
		return;
	}

	// get the first job
	buildTarget = m_chunkQueue.front().chunkTarget;
	workerType = m_chunkQueue.front().workerType;

	//============
	// handle Job
	//============

	if (workerType == WorkerType::build)
	{
		// create a build thread
		chunkThread = new ManagedThread<void()>;
		chunkThread->setFunction(std::bind(&VoxelWorld::buildChunk, this, buildTarget));
		chunkThread->begin();
		m_managedThreads.push_back(chunkThread);
	}
	else if (workerType == WorkerType::update)
	{
		// run a thread to update the chunk
	}

	// Remove chunk from build list
	m_chunkQueue.erase(m_chunkQueue.begin());

	return;
}

void VoxelWorld::buildChunk(D3DXVECTOR3 chunkIndex)
{
	string chunkKey;
	Chunk* createdChunk;

	// generate a key for the chunk
	chunkKey = getKey(chunkIndex.x, chunkIndex.y, chunkIndex.z);

	// Search for the chunk
	it_wc iterator = m_chunkMap.find(chunkKey);
	if (iterator == m_chunkMap.end())
	{
		float time = timeGetTime();

		// create the chunk
		createdChunk = new Chunk;
		createdChunk->initialise(chunkIndex.x, chunkIndex.y, chunkIndex.z);
		m_chunkGenerator->generateChunk(*createdChunk);
		createdChunk->refreshVisible();

		m_chunkMap[chunkKey] = createdChunk;
		createdChunk = 0;

		outputToDebug("created Chunk : " + chunkKey);
		outputTimeDelay("Time Taken : ", time, timeGetTime());

		m_averageloadTime += timeGetTime() - time;
		m_buildCount++;
	}
	else
	{
		outputToDebug("Found Existing Chunk : " + chunkKey);

		// Check if we have loaded it
		if (iterator->second)
		{
			// Chunk loaded
			return;
		}
		else
		{
			string chunkFilename = "chunks/" + chunkKey + ".bin";

			// onload chunk from bin
			FILE* pFile;
			fopen_s(&pFile, chunkFilename.c_str(), "rb");

			if (pFile != NULL)
			{
				//loadedChunk->initialise(chunkIndex.x, chunkIndex.y, chunkIndex.z);

				m_chunkMap[chunkKey] = new Chunk;
				m_chunkMap[chunkKey]->initialise(chunkIndex.x, chunkIndex.y, chunkIndex.z);

				// onload file into new chunk
				fread(m_chunkMap[chunkKey], sizeof(Chunk), 1, pFile);

				// Close File
				fclose(pFile);
				outputToDebug("File loaded!");
			}
			else
			{
				// No file loaded so generate again?
			}
		}
	}
}

void VoxelWorld::handleChunks()
{
	D3DXVECTOR3 chunkKey;

	//===============
	// update Chunks
	//===============

	for (it_wc it = m_chunkMap.begin(); it != m_chunkMap.end(); it++)
	{
		// Make sure we have a chunk to update
		if (it->second)
		{
			// update the chunk
			it->second->update();

			// Convert key from string to vector3
			chunkKey = getKey(it->first);

			if (it->second->isDirty())
			{
				// Add to list for refreshing
				m_chunkQueue.push_back(ChunkTarget(chunkKey, WorkerType::update));
			}
			else if (it->second->isOutOfRange())
			{
				// Add extension
				string chunkFilename = "chunks/" + it->first + ".bin";

				// run a thread to unload the chunk
				FILE* pFile = fopen(chunkFilename.c_str(), "wb");

				if (pFile != NULL)
				{
					// Write to file
					fwrite(it->second, sizeof(Chunk), 1, pFile);
					fclose(pFile);

					// terminate chunk
					it->second->terminate();
					it->second = 0;
				}
			}
		}
	}
}

void VoxelWorld::generateLocalChunks()
{
	D3DXVECTOR3 chunkIndex;

	//================
	// Check movement
	//================

	// Calculate the chunk column we are in
	chunkIndex.x = floor(Camera::getInstance()->getTransform()->getPosition().x / (float)World::ChunkSize);
	chunkIndex.y = floor(Camera::getInstance()->getTransform()->getPosition().y / (float)World::ChunkSize);
	chunkIndex.z = floor(Camera::getInstance()->getTransform()->getPosition().z / (float)World::ChunkSize);

	// Check if we have moved chunk since last check
	if (chunkIndex == m_lastChunkPosition)
	{
		return;
	}

	// Store the new chunk we are in
	m_lastChunkPosition = chunkIndex;

	//================
	// Chunk Building
	//================

	// Force to load centre first
	m_chunkQueue.push_back(ChunkTarget(chunkIndex + Vector::Zero, WorkerType::build));

	// onload chunks around us
	for (unsigned int i = 0; i < m_localChunks.size(); i++)
	{
		// Add to build list
		m_chunkQueue.push_back(ChunkTarget(chunkIndex + m_localChunks[i], WorkerType::build));
	}
}

void VoxelWorld::setBlock(int x, int y, int z, string blockName)
{
	Block* currentBlock;

	// get the block
	currentBlock = getBlock(x, y, z);

	// Make sure its a valid block
	if (!currentBlock)
		return;

	// update the block
	currentBlock->clone(World::Blocks[blockName]);
}

Block* VoxelWorld::getBlock(int x, int y, int z)
{
	float chunkX, chunkY, chunkZ, blockX, blockY, blockZ;
	Chunk* currentChunk;
	Block* currentBlock;

	//=======================
	// Calculate World Chunk
	//=======================

	chunkX = x / (float)World::ChunkSize;
	chunkZ = z / (float)World::ChunkSize;

	chunkY = y / (float)World::ChunkSize;
	chunkY = floor(chunkY);

	chunkX = floor(chunkX);
	chunkZ = floor(chunkZ);

	//===========
	// get Chunk
	//===========

	// Find the chunk
	it_wc it = m_chunkMap.find(getKey(chunkX, chunkY, chunkZ));
	if (it == m_chunkMap.end())
	{
		return 0;
	}

	// get the chunk
	currentChunk = it->second;
	if (!currentChunk)
	{
		return 0;
	}

	//===========
	// get Block
	//===========

	// Calculate block index
	blockX = x - currentChunk->getPosition().x;
	blockY = y - currentChunk->getPosition().y;
	blockZ = z - currentChunk->getPosition().z;

	// Try and get block
	currentBlock = currentChunk->getBlock(blockX, blockY, blockZ);
	if (!currentBlock)
	{
		return 0;
	}

	return currentBlock;
}