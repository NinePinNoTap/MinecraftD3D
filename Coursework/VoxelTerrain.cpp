#include "VoxelTerrain.h"

VoxelTerrain::VoxelTerrain()
{
	TerrainChunks_ = 0;
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
	// Loop through x dimension
	for (int i = 0; i < NO_OF_CHUNKS_WIDTH; i++)
	{
		// Loop through y dimension
		for (int j = 0; j < NO_OF_CHUNKS_HEIGHT; j++)
		{
			// Loop through z dimension
			for (int k = 0; k < NO_OF_CHUNKS_DEPTH; k++)
			{
				TerrainChunks_[i][j][k]->Frame();
			}
		}
	}
}

void VoxelTerrain::Render()
{
	// Loop through x dimension
	for (int i = 0; i < NO_OF_CHUNKS_WIDTH; i++)
	{
		// Loop through y dimension
		for (int j = 0; j < NO_OF_CHUNKS_HEIGHT; j++)
		{
			// Loop through z dimension
			for (int k = 0; k < NO_OF_CHUNKS_DEPTH; k++)
			{	
				if (TerrainChunks_[i][j][k]->IsVisible())
				{
					TerrainChunks_[i][j][k]->Render();
				}
			}
		}
	}
}

void VoxelTerrain::CreateChunks()
{
	//====================================
	// Create 3D array to represent world
	//====================================

	// Create x dimension
	TerrainChunks_ = new Chunk***[NO_OF_CHUNKS_WIDTH];

	for (int x = 0; x < NO_OF_CHUNKS_WIDTH; x++)
	{
		// Create y dimension
		TerrainChunks_[x] = new Chunk**[NO_OF_CHUNKS_HEIGHT];

		for (int y = 0; y < NO_OF_CHUNKS_HEIGHT; y++)
		{
			// Create z dimension
			TerrainChunks_[x][y] = new Chunk*[NO_OF_CHUNKS_DEPTH];

			for (int z = 0; z < NO_OF_CHUNKS_DEPTH; z++)
			{
				// Store chunk
				TerrainChunks_[x][y][z] = new Chunk;
				TerrainChunks_[x][y][z]->Initialise(x, y, z);
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
	perlinNoise.SetSeed(rand() % 10000);

	for (int x = 0; x < TERRAIN_WIDTH; x++)
	{
		for (int z = 0; z < TERRAIN_DEPTH; z++)
		{
			double a = (double)z / (double)TERRAIN_WIDTH * 2;
			double b = (double)x / (double)TERRAIN_DEPTH * 2;

			float noise = perlinNoise.CreateNoise(a, b, 0.8f);
			float height = floor(TERRAIN_HEIGHT * noise);
			// NEED A BASE HEIGHT VARIABLE HERE SO NOISE IS CREATED ON TOP OF THE GROUND LEVEL

			// Loop through y dimension
			for (int y = height; y >= 0; y--)
			{
				if (y > height - 10)
				{
					GetBlock(x, y, z)->CopyFrom(BlockManager::Instance()->GetBlock("dirt"));
				}
				else if (y > height - 32)
				{
					GetBlock(x, y, z)->CopyFrom(BlockManager::Instance()->GetBlock("sand"));
				}
				else if (y > height - 40)
				{
					GetBlock(x, y, z)->CopyFrom(BlockManager::Instance()->GetBlock("stone"));
				}
				else if (y > height - 60)
				{
					GetBlock(x, y, z)->CopyFrom(BlockManager::Instance()->GetBlock("diamond"));
				}
			}
		}
	}
}

void VoxelTerrain::RefreshTerrain()
{
	for (int x = 0; x < NO_OF_CHUNKS_WIDTH; x++)
	{
		for (int y = 0; y < NO_OF_CHUNKS_HEIGHT; y++)
		{
			for (int z = 0; z < NO_OF_CHUNKS_DEPTH; z++)
			{
				TerrainChunks_[x][y][z]->Build();
			}
		}
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

	return TerrainChunks_[chunkX][chunkY][chunkZ]->GetBlock(coordX, coordY, coordZ);
}