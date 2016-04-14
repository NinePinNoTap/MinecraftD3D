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
	UpdateThread_ = thread(&VoxelTerrain::UpdateWorld, this);
	NeedsUpdating_ = false;
}

// Frame
void VoxelTerrain::Frame()
{
	// Update active chunks
	HandleChunks();

	// Create chunks
	HandleChunkGeneration();
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

// Handlers
void VoxelTerrain::HandleChunks()
{
	//================
	// Update Chunks
	//================

	for (it_type iterator = ChunkMap_.begin(); iterator != ChunkMap_.end(); iterator++)
	{
		// Make sure it's active
		if (iterator->second)
		{
			iterator->second->Frame();

			if (!iterator->second->IsVisible())
			{
				// Remove
				//iterator->second = 0;
			}

			// NEED SOME CHUNKS FOR DELETING IF OUT OF RANGE
		}
	}
}

void VoxelTerrain::HandleChunkGeneration()
{
	D3DXVECTOR3 offsetPosition;
	D3DXMATRIX rotationMatrix;

	// Calculate current chunk we are in
	D3DXVECTOR3 chunkIndex =
	{
		floor(Camera::Instance()->GetTransform()->GetPosition().x / (float)CHUNK_SIZE),
		floor(Camera::Instance()->GetTransform()->GetPosition().y / (float)CHUNK_SIZE),
		floor(Camera::Instance()->GetTransform()->GetPosition().z / (float)CHUNK_SIZE)
	};

	//====================
	// Progression Checks
	//====================

	// Check for movement
	if (chunkIndex == LastChunkPos_)
	{
		// No change
		return;
	}
	else
	{
		// Store current chunk
		LastChunkPos_ = chunkIndex;
	}

	//================
	// Chunk Building
	//================

	// Build current chunk
	TryBuildChunk(chunkIndex);

	// Build chunks around us
	for (int i = 0; i < 9; i++)
	{
		// Rotate forward vector by angle
		D3DXMatrixRotationYawPitchRoll(&rotationMatrix, D3DXToRadian(i * 45), 0, 0);
		D3DXVec3TransformCoord(&offsetPosition, &FORWARD_VECTOR, &rotationMatrix);
		RoundVector3(offsetPosition);

		// Calculate new chunk index
		TryBuildChunk(chunkIndex + offsetPosition);
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

void VoxelTerrain::TryBuildChunk(D3DXVECTOR3 index)
{
	string chunkKey;
	
	// Generate a key for the chunk
	chunkKey = GetKey(index.x, index.y, index.z);

	// Search for the chunk
	it_type iterator = ChunkMap_.find(chunkKey);
	if (iterator == ChunkMap_.end())
	{
		// Couldn't find the chunk so check if its currently in the build list
		if (find(BuildList_.begin(), BuildList_.end(), index) == BuildList_.end())
		{
			OutputToDebug("New Chunk Found : " + chunkKey);

			// Add to build list
			BuildList_.push_back(index);
		}
	}
	else
	{
		// Chunk was found in the map
		// We should look to load it
	}
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
		D3DXVECTOR3 chunkIndex = BuildList_.front();

		//================
		// Generate Chunk
		//================

		// Create chunk and generate its terrain
		Chunk* currentChunk = new Chunk;
		currentChunk->Initialise(chunkIndex.x, chunkIndex.y, chunkIndex.z);
		currentChunk->Generate();

		// Add Chunk to Map
		ChunkMap_[GetKey(chunkIndex.x, chunkIndex.y, chunkIndex.z)] = currentChunk;

		// Clean Up
		currentChunk = 0;
		BuildList_.erase(BuildList_.begin());
		NeedsUpdating_ = true;
	}
}

void VoxelTerrain::UpdateWorld()
{
	while (true)
	{
		if (!NeedsUpdating_)
		{
			continue;
		}

		// Loop through chunks and refresh visibility
		for (it_type iterator = ChunkMap_.begin(); iterator != ChunkMap_.end(); iterator++)
		{
			// Make sure it's active
			if (iterator->second)
			{
				LinkBlocks(iterator->second);
				iterator->second->RefreshVisible();
			}
		}

		// Unflag
		NeedsUpdating_ = false;
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
	Chunk* currentChunk = 0;
	Block* currentBlock = 0;
	int blockX, blockY, blockZ;

	// Try and get chunk
	currentChunk = GetChunk(x, y, z);
	if (!currentChunk)
	{
		return 0;
	}

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