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

int bedrockBaseHeight = 0;

int diamondBaseHeight = 4;
int diamondMaxHeight = 13;
int diamondNoise = diamondMaxHeight - diamondBaseHeight;

int stoneBaseHeight = 16;
int dirtBaseHeight = 4;

void Chunk::GenerateColumn(int x, int z)
{
	// Generate Bedrock
	int bedrockHeight = floor(bedrockBaseHeight);
	bedrockHeight += GetNoise(x, 0, z, 0.5f, 2);

	// Generate Diamond
	int diamondHeight = bedrockHeight + floor(diamondBaseHeight);
	//Clamp(diamondHeight, diamondBaseHeight, 1000);
	//diamondHeight += GetNoise(x, 0, z, 100.0f, diamondNoise);

	// Generate Stone
	int stoneHeight = floor(stoneBaseHeight);
	//stoneHeight += GetNoise(x, 0, z, 0.008f, 12);
	//stoneHeight += GetNoise(x, 0, z, 0.05f, 4);

	int dirtHeight = stoneHeight + floor(dirtBaseHeight);
	dirtHeight += GetNoise(x, 100, z, 0.04f, 3);

	// Loop through y dimension
	for (int y = Position_.y; y < Position_.y + CHUNK_SIZE; y++)
	{
		//Get a value to base cave generation on
		int caveChance = GetNoise(x, y, z, 0.025f, 100);

		Block* currentBlock = GetBlock(x - Position_.x, y - Position_.y, z - Position_.z);

		if (y < bedrockHeight)
		{
			currentBlock->CopyFrom(BlockManager::Instance()->GetBlock("air"));
		}
		else if (y == bedrockHeight)
		{
			currentBlock->CopyFrom(BlockManager::Instance()->GetBlock("bedrock"));
		}
		else if (y == diamondHeight)
		{
			currentBlock->CopyFrom(BlockManager::Instance()->GetBlock("diamond"));
		}
		else if (y < stoneHeight)
		{
			currentBlock->CopyFrom(BlockManager::Instance()->GetBlock("stone"));
		}
		else if (y < dirtHeight)
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

		// Clean Up
		currentBlock = 0;
	}
}