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
	for (int i = 0; i < CHUNK_COUNT_WIDTH; i++)
	{
		// Loop through y dimension
		for (int j = 0; j < CHUNK_COUNT_HEIGHT; j++)
		{
			// Loop through z dimension
			for (int k = 0; k < CHUNK_COUNT_DEPTH; k++)
			{
				TerrainChunks_[i][j][k]->Frame();
			}
		}
	}
}

void VoxelTerrain::Render()
{
	// Loop through x dimension
	for (int i = 0; i < CHUNK_COUNT_WIDTH; i++)
	{
		// Loop through y dimension
		for (int j = 0; j < CHUNK_COUNT_HEIGHT; j++)
		{
			// Loop through z dimension
			for (int k = 0; k < CHUNK_COUNT_DEPTH; k++)
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
	TerrainChunks_ = new Chunk***[CHUNK_COUNT_WIDTH];

	for (int x = 0; x < CHUNK_COUNT_WIDTH; x++)
	{
		// Create y dimension
		TerrainChunks_[x] = new Chunk**[CHUNK_COUNT_HEIGHT];

		for (int y = 0; y < CHUNK_COUNT_HEIGHT; y++)
		{
			// Create z dimension
			TerrainChunks_[x][y] = new Chunk*[CHUNK_COUNT_DEPTH];

			for (int z = 0; z < CHUNK_COUNT_DEPTH; z++)
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

	for (int x = 0; x < CHUNK_COUNT_WIDTH; x++)
	{
		for (int y = 0; y < CHUNK_COUNT_HEIGHT; y++)
		{
			for (int z = 0; z < CHUNK_COUNT_DEPTH; z++)
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

	for (int x = 0; x < BLOCK_COUNT_WIDTH; x++)
	{
		for (int y = 0; y < BLOCK_COUNT_HEIGHT; y++)
		{
			for (int z = 0; z < BLOCK_COUNT_DEPTH; z++)
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
	for (int x = 0; x < CHUNK_COUNT_WIDTH; x++)
	{
		for (int y = 0; y < CHUNK_COUNT_HEIGHT; y++)
		{
			for (int z = 0; z < CHUNK_COUNT_DEPTH; z++)
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
	for (int i = 0; i < BLOCK_COUNT_WIDTH; i++)
	{
		// Loop through z dimension
		for (int k = 0; k < BLOCK_COUNT_DEPTH; k++)
		{
			double x = (double)k / (double)BLOCK_COUNT_WIDTH;
			double y = (double)i / (double)BLOCK_COUNT_DEPTH;

			float noise = perlinNoise.CreateNoise(x, y, 0.8f);
			float height = floor(BLOCK_COUNT_HEIGHT * noise);

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
	if (!RangeCheck(x, 0, BLOCK_COUNT_WIDTH - 1))
	{
		if (x < 0)
			chunkX--;
		else
			chunkX++;

		Wrap(x, 0, BLOCK_COUNT_WIDTH - 2);
	}

	// Check y
	if (!RangeCheck(y, 0, BLOCK_COUNT_HEIGHT - 1))
	{
		if (y < 0)
			chunkY--;
		else
			chunkY++;

		Wrap(y, 0, BLOCK_COUNT_HEIGHT - 2);
	}

	// Check z
	if (!RangeCheck(z, 0, BLOCK_COUNT_DEPTH - 1))
	{
		if (z < 0)
			chunkZ--;
		else
			chunkZ++;

		Wrap(z, 0, BLOCK_COUNT_DEPTH - 2);
	}

	//=================================
	// Make sure the chunk ID is valid
	//=================================

	if (RangeCheck(chunkX, 0, CHUNK_COUNT_WIDTH - 1) &&
		RangeCheck(chunkY, 0, CHUNK_COUNT_HEIGHT - 1) &&
		RangeCheck(chunkZ, 0, CHUNK_COUNT_DEPTH - 1))
	{
		return TerrainChunks_[chunkX][chunkY][chunkZ]->GetBlock(x, y, z);
	}

	return 0;
}