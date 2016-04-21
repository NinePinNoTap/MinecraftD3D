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
	ChunkBlock_->SetRenderModes(RenderMode::On, RenderMode::On, RenderMode::Off, BlendMode::NoBlending);

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
}

void Chunk::Shutdown()
{
	// Delete the blocks
	for (int x = 0; x < CHUNK_SIZE; x++)
	{
		// Loop through y dimension
		for (int y = 0; y < CHUNK_SIZE; y++)
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
	for (int x = 0; x < CHUNK_SIZE; x++)
	{
		// Loop through y dimension
		for (int y = 0; y < CHUNK_SIZE; y++)
		{
			// Loop through z dimension
			for (int z = 0; z < CHUNK_SIZE; z++)
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
	//if (distanceAway > 4 * CHUNK_SIZE)
	//{
	//	IsVisible_ = false;
	//}
	//else
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
	for (int x = 0; x < CHUNK_SIZE; x++)
	{
		// Loop through y dimension
		for (int y = 0; y < CHUNK_SIZE; y++)
		{
			// Loop through z dimension
			for (int z = 0; z < CHUNK_SIZE; z++)
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
	Blocks_ = new Block**[CHUNK_SIZE];

	// Loop through x dimension
	for(int x = 0; x < CHUNK_SIZE; x++)
	{
		Blocks_[x] = new Block*[CHUNK_SIZE];

		// Loop through y dimension
		for (int y = 0; y < CHUNK_SIZE; y++)
		{
			Blocks_[x][y] = new Block[CHUNK_SIZE];

			// Loop through z dimension
			for (int z = 0; z < CHUNK_SIZE; z++)
			{
				// Define position of the block
				xPos = Position_.x + x;
				yPos = Position_.y + y;
				zPos = Position_.z + z;

				// Store it
				Blocks_[x][y][z].CopyFrom(BlockManager::Instance()->GetBlock("air"));
				Blocks_[x][y][z].SetPosition(xPos, yPos, zPos);
			}
		}
	}

	// Loop through x dimension
	for (int x = 0; x < CHUNK_SIZE; x++)
	{
		// Loop through y dimension
		for (int y = 0; y < CHUNK_SIZE; y++)
		{
			// Loop through z dimension
			for (int z = 0; z < CHUNK_SIZE; z++)
			{
				// Set block neighbours
				Blocks_[x][y][z].SetNeighbour(Direction::Up, GetBlock(x, y + 1, z));
				Blocks_[x][y][z].SetNeighbour(Direction::Down, GetBlock(x, y - 1, z));

				Blocks_[x][y][z].SetNeighbour(Direction::Left, GetBlock(x - 1, y, z));
				Blocks_[x][y][z].SetNeighbour(Direction::Right, GetBlock(x + 1, y, z));

				Blocks_[x][y][z].SetNeighbour(Direction::Forward, GetBlock(x, y, z + 1));
				Blocks_[x][y][z].SetNeighbour(Direction::Backward, GetBlock(x, y, z - 1));
			}
		}
	}
}