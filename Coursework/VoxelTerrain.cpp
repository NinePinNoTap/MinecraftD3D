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
	BlockLoader_ = new BlockLoader;

	//
	// Build Terrain
	//

	// Create the 3D array to store blocks in chunk
	TerrainChunks_ = new Chunk***[CHUNK_COUNT];

	// Loop through x dimension
	for (int i = 0; i < CHUNK_COUNT; i++)
	{
		TerrainChunks_[i] = new Chunk**[CHUNK_COUNT];

		// Loop through y dimension
		for (int j = 0; j < CHUNK_COUNT; j++)
		{
			TerrainChunks_[i][j] = new Chunk*[CHUNK_COUNT];

			// Loop through z dimension
			for (int k = 0; k < CHUNK_COUNT; k++)
			{
				Chunk* chunk = new Chunk;
				chunk->Initialise();
				chunk->GetTransform()->SetPosition(CHUNK_TOTAL_WIDTH * i, CHUNK_TOTAL_HEIGHT * j, CHUNK_TOTAL_DEPTH * k);
				TerrainChunks_[i][j][k] = chunk;
			}
		}
	}
}

void VoxelTerrain::Render()
{
	// Loop through x dimension
	for (int i = 0; i < CHUNK_COUNT; i++)
	{
		// Loop through y dimension
		for (int j = 0; j < CHUNK_COUNT; j++)
		{
			// Loop through z dimension
			for (int k = 0; k < CHUNK_COUNT; k++)
			{
				//if (TerrainChunks_[i][j][k])
				TerrainChunks_[i][j][k]->Render();
			}
		}
	}
}
