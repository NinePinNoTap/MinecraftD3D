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

	Transform_ = new Transform;
	Transform_->SetPosition(CHUNK_WIDTH * x, CHUNK_HEIGHT * y, CHUNK_DEPTH * z);

	//================
	// Generate Chunk
	//================

	GenerateBlankChunk();
		
	//==================
	// Initialise Flags
	//==================

	IsVisible_ = true;

	ChunkBlock_ = new InstancedGameObject;
	ChunkBlock_->Initialise("block.obj");
	ChunkBlock_->SetShader("instancedlight");
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
	// Loop through x dimension
	for(int x = 0; x < NO_OF_BLOCKS_WIDTH; x++)
	{
		// Loop through y dimension
		for (int y = 0; y < NO_OF_BLOCKS_HEIGHT; y++)
		{
			// Loop through z dimension
			for (int z = 0; z < NO_OF_BLOCKS_DEPTH; z++)
			{
				//Chunk_[x][y][z].Frame();
			}
		}
	}
}

void Chunk::Render()
{
	if (!IsVisible_)
	{
		return;
	}
	
	ChunkBlock_->Render();
}

// Terrain Generation
void Chunk::GenerateBlankChunk()
{
	D3DXVECTOR3 chunkPosition;
	Block airBlock;
	float xPos, yPos, zPos;

	// Retrieve start position of the chunk
	chunkPosition = GetTransform()->GetPosition();

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
				xPos = chunkPosition.x + (x * BLOCK_SIZE) - (CHUNK_WIDTH / 2);
				yPos = chunkPosition.y + (y * BLOCK_SIZE) - (CHUNK_HEIGHT / 2);
				zPos = chunkPosition.z + (z * BLOCK_SIZE) - (CHUNK_DEPTH / 2);

				// Define base block
				airBlock = BlockManager::Instance()->GetBlock("air");
				airBlock.SetPosition(xPos, yPos, zPos);

				// Store it
				Chunk_[x][y][z] = airBlock;
			}
		}
	}
}

// Getters
bool Chunk::IsVisible()
{
	return IsVisible_;
}

Transform* Chunk::GetTransform()
{
	return Transform_;
}