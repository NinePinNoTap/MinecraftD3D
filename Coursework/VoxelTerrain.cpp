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

	//=================
	// Generate Chunks
	//=================

	CreateChunks();
	LinkBlocks();
	GenerateTerrain();
	RefreshTerrain();

	OutputToDebug("Terrain generated");
}

void VoxelTerrain::Frame()
{
	// Look through terrain
	typedef std::map<std::string, Chunk>::iterator it_type;
	for (it_type iterator = ChunkMap_.begin(); iterator != ChunkMap_.end(); iterator++)
	{
		// Update chunk
		iterator->second.Frame();
	}
}

void VoxelTerrain::Render()
{
	// Loop through terrain and render active chunks
	typedef std::map<std::string, Chunk>::iterator it_type;
	for (it_type iterator = ChunkMap_.begin(); iterator != ChunkMap_.end(); iterator++)
	{
		// Check if the chunk is active
		if (iterator->second.IsVisible())
		{
			// Render it
			iterator->second.Render();
		}
	}
}

void VoxelTerrain::CreateChunks()
{
	//====================================
	// Create 3D array to represent world
	//====================================

	for (int x = 0; x < 2; x++)
	{
		for (int y = 0; y < 2; y++)
		{
			for (int z = 0; z < 2; z++)
			{
				Chunk chunk;
				chunk.Initialise(x, y, z);
				chunk.SetBlocks("air");

				ChunkMap_[GetKey(x, y, z)] = chunk;
			}
		}
	}
}

void VoxelTerrain::LinkBlocks()
{
	//===============================================
	// Link each block to its surrounding neighbours
	//===============================================

	for (int x = 0; x < TERRAIN_WIDTH; x++)
	{
		for (int y = 0; y < TERRAIN_HEIGHT; y++)
		{
			for (int z = 0; z < TERRAIN_DEPTH; z++)
			{
				SetBlockNeighbours(x, y, z);
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

	// Set block neighbours
	blockCurrent->SetNeighbour(Direction::Up, GetBlock(x, y + 1, z));
	blockCurrent->SetNeighbour(Direction::Down, GetBlock(x, y - 1, z));

	blockCurrent->SetNeighbour(Direction::Left, GetBlock(x - 1, y, z));
	blockCurrent->SetNeighbour(Direction::Right, GetBlock(x + 1, y, z));

	blockCurrent->SetNeighbour(Direction::Forward, GetBlock(x, y, z + 1));
	blockCurrent->SetNeighbour(Direction::Backward, GetBlock(x, y, z - 1));
}

void VoxelTerrain::GenerateTerrain()
{
	PerlinNoiseGenerator perlinNoise;

	// Generate a random seed for the noise to use
	perlinNoise.SetSeed(rand() % 10000 + rand() % 10000);

	int workArea = TERRAIN_HEIGHT - TERRAIN_BASE_HEIGHT;

	for (int x = 0; x < TERRAIN_WIDTH; x++)
	{
		for (int z = 0; z < TERRAIN_DEPTH; z++)
		{
			double a = (double)z / (double)TERRAIN_WIDTH * 2;
			double b = (double)x / (double)TERRAIN_DEPTH * 2;

			float noise = perlinNoise.CreateNoise(a, b, 0.8f);
			float height = floor(workArea * noise);
			height += TERRAIN_BASE_HEIGHT;

			// Loop through y dimension
			for (int y = 0; y < height; y++)
			{
				if (y < 8)
				{
					int r = rand() % 50;
					if (r < 3)
					{
						GetBlock(x, y, z)->CopyFrom(BlockManager::Instance()->GetBlock("diamond"));
					}
					else
					{
						GetBlock(x, y, z)->CopyFrom(BlockManager::Instance()->GetBlock("sand"));
					}
				}
				else if (y < 12)
				{
					GetBlock(x, y, z)->CopyFrom(BlockManager::Instance()->GetBlock("stone"));
				}
				else if (y < 16)
				{
					GetBlock(x, y, z)->CopyFrom(BlockManager::Instance()->GetBlock("sand"));
				}
				else
				{
					GetBlock(x, y, z)->CopyFrom(BlockManager::Instance()->GetBlock("dirt"));
				}
			}
		}
	}
}

void VoxelTerrain::RefreshTerrain()
{
	typedef std::map<std::string, Chunk>::iterator it_type;
	for (it_type iterator = ChunkMap_.begin(); iterator != ChunkMap_.end(); iterator++)
	{
		iterator->second.Build();
		// iterator->first = key
		// iterator->second = value
		// Repeat if you also want to iterate through the second map.
	}
}

Block* VoxelTerrain::GetBlock(int x, int y, int z)
{
	int chunkX, chunkY, chunkZ;
	int coordX, coordY, coordZ;

	//===================
	// Check Index Range
	//===================

	if (!RangeCheck(x, 0, TERRAIN_WIDTH - 1))
		return 0;
	if (!RangeCheck(y, 0, TERRAIN_HEIGHT - 1))
		return 0;
	if (!RangeCheck(z, 0, TERRAIN_DEPTH - 1))
		return 0;

	//=================================
	// Calculate Chunk and Local Coord
	//=================================

	coordX = x % NO_OF_BLOCKS_WIDTH;
	coordY = y % NO_OF_BLOCKS_HEIGHT;
	coordZ = z % NO_OF_BLOCKS_DEPTH;

	chunkX = (x - coordX) / NO_OF_BLOCKS_WIDTH;
	chunkY = (y - coordY) / NO_OF_BLOCKS_HEIGHT;
	chunkZ = (z - coordZ) / NO_OF_BLOCKS_DEPTH;
		
	// Create a lookup key
	string chunkKey = GetKey(chunkX, chunkY, chunkZ);

	if (ChunkMap_.count(chunkKey))
	{
		return ChunkMap_[chunkKey].GetBlock(coordX, coordY, coordZ);
	}

	return 0;
}