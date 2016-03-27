#pragma once

#include "Chunk.h"
#include "Constants.h"
#include "BlockManager.h"

class VoxelTerrain
{
public:
	VoxelTerrain() { }
	~VoxelTerrain() { }

	void Initialise()
	{
		float ChunkSize = BLOCKS_IN_CHUNK * BLOCK_SIZE;

		BlockManager* blockManager = new BlockManager;
		GameObject* block = blockManager->GetBlock("sand");

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
					chunk->GetTransform()->SetPosition(ChunkSize * i, ChunkSize * j, ChunkSize * k);
					TerrainChunks_[i][j][k] = chunk;
				}
			}
		}
	}

	void Render()
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

private:
	Chunk**** TerrainChunks_;
};