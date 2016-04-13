#include "VoxelTerrain.h"

VoxelTerrain::VoxelTerrain()
{
}

VoxelTerrain::~VoxelTerrain()
{
}

void VoxelTerrain::Initialise()
{
	//======================
	// Create Block Manager
	//======================

	BlockManager_ = new BlockManager;
	BlockManager_->Initialise();

	//==========================
	// Initialise Chunk Updates
	//==========================

	BuildThread_ = thread(&VoxelTerrain::BuildChunks, this);
	UpdateDelay_ = 3;
	LastChunkCheck_ = 0;
}

void VoxelTerrain::Frame()
{
	//======================
	// Update Active Chunks
	//======================

	for (it_type iterator = ChunkMap_.begin(); iterator != ChunkMap_.end(); iterator++)
	{
		// Update chunk
		iterator->second->Frame();
	}

	//==================
	// Chunk Generation
	//==================

	// Check if enough time has elapsed
	if ((timeGetTime() - LastChunkCheck_) < UpdateDelay_)
	{
		return;
	}

	// Get the position of the camera
	D3DXVECTOR3 cameraPos = Camera::Instance()->GetTransform()->GetPosition();

	// Calculate chunk index
	D3DXVECTOR3 chunkIndex = { floor(cameraPos.x / (float)CHUNK_SIZE),
							   floor(cameraPos.y / (float)CHUNK_SIZE),
							   floor(cameraPos.z / (float)CHUNK_SIZE) };

	// Check if we have moved from the last chunk
	if (chunkIndex == LastChunkPos_)
	{
		return;
	}

	// Get the current chunk we are now in
	Chunk* currentChunk = GetChunk(cameraPos.x, cameraPos.y, cameraPos.z);
	// Find the chunk we are in
	if (currentChunk)
	{
		// We are in a chunk
		// Check if it active
		// If active then continue
		// If not make it active, pull from file etc
		currentChunk = 0;
	}
	else
	{
		// Check if the chun is already on the build list
		if (BuildList_.empty())
		{
			OutputToDebug("New chunk found - Please generate me");

			// Add to build list
			BuildList_.push_back(chunkIndex);
		}
		else if (find(BuildList_.begin(), BuildList_.end(), chunkIndex) == BuildList_.end())
		{
			OutputToDebug("New chunk found - Please generate me");

			// Add to build list
			BuildList_.push_back(chunkIndex);
		}
	}
}

void VoxelTerrain::Render()
{
	//==============
	// Render Chunk
	//==============

	for (it_type iterator = ChunkMap_.begin(); iterator != ChunkMap_.end(); iterator++)
	{
		// Check if the chunk is active
		if (iterator->second->IsVisible())
		{
			// Render it
			iterator->second->Render();
		}
	}
}

void VoxelTerrain::LinkBlocks(Chunk* chunk)
{
	D3DXVECTOR3 blockPos;

	// Loop through blocks in chunk
	for (int x = 0; x < CHUNK_BLOCKS; x++)
	{
		for (int y = 0; y < CHUNK_BLOCKS; y++)
		{
			for (int z = 0; z < CHUNK_BLOCKS; z++)
			{
				// Calculate block index in world space
				blockPos = chunk->GetPosition() + D3DXVECTOR3(x, y, z);

				// Config the block neighbours
				SetBlockNeighbours(blockPos.x, blockPos.y, blockPos.z);
			}
		}
	}
}

void VoxelTerrain::SetBlockNeighbours(int x, int y, int z)
{
	//===========================
	// Apply neighbours to block
	//===========================

	Block* blockCurrent;

	// Get current block
	blockCurrent = GetBlock(x, y, z);

	if (!blockCurrent)
		return;

	// Set block neighbours
	blockCurrent->SetNeighbour(Direction::Up, GetBlock(x, y + 1, z));
	blockCurrent->SetNeighbour(Direction::Down, GetBlock(x, y - 1, z));

	blockCurrent->SetNeighbour(Direction::Left, GetBlock(x - 1, y, z));
	blockCurrent->SetNeighbour(Direction::Right, GetBlock(x + 1, y, z));

	blockCurrent->SetNeighbour(Direction::Forward, GetBlock(x, y, z + 1));
	blockCurrent->SetNeighbour(Direction::Backward, GetBlock(x, y, z - 1));
}

void VoxelTerrain::BuildChunks()
{
	while (true)
	{
		// Make sure we have chunks to build
		if (BuildList_.empty())
		{
			continue;
		}

		// Get current working chunk
		D3DXVECTOR3 chunkIndex = BuildList_[0];

		// Get time before build started
		float tBefore = timeGetTime();

		//================
		// Generate Chunk
		//================

		// Create chunk and generate its terrain
		Chunk* currentChunk = new Chunk;
		currentChunk->Initialise(chunkIndex.x, chunkIndex.y, chunkIndex.z);
		currentChunk->Generate();
		
		// Update block visibility
		LinkBlocks(currentChunk); // Causing time delays -- Better way?
		currentChunk->RefreshVisible();

		// Add Chunk to Map
		ChunkMap_[GetKey(chunkIndex.x, chunkIndex.y, chunkIndex.z)] = currentChunk;

		// DEBUGGING PURPOSES
		OutputToDebug(GetKey(chunkIndex.x, chunkIndex.y, chunkIndex.z));
		OutputTimeDelay("Generation Time ", tBefore, timeGetTime());

		// Clean Up
		currentChunk = 0;
		BuildList_.erase(BuildList_.begin());
	}
}

// Getters
Chunk* VoxelTerrain::GetChunk(int x, int y, int z)
{
	float chunkX, chunkY, chunkZ;

	// Calculate chunk index
	chunkX = x / (float)CHUNK_SIZE;
	chunkY = y / (float)CHUNK_SIZE;
	chunkZ = z / (float)CHUNK_SIZE;

	chunkX = floor(chunkX);
	chunkY = floor(chunkY);
	chunkZ = floor(chunkZ);

	// Try and get chunk from map
	if (ChunkMap_.count(GetKey(chunkX, chunkY, chunkZ)))
	{
		return ChunkMap_[GetKey(chunkX, chunkY, chunkZ)];
	}

	return 0;
}

Block* VoxelTerrain::GetBlock(int x, int y, int z)
{
	Chunk* chunk = 0;
	Block* block = 0;
	int blockX, blockY, blockZ;

	// Try and get chunk
	chunk = GetChunk(x, y, z);
	if (!chunk)
	{
		return 0;
	}

	// Calculate block index
	blockX = x - chunk->GetPosition().x;
	blockY = y - chunk->GetPosition().y;
	blockZ = z - chunk->GetPosition().z;

	// Try and get block
	block = chunk->GetBlock(blockX, blockY, blockZ);
	if (!block)
	{
		return 0;
	}

	return block;
}