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
	IsGenerated_ = false;
}

void Chunk::Generate()
{
	if (IsGenerated_)
	{
		// Get filename
		string chunkFilename = GetKey(Position_.x, Position_.y, Position_.z);

		// Load data from file
		FILE* pFile = fopen(chunkFilename.c_str(), "rb");

		if (pFile != NULL)
		{
			fread(this, sizeof(Chunk), 1, pFile);
			fclose(pFile);
		}
		else
		{
			// No file loaded so generate again
		}
	}
	else
	{
		// Generate new chunk
		GenerateChunk();
	}
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
				xPos = Position_.x + (x * BLOCK_SIZE);
				yPos = Position_.y + (y * BLOCK_SIZE);
				zPos = Position_.z + (z * BLOCK_SIZE);
				
				// Store it
				Blocks_[x][y][z].CopyFrom(BlockManager::Instance()->GetBlock("air"));
				Blocks_[x][y][z].SetPosition(xPos, yPos, zPos);
			}
		}
	}
}

void Chunk::GenerateChunk()
{
	PerlinNoiseGenerator perlinNoise;

	// Generate a random seed for the noise to use
	perlinNoise.SetSeed(100);
	
	for (int x = Position_.x; x < Position_.x + CHUNK_BLOCKS; x++)
	{
		for (int z = Position_.z; z < Position_.z + CHUNK_BLOCKS; z++)
		{
			double a = (double)z / (double)CHUNK_BLOCKS;
			double b = (double)x / (double)CHUNK_BLOCKS;
	
			float noise = perlinNoise.CreateNoise(a, b, 0.8f);
			float height = floor(CHUNK_BLOCKS * noise);
	
			// Loop through y dimension
			for (int y = Position_.y; y < Position_.y + height; y++)
			{	
				Block* currentBlock = GetBlock(x - Position_.x, y - Position_.y, z - Position_.z);

				if (y < 3)
				{
					int r = rand() % 50;
					if (r < 3)
					{
						currentBlock->CopyFrom(BlockManager::Instance()->GetBlock("diamond"));
					}
					else
					{
						currentBlock->CopyFrom(BlockManager::Instance()->GetBlock("sand"));
					}
				}
				else if (y < 4)
				{
					currentBlock->CopyFrom(BlockManager::Instance()->GetBlock("stone"));
				}
				else if (y < 11)
				{
					currentBlock->CopyFrom(BlockManager::Instance()->GetBlock("sand"));
				}
				else if (y < 16)
				{
					currentBlock->CopyFrom(BlockManager::Instance()->GetBlock("dirt"));
				}
				else
				{
					currentBlock->CopyFrom(BlockManager::Instance()->GetBlock("air"));
				}

				// Clean Up
				currentBlock = 0;
			}
		}
	}
}