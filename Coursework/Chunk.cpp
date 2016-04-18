#include "Chunk.h"

Chunk::Chunk()
{
	Blocks_ = 0;
	ChunkBlock_ = 0;
}

Chunk::~Chunk()
{
}

void Chunk::Initialise(int x, int y, int z)
{
	//==================
	// Create Transform
	//==================

	Position_ = D3DXVECTOR3(x * CHUNK_SIZE, y * CHUNK_SIZE, z * CHUNK_SIZE);

	//===========================
	// Create Representing Block
	//===========================

	ChunkBlock_ = new InstancedGameObject;
	ChunkBlock_->Initialise("block.obj");
	ChunkBlock_->SetShader("instancedlight");

	//================
	// Generate Chunk
	//================

	GenerateBlankChunk();

	//==================
	// Initialise Flags
	//==================

	IsVisible_ = true;
	IsGenerated_ = false;
}

void Chunk::Generate()
{
	/*if (IsGenerated_)
	{
		
	}
	else
	{
		IsGenerated_ = true;
		IsVisible_ = true;
	}*/
}

void Chunk::Shutdown()
{
	// Delete the blocks
	for (int x = 0; x < CHUNK_BLOCKS; x++)
	{
		// Loop through y dimension
		for (int y = 0; y < CHUNK_BLOCKS; y++)
		{
			delete[] Blocks_[x][y];
		}

		delete[] Blocks_[x];
	}

	delete[] Blocks_;
}

void Chunk::RefreshVisible()
{
	// Loop through x dimension
	for (int x = 0; x < CHUNK_BLOCKS; x++)
	{
		// Loop through y dimension
		for (int y = 0; y < CHUNK_BLOCKS; y++)
		{
			// Loop through z dimension
			for (int z = 0; z < CHUNK_BLOCKS; z++)
			{
				Blocks_[x][y][z].Refresh();

				if (Blocks_[x][y][z].IsActive() && Blocks_[x][y][z].IsSolid())
				{
					ChunkBlock_->AddInstance(Blocks_[x][y][z].GetInstance());
				}
			}
		}
	}

	// Compile the instance data to buffer
	ChunkBlock_->RebuildInstanceBuffer();
}

// Frame
void Chunk::Frame()
{
	//float distanceAway;

	//// Check distance to player
	//distanceAway = Distance(Camera::Instance()->GetTransform()->GetPosition(), Position_);
	//if (distanceAway > 30)
	//{
	//	IsVisible_ = false;
	//}
	//else if (distanceAway < 20)
	//{
	//	IsVisible_ = true;
	//}

	// Set IsActive based on that
	// We need Neighbours for chunks as well
}

void Chunk::Render()
{
	if (!IsVisible_)
	{
		return;
	}
	
	ChunkBlock_->Render();
}

void Chunk::SetBlocks(string blockName)
{
	// Loop through x dimension
	for (int x = 0; x < CHUNK_BLOCKS; x++)
	{
		// Loop through y dimension
		for (int y = 0; y < CHUNK_BLOCKS; y++)
		{
			// Loop through z dimension
			for (int z = 0; z < CHUNK_BLOCKS; z++)
			{
				// Store it
				Blocks_[x][y][z].CopyFrom(BlockManager::Instance()->GetBlock(blockName));
			}
		}
	}
}

// Terrain Generation
void Chunk::GenerateBlankChunk()
{
	float xPos, yPos, zPos;

	// Create the 3D array to store blocks in chunk
	Blocks_ = new Block**[CHUNK_BLOCKS];

	// Loop through x dimension
	for(int x = 0; x < CHUNK_BLOCKS; x++)
	{
		Blocks_[x] = new Block*[CHUNK_BLOCKS];

		// Loop through y dimension
		for (int y = 0; y < CHUNK_BLOCKS; y++)
		{
			Blocks_[x][y] = new Block[CHUNK_BLOCKS];

			// Loop through z dimension
			for (int z = 0; z < CHUNK_BLOCKS; z++)
			{
				// Define position of the block
				xPos = Position_.x + (x * BLOCK_SIZE) - (CHUNK_SIZE / 2);
				yPos = Position_.y + (y * BLOCK_SIZE);
				zPos = Position_.z + (z * BLOCK_SIZE) - (CHUNK_SIZE / 2);

				// Store it
				Blocks_[x][y][z].CopyFrom(BlockManager::Instance()->GetBlock("dirt"));
				Blocks_[x][y][z].SetPosition(xPos, yPos, zPos);
			}
		}
	}

	// Loop through x dimension
	for (int x = 0; x < CHUNK_BLOCKS; x++)
	{
		// Loop through y dimension
		for (int y = 0; y < CHUNK_BLOCKS; y++)
		{
			// Loop through z dimension
			for (int z = 0; z < CHUNK_BLOCKS; z++)
			{
				// Set block neighbours
				Blocks_[x][y][z].SetNeighbour(Direction::Up, GetBlock(x, y + 1, z));
				Blocks_[x][y][z].SetNeighbour(Direction::Down, GetBlock(x, y - 1, z));

				Blocks_[x][y][z].SetNeighbour(Direction::Left, GetBlock(x - 1, y, z));
				Blocks_[x][y][z].SetNeighbour(Direction::Right, GetBlock(x + 1, y, z));

				Blocks_[x][y][z].SetNeighbour(Direction::Forward, GetBlock(x, y, z + 1));
				Blocks_[x][y][z].SetNeighbour(Direction::Backward, GetBlock(x, y, z - 1));

				Blocks_[x][y][z].Refresh();

				if (Blocks_[x][y][z].IsActive())
					ChunkBlock_->AddInstance(Blocks_[x][y][z].GetInstance());
			}
		}
	}

	ChunkBlock_->RebuildInstanceBuffer();
}