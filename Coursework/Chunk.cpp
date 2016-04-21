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

	Position_ = D3DXVECTOR3(x * World::ChunkSize, y * World::ChunkSize, z * World::ChunkSize);

	//===========================
	// Create Representing Block
	//===========================

	ChunkBlock_ = new InstancedGameObject;
	ChunkBlock_->Initialise("block.obj");
	ChunkBlock_->SetShader("instancedlight");
	ChunkBlock_->SetRenderModes(RenderMode::On, RenderMode::On, RenderMode::On, BlendMode::NoBlending);

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
	for (int x = 0; x < World::ChunkSize; x++)
	{
		// Loop through y dimension
		for (int y = 0; y < World::ChunkSize; y++)
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
	for (int x = 0; x < World::ChunkSize; x++)
	{
		// Loop through y dimension
		for (int y = 0; y < World::ChunkSize; y++)
		{
			// Loop through z dimension
			for (int z = 0; z < World::ChunkSize; z++)
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
bool Chunk::Frame()
{
	//float distanceAway;

	//// Check distance to player
	//distanceAway = Distance(Camera::Instance()->GetTransform()->GetPosition(), Position_);
	//if (distanceAway > 4 * World::ChunkSize)
	//{
	//	IsVisible_ = false;
	//}
	//else
	//{
	//	IsVisible_ = true;
	//}

	// Set IsActive based on that
	// We need Neighbours for chunks as well

	return true;
}

bool Chunk::Render()
{
	if (!IsVisible_)
	{
		return true;
	}
	
	// Render Block
	Result_ = ChunkBlock_->Render();
	if (!Result_)
	{
		return false;
	}

	return true;
}

void Chunk::SetBlocks(string blockName)
{
	// Loop through x dimension
	for (int x = 0; x < World::ChunkSize; x++)
	{
		// Loop through y dimension
		for (int y = 0; y < World::ChunkSize; y++)
		{
			// Loop through z dimension
			for (int z = 0; z < World::ChunkSize; z++)
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
	Blocks_ = new Block**[World::ChunkSize];

	// Loop through x dimension
	for(int x = 0; x < World::ChunkSize; x++)
	{
		Blocks_[x] = new Block*[World::ChunkSize];

		// Loop through y dimension
		for (int y = 0; y < World::ChunkSize; y++)
		{
			Blocks_[x][y] = new Block[World::ChunkSize];

			// Loop through z dimension
			for (int z = 0; z < World::ChunkSize; z++)
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
	for (int x = 0; x < World::ChunkSize; x++)
	{
		// Loop through y dimension
		for (int y = 0; y < World::ChunkSize; y++)
		{
			// Loop through z dimension
			for (int z = 0; z < World::ChunkSize; z++)
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