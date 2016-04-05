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
	// Create a Block Manager
	BlockManager_ = new BlockManager;

	//=================
	// Generate Chunks
	//=================

	CreateChunks();
	LinkBlocks();
	GenerateTerrain();
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

	for (int x = 0; x < NO_OF_CHUNKS_WIDTH; x++)
	{
		for (int y = 0; y < NO_OF_CHUNKS_HEIGHT; y++)
		{
			for (int z = 0; z < NO_OF_CHUNKS_DEPTH; z++)
			{
				LinkBlocksInChunk(TerrainChunks_[x][y][z]);
			}
		}
	}
}

void VoxelTerrain::LinkBlocksInChunk(Chunk* chunk)
{
	//===========================
	// Set the blocks neighbours
	//===========================

	for (int x = 0; x < NO_OF_BLOCKS_WIDTH; x++)
	{
		for (int y = 0; y < NO_OF_BLOCKS_HEIGHT; y++)
		{
			for (int z = 0; z < NO_OF_BLOCKS_DEPTH; z++)
			{
				SetBlockNeighbours(chunk, x, y, z);
			}
		}
	}
}

void VoxelTerrain::SetBlockNeighbours(Chunk* chunk, int x, int y, int z)
{
	//===========================
	// Apply neighbours to block
	//===========================

	Block* blockCurrent;

	// Get current block
	blockCurrent = GetBlock(chunk->GetChunkID(), x, y, z);

	// Set block neighbours
	blockCurrent->SetNeighbour(Direction::Up, GetBlock(chunk->GetChunkID(), x, y + 1, z));
	blockCurrent->SetNeighbour(Direction::Down, GetBlock(chunk->GetChunkID(), x, y - 1, z));

	blockCurrent->SetNeighbour(Direction::Left, GetBlock(chunk->GetChunkID(), x - 1, y, z));
	blockCurrent->SetNeighbour(Direction::Right, GetBlock(chunk->GetChunkID(), x + 1, y, z));

	blockCurrent->SetNeighbour(Direction::Forward, GetBlock(chunk->GetChunkID(), x, y, z + 1));
	blockCurrent->SetNeighbour(Direction::Backward, GetBlock(chunk->GetChunkID(), x, y, z - 1));
}

void VoxelTerrain::GenerateTerrain()
{
	for (int x = 0; x < NO_OF_CHUNKS_WIDTH; x++)
	{
		for (int y = 0; y < NO_OF_CHUNKS_HEIGHT; y++)
		{
			for (int z = 0; z < NO_OF_CHUNKS_DEPTH; z++)
			{
				GenerateChunkTerrain(TerrainChunks_[x][y][z]);
			}
		}
	}
}

void VoxelTerrain::GenerateChunkTerrain(Chunk* chunk)
{
	PerlinNoiseGenerator perlinNoise;

	// Generate a random seed for the noise to use
	perlinNoise.SetSeed(rand() % 10000);
	
	// Loop through x dimension
	for (int i = 0; i < NO_OF_BLOCKS_WIDTH; i++)
	{
		// Loop through z dimension
		for (int k = 0; k < NO_OF_BLOCKS_DEPTH; k++)
		{
			double x = (double)k / (double)NO_OF_BLOCKS_WIDTH;
			double y = (double)i / (double)NO_OF_BLOCKS_DEPTH;

			float noise = perlinNoise.CreateNoise(x, y, 0.8f);
			float height = floor(NO_OF_BLOCKS_HEIGHT * noise);

			// Loop through y dimension
			for (int j = 0; j < height; j++)
			{
				if (j < 1)
				{
					chunk->GetBlock(i, j, k)->SetName("cobblestone");
					chunk->GetBlock(i, j, k)->SetType(BlockType::Cobblestone);
				}
				else if (j < 3)
				{
					chunk->GetBlock(i, j, k)->SetName("dirt");
					chunk->GetBlock(i, j, k)->SetType(BlockType::Dirt);
				}
				else
				{
					chunk->GetBlock(i, j, k)->SetName("sand");
					chunk->GetBlock(i, j, k)->SetType(BlockType::Sand);
				}

				chunk->GetBlock(i, j, k)->SetSolid(true);
				chunk->GetBlock(i, j, k)->SetActive(true);
			}
		}
	}
}

Block* VoxelTerrain::GetBlock(D3DXVECTOR3 currentChunkID, int x, int y, int z)
{
	int chunkX = currentChunkID.x;
	int chunkY = currentChunkID.y;
	int chunkZ = currentChunkID.z;

	//=============================================================
	// Check if the block we are trying to get is in another chunk
	//=============================================================

	// Check x
	if (!RangeCheck(x, 0, NO_OF_BLOCKS_WIDTH - 1))
	{
		if (x < 0)
			chunkX--;
		else
			chunkX++;

		Wrap(x, 0, NO_OF_BLOCKS_WIDTH - 2);
	}

	// Check y
	if (!RangeCheck(y, 0, NO_OF_BLOCKS_HEIGHT - 1))
	{
		if (y < 0)
			chunkY--;
		else
			chunkY++;

		Wrap(y, 0, NO_OF_BLOCKS_HEIGHT - 2);
	}

	// Check z
	if (!RangeCheck(z, 0, NO_OF_BLOCKS_DEPTH - 1))
	{
		if (z < 0)
			chunkZ--;
		else
			chunkZ++;

		Wrap(z, 0, NO_OF_BLOCKS_DEPTH - 2);
	}

	//=================================
	// Make sure the chunk ID is valid
	//=================================

	if (RangeCheck(chunkX, 0, NO_OF_CHUNKS_WIDTH - 1) &&
		RangeCheck(chunkY, 0, NO_OF_CHUNKS_HEIGHT - 1) &&
		RangeCheck(chunkZ, 0, NO_OF_CHUNKS_DEPTH - 1))
	{
		return TerrainChunks_[chunkX][chunkY][chunkZ]->GetBlock(x, y, z);
	}

	return 0;
}

Block* VoxelTerrain::GetBlock(int x, int y, int z)
{
	D3DXVECTOR3 chunkID;
	int coordX, coordY, coordZ;

	//====================================
	// Check if the index is out of range
	//====================================

	if (!RangeCheck(x, 0, TERRAIN_WIDTH - 1))
		return 0;
	if (!RangeCheck(y, 0, TERRAIN_HEIGHT - 1))
		return 0;
	if (!RangeCheck(z, 0, TERRAIN_DEPTH - 1))
		return 0;

	//===========================
	// Convert global coordinate
	//===========================

	coordX = x%NO_OF_BLOCKS_WIDTH;
	coordY = y%NO_OF_BLOCKS_HEIGHT;
	coordZ = z%NO_OF_BLOCKS_DEPTH;

	chunkID.x = (x - coordX) / NO_OF_BLOCKS_WIDTH;
	chunkID.y = (y - coordY) / NO_OF_BLOCKS_HEIGHT;
	chunkID.z = (z - coordZ) / NO_OF_BLOCKS_DEPTH;

	return GetBlock(chunkID, coordX, coordY, coordZ);
}