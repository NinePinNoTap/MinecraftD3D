#include "VoxelTerrain.h"


float stoneBaseHeight = -24;
float stoneBaseNoise = 0.05f;
float stoneBaseNoiseHeight = 4;

float stoneMountainHeight = 48;
float stoneMountainFrequency = 0.008f;
float stoneMinHeight = -12;

float dirtBaseHeight = 1;
float dirtNoise = 0.04f;
float dirtNoiseHeight = 3;


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

#include "Noise.h"
using namespace SimplexNoise;

static int GetNoise(int x, int y, int z, float scale, int max)
{
	return floor ((Noise::Generate(x * scale, y * scale, z * scale) + 1.0f) * (max / 2.0f));
}

void VoxelTerrain::GenerateTerrain()
{
	PerlinNoiseGenerator perlinNoise;

	// Generate a random seed for the noise to use
	perlinNoise.SetSeed(rand() % 10000);

	int workArea = TERRAIN_HEIGHT - TERRAIN_BASE_HEIGHT;

	for (int x = 0; x < TERRAIN_WIDTH; x++)
	{
		for (int z = 0; z < TERRAIN_DEPTH; z++)
		{
			int stoneHeight = floor(stoneBaseHeight);
			stoneHeight += TERRAIN_BASE_HEIGHT;
			stoneHeight += GetNoise(x, 0, z, stoneMountainFrequency, floor(stoneMountainHeight));

			if (stoneHeight < stoneMinHeight)
				stoneHeight = floor(stoneMinHeight);

			stoneHeight += GetNoise(x, 0, z, stoneBaseNoise, floor(stoneBaseNoiseHeight));

			int dirtHeight = stoneHeight + floor(dirtBaseHeight);
			dirtHeight += GetNoise(x, 100, z, dirtNoise, floor(dirtNoiseHeight));

			for (int y = 0; y < TERRAIN_HEIGHT; y++)
			{
				if (y <= stoneHeight)
				{
					GetBlock(x, y, z)->CopyFrom(BlockManager::Instance()->GetBlock("stone"));
				}
				else if (y <= dirtHeight)
				{
					GetBlock(x, y, z)->CopyFrom(BlockManager::Instance()->GetBlock("dirt"));
				}
				else
				{
					GetBlock(x, y, z)->CopyFrom(BlockManager::Instance()->GetBlock("air"));
				}

			}


			//double a = (double)z / (double)TERRAIN_WIDTH * 2;
			//double b = (double)x / (double)TERRAIN_DEPTH * 2;

			//float noise = perlinNoise.CreateNoise(a, b, 0.8f);
			//float height = floor(workArea * noise);
			//height += TERRAIN_BASE_HEIGHT;

			//// Loop through y dimension
			//for (int y = height; y >= 0; y--)
			//{
			//	if (y < 10)
			//	{
			//		int r = rand() % 50;
			//		if (r < 3)
			//		{
			//			GetBlock(x, y, z)->CopyFrom(BlockManager::Instance()->GetBlock("diamond"));
			//		}
			//		else
			//		{
			//			GetBlock(x, y, z)->CopyFrom(BlockManager::Instance()->GetBlock("sand"));
			//		}
			//	}
			//	else if (y < 32)
			//	{
			//		GetBlock(x, y, z)->CopyFrom(BlockManager::Instance()->GetBlock("stone"));
			//	}
			//	else if (y < 40)
			//	{
			//		GetBlock(x, y, z)->CopyFrom(BlockManager::Instance()->GetBlock("sand"));
			//	}
			//	else
			//	{
			//		GetBlock(x, y, z)->CopyFrom(BlockManager::Instance()->GetBlock("dirt"));
			//	}
			//}
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