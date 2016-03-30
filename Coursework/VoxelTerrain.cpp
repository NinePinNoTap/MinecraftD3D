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
	TerrainChunks_ = new Chunk***[NO_OF_CHUNKS];

	// Loop through x dimension
	for (int i = 0; i < NO_OF_CHUNKS; i++)
	{
		TerrainChunks_[i] = new Chunk**[NO_OF_CHUNKS];

		// Loop through y dimension
		for (int j = 0; j < NO_OF_CHUNKS; j++)
		{
			TerrainChunks_[i][j] = new Chunk*[NO_OF_CHUNKS];

			// Loop through z dimension
			for (int k = 0; k < NO_OF_CHUNKS; k++)
			{
				Chunk* chunk = new Chunk;
				chunk->Initialise();
				chunk->GetTransform()->SetPosition(CHUNK_SIZE * i, CHUNK_SIZE * j, CHUNK_SIZE * k);
				TerrainChunks_[i][j][k] = chunk;
			}
		}
	}
}

void VoxelTerrain::Render()
{
	// Loop through x dimension
	for (int i = 0; i < NO_OF_CHUNKS; i++)
	{
		// Loop through y dimension
		for (int j = 0; j < NO_OF_CHUNKS; j++)
		{
			// Loop through z dimension
			for (int k = 0; k < NO_OF_CHUNKS; k++)
			{
				//if (TerrainChunks_[i][j][k])
				TerrainChunks_[i][j][k]->Render();
			}
		}
	}
}
