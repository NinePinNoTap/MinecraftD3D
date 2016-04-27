#include "VoxelWorld.h"
#include "WindowManager.h"

VoxelWorld::VoxelWorld()
{
	ChunkGenerator_ = 0;
	ThreadHandler_ = 0;
	Player_ = 0;
}

VoxelWorld::~VoxelWorld()
{
}

void VoxelWorld::Initialise()
{
	ManagedThread<void()>* initialChunkLoading;
	SYSTEM_INFO systemInfo;

	//===================
	// Initialise Player
	//===================

	Player_ = new Player;
	Player_->Initialise();
	Player_->SetHeight(2.0f);

	//===========================
	// Initialise Terrain Layers
	//===========================

	ChunkGenerator_ = new ChunkGenerator;

	// LayerType						 (LayerName,         Block,      H,   Freq,   Amp, Exp,       Type)
	ChunkGenerator_->AddLayer(TerrainLayer("Bedrock", "bedrock", 1, 0.1f, 2, 1, LayerType::Absolute));
	ChunkGenerator_->AddLayer(TerrainLayer("Base Stone", "stone", 39, 64.0f, 9, 1, LayerType::Absolute));
	ChunkGenerator_->AddLayer(TerrainLayer("Mountain 1", "stone", 1, 129.0f, 60, 1, LayerType::Absolute));
	ChunkGenerator_->AddLayer(TerrainLayer("Mountain 2", "stone", 4, 111.0f, 58, 1, LayerType::Absolute));
	ChunkGenerator_->AddLayer(TerrainLayer("Base Dirt 1", "dirt", 60, 16.0f, 4, 1, LayerType::Absolute));
	ChunkGenerator_->AddLayer(TerrainLayer("Base Dirt 2", "dirt", 59, 25.0f, 6, 1, LayerType::Absolute));
	ChunkGenerator_->AddLayer(TerrainLayer("Surface", "dirt", 1, 1.0f, 1, 1, LayerType::Additive));
	ChunkGenerator_->AddLayer(TerrainLayer("cave", "air", 1, 10.0f, 10, 10, LayerType::Absolute));


	//===========================
	// Initialise Chunk Building
	//===========================

	// Positions within a radius around us
	for (int y = World::LoadRadius; y >= -World::LoadRadius; y--)
	{
		for (int z = -World::LoadRadius; z <= World::LoadRadius; z++)
		{
			for (int x = -World::LoadRadius; x <= World::LoadRadius; x++)
			{
				LocalChunks_.push_back(D3DXVECTOR3(x, y, z));
			}
		}
	}

	GenerateLocalChunks();

	// Set starting stats
	AverageLoadTime_ = 0.0f;
	BuildCount_ = 0;

	//======================
	// Initialise Threading
	//======================

	// Retrieve system Information
	GetSystemInfo(&systemInfo);

	// Store how many threads we have, reserve one for thread handler
	MaxThreads_ = systemInfo.dwNumberOfProcessors - 2;

	// Run a thread to handle initial worker threads
	initialChunkLoading = new ManagedThread<void()>;
	initialChunkLoading->SetFunction(std::bind(&VoxelWorld::HandleThreads, this));
	initialChunkLoading->Start(true);

	// Force a wait until initial terrain built
	while (true)
	{
		if (ChunkQueue_.empty())
			break;
	}

	initialChunkLoading->ForceClose();
	initialChunkLoading = 0;
}

bool VoxelWorld::Frame()
{
	// Handle Player Input
	Player_->Frame();

	//=========================
	// Handle Chunk Generation
	//=========================

	GenerateLocalChunks();

	//=======================
	// Handle Chunk Updating
	//=======================

	HandleChunks();

	//==================
	// Handle Building
	//==================

	HandleThreads();

	return true;
}

bool VoxelWorld::Render()
{
	//======================
	// Render Active Chunks
	//======================

	for (it_wc it = Map_.begin(); it != Map_.end(); it++)
	{
		if (it->second)
		{
			Result_ = it->second->Render();
			if (!Result_)
			{
				return false;
			}
		}
	}

	return true;
}

void VoxelWorld::HandleThreads()
{
	HandleActiveThreads();
	HandleBuildList();
}

void VoxelWorld::HandleActiveThreads()
{
	if (ManagedThreads_.empty())
	{
		return;
	}

	//=========================
	// Remove Finished Threads
	//=========================

	for (vector<ManagedThread<void()>*>::iterator it = ManagedThreads_.begin(); it != ManagedThreads_.end();)
	{
		ManagedThread<void()>* chunkThread = *it;

		// Remove finished threads
		if (chunkThread->TryJoin())
		{
			it = ManagedThreads_.erase(it);
		}
		else
		{
			++it;
		}

		// Clean Up
		chunkThread = 0;
	}
}

void VoxelWorld::HandleBuildList()
{
	D3DXVECTOR3 buildTarget;
	WorkerType workerType;
	ManagedThread<void()>* chunkThread;

	// Make sure we have chunks to deal with
	if (ChunkQueue_.empty())
	{
		return;
	}

	// Make sure we have available threads
	if (ManagedThreads_.size() >= MaxThreads_)
	{
		return;
	}

	// Get the first job
	buildTarget = ChunkQueue_.front().chunkTarget;
	workerType = ChunkQueue_.front().workerType;

	//============
	// Handle Job
	//============

	if (workerType == WorkerType::Build)
	{
		// Create a build thread
		chunkThread = new ManagedThread<void()>;
		chunkThread->SetFunction(std::bind(&VoxelWorld::BuildChunk, this, buildTarget));
		chunkThread->Start();
		ManagedThreads_.push_back(chunkThread);
	}
	else if (workerType == WorkerType::Update)
	{
		// Run a thread to update the chunk
	}

	// Remove chunk from build list
	ChunkQueue_.erase(ChunkQueue_.begin());

	return;
}

