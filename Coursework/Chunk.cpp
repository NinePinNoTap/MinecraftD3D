#include "Chunk.h"

Chunk::Chunk()
{
	Chunk_ = 0;
	ChunkBlock_ = 0;
}

Chunk::~Chunk()
{
}

void Chunk::Initialise(int x, int y, int z)
{
	// Store the ID of the chunk
	ChunkID_ = D3DXVECTOR3(x, y, z);

	//==================
	// Create Transform
	//==================

	Position_ = D3DXVECTOR3(CHUNK_WIDTH * x, CHUNK_HEIGHT * y, CHUNK_DEPTH * z);

	//================
	// Generate Chunk
	//================

	GenerateBlankChunk();
		
	//===========================
	// Create Representing Block
	//===========================

	ChunkBlock_ = new InstancedGameObject;
	ChunkBlock_->Initialise("block.obj");
	ChunkBlock_->SetShader("instancedlight");

	//==================
	// Initialise Flags
	//==================

	IsVisible_ = true;
}

void Chunk::Shutdown()
{
	// Delete the blocks
	for (int x = 0; x < NO_OF_BLOCKS_WIDTH; x++)
	{
		// Loop through y dimension
		for (int y = 0; y < NO_OF_BLOCKS_HEIGHT; y++)
		{
			delete[] Chunk_[x][y];
		}

		delete[] Chunk_[x];
	}

	delete[] Chunk_;
}

void Chunk::Build()
{
	// Loop through x dimension
	for (int x = 0; x < NO_OF_BLOCKS_WIDTH; x++)
	{
		// Loop through y dimension
		for (int y = 0; y < NO_OF_BLOCKS_HEIGHT; y++)
		{
			// Loop through z dimension
			for (int z = 0; z < NO_OF_BLOCKS_DEPTH; z++)
			{
				Chunk_[x][y][z].Refresh();
			}
		}
	}

	// Loop through x dimension
	for (int x = 0; x < NO_OF_BLOCKS_WIDTH; x++)
	{
		// Loop through y dimension
		for (int y = 0; y < NO_OF_BLOCKS_HEIGHT; y++)
		{
			// Loop through z dimension
			for (int z = 0; z < NO_OF_BLOCKS_DEPTH; z++)
			{
				if (Chunk_[x][y][z].IsActive() && Chunk_[x][y][z].IsSolid())
				{
					ChunkBlock_->AddInstance(Chunk_[x][y][z].GetInstance());
				}
			}
		}
	}

	ChunkBlock_->RebuildInstanceBuffer();
}

// Frame
void Chunk::Frame()
{
	// Check distance to player
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
	for (int x = 0; x < NO_OF_BLOCKS_WIDTH; x++)
	{
		// Loop through y dimension
		for (int y = 0; y < NO_OF_BLOCKS_HEIGHT; y++)
		{
			// Loop through z dimension
			for (int z = 0; z < NO_OF_BLOCKS_DEPTH; z++)
			{
				// Store it
				Chunk_[x][y][z].CopyFrom(BlockManager::Instance()->GetBlock(blockName));
			}
		}
	}
}

// Terrain Generation
void Chunk::GenerateBlankChunk()
{
	float xPos, yPos, zPos;

	// Create the 3D array to store blocks in chunk
	Chunk_ = new Block**[NO_OF_BLOCKS_WIDTH];

	// Loop through x dimension
	for(int x = 0; x < NO_OF_BLOCKS_WIDTH; x++)
	{
		Chunk_[x] = new Block*[NO_OF_BLOCKS_HEIGHT];

		// Loop through y dimension
		for (int y = 0; y < NO_OF_BLOCKS_HEIGHT; y++)
		{
			Chunk_[x][y] = new Block[NO_OF_BLOCKS_DEPTH];

			// Loop through z dimension
			for (int z = 0; z < NO_OF_BLOCKS_DEPTH; z++)
			{
				// Define position of the block
				xPos = Position_.x + (x * BLOCK_SIZE) - (CHUNK_WIDTH / 2);
				yPos = Position_.y + (y * BLOCK_SIZE) - (CHUNK_HEIGHT / 2);
				zPos = Position_.z + (z * BLOCK_SIZE) - (CHUNK_DEPTH / 2);
				
				// Store it
				Chunk_[x][y][z].CopyFrom(BlockManager::Instance()->GetBlock("air"));
				Chunk_[x][y][z].SetPosition(xPos, yPos, zPos);
			}
		}
	}
}

// Getters
bool Chunk::IsVisible()
{
	return IsVisible_;
}