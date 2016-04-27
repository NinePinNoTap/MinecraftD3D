#include "VoxelWorld.h"

VoxelWorld::VoxelWorld()
{
}

VoxelWorld::~VoxelWorld()
{
}

void VoxelWorld::Initialise()
{
	SYSTEM_INFO systemInfo;

	//===================
	// Initialise Player
	//===================

	Player_ = new Player;
	Player_->Initialise();
	Player_->SetHeight(2.0f);

	//===========================
	// Initialise Chunk Building
	//===========================

	// Positions within a radius
	for (int z = -World::LoadRadius; z <= World::LoadRadius; z++)
	{
		for (int x = -World::LoadRadius; x <= World::LoadRadius; x++)
		{
			LocalChunks_.push_back(D3DXVECTOR3(x, 0, z));
		}
	}
	
	//-- MAYBE HAVE THIS IN A THREAD WE WAIT ON
	//-- THEN IT FORCES THE LOADING SCREEN TO WAIT
	// Generate local chunks
	GenerateLocalChunks();

	//======================
	// Initialise Threading
	//======================

	// Retrieve system Information
	GetSystemInfo(&systemInfo);

	// Store how many threads we have, reserve one for thread handler
	MaxThreads_ = systemInfo.dwNumberOfProcessors - 2;

	// Run a thread to handle worker threads
	ThreadHandler_ = thread(&VoxelWorld::HandleThreads, this);

	// Force a wait until initial terrain built
	while (true)
	{
		if (BuildList_.empty())
			break;
	}
}

bool VoxelWorld::Frame()
{
	Player_->Frame();

	//=========================
	// Handle Chunk Generation
	//=========================

	//GenerateLocalChunks();

	//======================
	// Update Active Chunks
	//======================

	HandleChunks();

	return true;
}

bool VoxelWorld::Render()
{
	//======================
	// Render Active Chunks
	//======================

	for (it_wc it = Map_.begin(); it != Map_.end(); it++)
	{
		Result_ = it->second->Render();
		if (!Result_)
		{
			return false;
		}
	}

	return true;
}

void VoxelWorld::HandleThreads()
{
	while (true)
	{
		HandleActiveThreads();
		HandleBuildList();
	}
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
	// Make sure we have chunks to deal with
	if (BuildList_.empty())
	{
		return;
	}

	// Make sure we have available threads
	if (ManagedThreads_.size() >= MaxThreads_)
	{
		return;
	}

	// Get the first job
	D3DXVECTOR3 buildTarget = BuildList_.front();

	// Create a build thread
	ManagedThread<void()>* chunkThread = new ManagedThread<void()>;
	chunkThread->SetFunction(std::bind(&VoxelWorld::BuildChunk, this, buildTarget));
	chunkThread->Start();
	ManagedThreads_.push_back(chunkThread);

	// Remove chunk from build list
	BuildList_.erase(BuildList_.begin());

	return;
}

void VoxelWorld::BuildChunk(D3DXVECTOR3 chunkIndex)
{
	string chunkKey;

	// Generate a key for the chunk
	chunkKey = GetKey(chunkIndex.x, chunkIndex.y, chunkIndex.z);

	// Search for the chunk
	it_wc iterator = Map_.find(chunkKey);
	if (iterator == Map_.end())
	{
		// Create the chunk
		Map_[chunkKey] = new ChunkColumn;
		Map_[chunkKey]->Initialise(chunkIndex.x, chunkIndex.z, World::ColumnHeight);

		OutputToDebug("Created Chunk : " + chunkKey);
	}
	else
	{
		OutputToDebug("Found Existing Chunk : " + chunkKey);

		//--TODO
		// Chunk was found in the map
		// We should look to load it
	}
}

void VoxelWorld::HandleChunks()
{
	//===============
	// Update Chunks
	//===============

	for (it_wc it = Map_.begin(); it != Map_.end(); it++)
	{
		it->second->Frame();
	}
}

void VoxelWorld::GenerateLocalChunks()
{
	D3DXVECTOR3 chunkIndex;

	//================
	// Check Movement
	//================

	// Calculate the chunk we are in
	chunkIndex.x = floor(Camera::Instance()->GetTransform()->GetPosition().x / (float)World::ChunkSize);
	chunkIndex.y = 0;
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

	// Force to load centre
	BuildList_.push_back(D3DXVECTOR3(0, 0, 0));

	// Build chunks around us
	for (unsigned int i = 0; i < LocalChunks_.size(); i++)
	{
		// Add to build list
		BuildList_.push_back(chunkIndex + LocalChunks_[i]);
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

	chunkX = floor(chunkX);
	chunkZ = floor(chunkZ);

	//=======================
	// Calculate Chunk Index
	//=======================

	chunkY = y / (float)World::ChunkSize;
	chunkY = floor(chunkY);

	// Find the chunk
	it_wc it = Map_.find(GetKey(chunkX, 0.0f, chunkZ));
	if (it == Map_.end())
	{
		return 0;
	}

	// Get the chunk
	currentChunk = it->second->GetChunk(chunkY);
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