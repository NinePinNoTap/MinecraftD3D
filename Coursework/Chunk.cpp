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

	IsVisible_ = false;
	IsGenerated_ = false;
}

void Chunk::Generate()
{
	if (IsGenerated_)
	{
		
	}
	else
	{
		// Generate new chunk
		GenerateChunk();
		IsGenerated_ = true;
		IsVisible_ = true;
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
	// Loop through chunk
	for (int x = Position_.x; x < Position_.x + CHUNK_BLOCKS; x++)
	{
		for (int z = Position_.z; z < Position_.z + CHUNK_BLOCKS; z++)
		{
			GenerateColumn(x, z);
		}
	}

	// Generate initial visuals
	ChunkBlock_->RebuildInstanceBuffer();
}

#include "SimplexNoise.h"

float stoneBaseHeight = -24;
float stoneBaseNoise = 0.05f;
float stoneBaseNoiseHeight = 4;

float stoneMountainHeight = 48;
float stoneMountainFrequency = 0.008f;
float stoneMinHeight = -12;

float dirtBaseHeight = 1;
float dirtNoise = 0.04f;
float dirtNoiseHeight = 3;

float caveFrequency = 0.025f;
int caveSize = 7;

static int GetNoise(int x, int y, int z, float scale, int max)
{
	return floor((SimplexNoise::Noise(x * scale, y * scale, z * scale) + 1.0f) * (max / 2.0f));
}

void Chunk::GenerateColumn(int x, int z)
{
	//PerlinNoise perlinNoise;

	//// Calculate noise factors
	//double a = (double)z / (double)10;
	//double b = (double)x / (double)10;

	//float noise = perlinNoise.CreateNoise(a, b, 0.0f);
	//float height = floor(CHUNK_BLOCKS * noise);

	int stoneHeight = floor(stoneBaseHeight);
	stoneHeight += GetNoise(x, 0, z, stoneMountainFrequency, floor(stoneMountainHeight));

	if (stoneHeight < stoneMinHeight)
		stoneHeight = floor(stoneMinHeight);

	stoneHeight += GetNoise(x, 0, z, stoneBaseNoise, floor(stoneBaseNoiseHeight));

	int dirtHeight = stoneHeight + floor(dirtBaseHeight);
	dirtHeight += GetNoise(x, 100, z, dirtNoise, floor(dirtNoiseHeight));

	// Loop through y dimension
	for (int y = Position_.y; y < Position_.y + CHUNK_SIZE; y++)
	{
		Block* currentBlock = GetBlock(x - Position_.x, y - Position_.y, z - Position_.z);

		//Get a value to base cave generation on
		int caveChance = GetNoise(x, y, z, caveFrequency, 100);

		if (y <= stoneHeight && caveSize < caveChance)
		{
			currentBlock->CopyFrom(BlockManager::Instance()->GetBlock("stone"));
		}
		else if (y <= dirtHeight && caveSize < caveChance)
		{
			currentBlock->CopyFrom(BlockManager::Instance()->GetBlock("dirt"));
		}
		else
		{
			currentBlock->CopyFrom(BlockManager::Instance()->GetBlock("air"));
		}

		// Create instance 
		if (currentBlock->IsActive() && currentBlock->IsSolid())
		{
			ChunkBlock_->AddInstance(currentBlock->GetInstance());
		}

		//if (y < 3)
		//{
		//	int r = rand() % 50;
		//	if (r < 3)
		//	{
		//		currentBlock->CopyFrom(BlockManager::Instance()->GetBlock("diamond"));
		//	}
		//	else
		//	{
		//		currentBlock->CopyFrom(BlockManager::Instance()->GetBlock("sand"));
		//	}
		//}
		//else if (y < 4)
		//{
		//	currentBlock->CopyFrom(BlockManager::Instance()->GetBlock("stone"));
		//}
		//else if (y < 11)
		//{
		//	currentBlock->CopyFrom(BlockManager::Instance()->GetBlock("sand"));
		//}
		//else if (y < 16)
		//{
		//	currentBlock->CopyFrom(BlockManager::Instance()->GetBlock("dirt"));
		//}
		//else
		//{
		//	currentBlock->CopyFrom(BlockManager::Instance()->GetBlock("air"));
		//}
		//
		//// Create instance 
		//if (currentBlock->IsActive() && currentBlock->IsSolid())
		//{
		//	ChunkBlock_->AddInstance(currentBlock->GetInstance());
		//}

		// Clean Up
		currentBlock = 0;
	}
}