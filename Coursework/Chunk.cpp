#include "Chunk.h"
#include "ViewFrustumManager.h"

Chunk::Chunk()
{
	Blocks_ = 0;
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

	ChunkBlock_.Initialise("block.obj");
	ChunkBlock_.SetShader("instancedlight");
	ChunkBlock_.SetRenderModes(RenderMode::On, RenderMode::On, RenderMode::On, BlendMode::NoBlending);

	//================
	// Generate Chunk
	//================

	GenerateBlankChunk();

	//==================
	// Initialise Flags
	//==================

	IsVisible_ = true;
	IsGenerated_ = false;
	IsDirty_ = false;
}

void Chunk::Shutdown()
{
	//// Delete the blocks
	//for (int x = 0; x < World::ChunkSize; x++)
	//{
	//	// Loop through y dimension
	//	for (int y = 0; y < World::ChunkSize; y++)
	//	{
	//		delete[] Blocks_[x][y];
	//	}

	//	delete[] Blocks_[x];
	//}

	//delete[] Blocks_;

	ChunkBlock_.Shutdown();
}

// Frame
bool Chunk::Frame()
{
	float distanceAway;
	
	//==============
	// View Frustum
	//==============

	IsVisible_ = ViewFrustumManager::Instance()->CheckCube(Position_ + World::ChunkOffset, World::ChunkSize / 2);
	if (!IsVisible_)
	{
		return true;
	}

	//=================
	// Distance Checks
	//=================

	// Check distance to player
	distanceAway = Distance(Camera::Instance()->GetTransform()->GetPosition(), Position_);

	// Check if we are out of range
	if (distanceAway >= World::UnloadRadius * World::ChunkSize)
	{
		IsVisible_ = false;
		OutOfRange_ = true;
	}
	else
	{
		IsVisible_ = true;
		OutOfRange_ = false;
	}

	return true;
}

bool Chunk::Render()
{
	if (!IsVisible_)
	{
		return true;
	}
	
	// Render Block
	Result_ = ChunkBlock_.Render();
	if (!Result_)
	{
		return false;
	}

	return true;
}

// Chunk Generating
void Chunk::GenerateBlankChunk()
{
	float xPos, yPos, zPos;

	// Create the 3D array to store blocks in chunk
	Blocks_ = new Block**[World::ChunkSize];

	// Loop through x dimension
	for (int x = 0; x < World::ChunkSize; x++)
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
				Blocks_[x][y][z].CopyFrom(World::Blocks["air"]);
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

void Chunk::RefreshVisible()
{
	D3DXVECTOR3 worldPos;

	// Loop through x dimension
	for (int x = 0; x < World::ChunkSize; x++)
	{
		// Loop through y dimension
		for (int y = 0; y < World::ChunkSize; y++)
		{
			// Loop through z dimension
			for (int z = 0; z < World::ChunkSize; z++)
			{
				worldPos = Position_ + D3DXVECTOR3(x, y, z);

				// Set block neighbours
				/*Blocks_[x][y][z].SetNeighbour(Direction::Up, GetBlock(worldPos.x, worldPos.y + 1, worldPos.z));
				Blocks_[x][y][z].SetNeighbour(Direction::Down, GetBlock(worldPos.x, worldPos.y - 1, worldPos.z));

				Blocks_[x][y][z].SetNeighbour(Direction::Left, GetBlock(worldPos.x - 1, worldPos.y, worldPos.z));
				Blocks_[x][y][z].SetNeighbour(Direction::Right, GetBlock(worldPos.x + 1, worldPos.y, worldPos.z));

				Blocks_[x][y][z].SetNeighbour(Direction::Forward, GetBlock(worldPos.x, worldPos.y, worldPos.z + 1));
				Blocks_[x][y][z].SetNeighbour(Direction::Backward, GetBlock(worldPos.x, worldPos.y, worldPos.z - 1));*/

				// Refresh the block visibility
				Blocks_[x][y][z].Refresh();

				if (Blocks_[x][y][z].IsActive() && Blocks_[x][y][z].IsSolid())
				{
					ChunkBlock_.AddInstance(Blocks_[x][y][z].GetInstance());
				}
			}
		}
	}

	// Compile the instance data to buffer
	ChunkBlock_.RebuildInstanceBuffer();
}

// Setters
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
				Blocks_[x][y][z].CopyFrom(World::Blocks[blockName]);
			}
		}
	}
}

// Getters
inline Block* Chunk::GetBlock(int x, int y, int z)
{
	// Check the value is within range
	if (!RangeCheck(x, 0, World::ChunkSize - 1))
	{
		return 0;
	}

	if (!RangeCheck(y, 0, World::ChunkSize - 1))
	{
		return 0;
	}
	if (!RangeCheck(z, 0, World::ChunkSize - 1))
	{
		return 0;
	}

	// Return block
	return &Blocks_[x][y][z];
}

inline void Chunk::SetBlock(int x, int y, int z, Block block)
{
	Block* targetBlock;

	// Get the block
	targetBlock = GetBlock(x, y, z);
	if (targetBlock)
	{
		// Update the block
		targetBlock = &block;

		// Flag we need a refresh
		IsDirty_ = true;
	}

	// Clean Up
	targetBlock = 0;
}