void VoxelWorld::BuildChunk(D3DXVECTOR3 chunkIndex)
{
	string chunkKey;
	Chunk* createdChunk;

	// Generate a key for the chunk
	chunkKey = GetKey(chunkIndex.x, chunkIndex.y, chunkIndex.z);

	// Search for the chunk
	it_wc iterator = Map_.find(chunkKey);
	if (iterator == Map_.end())
	{
		float time = timeGetTime();

		// Create the chunk
		createdChunk = new Chunk;
		createdChunk->Initialise(chunkIndex.x, chunkIndex.y, chunkIndex.z);
		ChunkGenerator_->GenerateChunk(*createdChunk);
		createdChunk->RefreshVisible();

		Map_[chunkKey] = createdChunk;
		createdChunk = 0;

		OutputToDebug("Created Chunk : " + chunkKey);
		OutputTimeDelay("Time Taken : ", time, timeGetTime());

		AverageLoadTime_ += timeGetTime() - time;
		BuildCount_++;
	}
	else
	{
		OutputToDebug("Found Existing Chunk : " + chunkKey);

		// Check if we have loaded it
		if (iterator->second)
		{
			// Chunk loaded
			return;
		}
		else
		{
			string chunkFilename = "chunks/" + chunkKey + ".bin";

			// Load chunk from bin
			FILE* pFile;
			fopen_s(&pFile, chunkFilename.c_str(), "rb");

			if (pFile != NULL)
			{
				//loadedChunk->Initialise(chunkIndex.x, chunkIndex.y, chunkIndex.z);

				Map_[chunkKey] = new Chunk;
				Map_[chunkKey]->Initialise(chunkIndex.x, chunkIndex.y, chunkIndex.z);

				// Load file into new chunk
				fread(Map_[chunkKey], sizeof(Chunk), 1, pFile);

				// Close File
				fclose(pFile);
				OutputToDebug("File Loaded!");
			}
			else
			{
				// No file loaded so generate again?
			}
		}
	}
}

void VoxelWorld::HandleChunks()
{
	D3DXVECTOR3 chunkKey;

	//===============
	// Update Chunks
	//===============

	for (it_wc it = Map_.begin(); it != Map_.end(); it++)
	{
		// Make sure we have a chunk to update
		if (it->second)
		{
			// Update the chunk
			it->second->Frame();

			// Convert key from string to vector3
			chunkKey = GetKey(it->first);

			if (it->second->IsDirty())
			{
				// Add to list for refreshing
				ChunkQueue_.push_back(ChunkTarget(chunkKey, WorkerType::Update));
			}
			else if (it->second->IsOutOfRange())
			{
				// Add extension
				string chunkFilename = "chunks/" + it->first + ".bin";

				// Run a thread to unload the chunk
				FILE* pFile = fopen(chunkFilename.c_str(), "wb");

				if (pFile != NULL)
				{
					// Write to file
					fwrite(it->second, sizeof(Chunk), 1, pFile);
					fclose(pFile);

					// Shutdown chunk
					it->second->Shutdown();
					it->second = 0;
				}
			}
		}
	}
}

void VoxelWorld::GenerateLocalChunks()
{
	D3DXVECTOR3 chunkIndex;

	//================
	// Check Movement
	//================

	// Calculate the chunk column we are in
	chunkIndex.x = floor(Camera::Instance()->GetTransform()->GetPosition().x / (float)World::ChunkSize);
	chunkIndex.y = floor(Camera::Instance()->GetTransform()->GetPosition().y / (float)World::ChunkSize);
	chunkIndex.z = floor(Camera::Instance()->GetTransform()->GetPosition().z / (float)World::ChunkSize);

	// Check if we have moved chunk since last check
	if (chunkIndex == LastChunkPosition_)
	{
		return;
	}

	// Store the new chunk we are in
	LastChunkPosition_ = chunkIndex;

	//================
	// Chunk Building
	//================

	// Force to load centre first
	ChunkQueue_.push_back(ChunkTarget(chunkIndex + Vector::Zero, WorkerType::Build));

	// Load chunks around us
	for (unsigned int i = 0; i < LocalChunks_.size(); i++)
	{
		// Add to build list
		ChunkQueue_.push_back(ChunkTarget(chunkIndex + LocalChunks_[i], WorkerType::Build));
	}
}

void VoxelWorld::SetBlock(int x, int y, int z, string blockName)
{
	Block* currentBlock;

	// Get the block
	currentBlock = GetBlock(x, y, z);

	// Make sure its a valid block
	if (!currentBlock)
		return;

	// Update the block
	currentBlock->CopyFrom(World::Blocks[blockName]);
}

Block* VoxelWorld::GetBlock(int x, int y, int z)
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
	// Get Chunk
	//===========

	// Find the chunk
	it_wc it = Map_.find(GetKey(chunkX, chunkY, chunkZ));
	if (it == Map_.end())
	{
		return 0;
	}

	// Get the chunk
	currentChunk = it->second;
	if (!currentChunk)
	{
		return 0;
	}

	//===========
	// Get Block
	//===========

	// Calculate block index
	blockX = x - currentChunk->GetPosition().x;
	blockY = y - currentChunk->GetPosition().y;
	blockZ = z - currentChunk->GetPosition().z;

	// Try and get block
	currentBlock = currentChunk->GetBlock(blockX, blockY, blockZ);
	if (!currentBlock)
	{
		return 0;
	}

	return currentBlock;
}