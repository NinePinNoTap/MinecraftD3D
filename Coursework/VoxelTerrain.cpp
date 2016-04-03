#include "VoxelTerrain.h"

VoxelTerrain::VoxelTerrain()
{
	TerrainChunks_ = 0;
}

VoxelTerrain::~VoxelTerrain()
{
}

Block test = Block();

void VoxelTerrain::Initialise()
{
	// Create a Block Manager
	BlockLoader_ = new BlockLoader;

	//
	// Build Terrain
	//

	// Create the 3D array to store blocks in chunk
	TerrainChunks_ = new Chunk***[CHUNK_COUNT_WIDTH];

	// Loop through x dimension
	for (int i = 0; i < CHUNK_COUNT_WIDTH; i++)
	{
		TerrainChunks_[i] = new Chunk**[CHUNK_COUNT_HEIGHT];

		// Loop through y dimension
		for (int j = 0; j < CHUNK_COUNT_HEIGHT; j++)
		{
			TerrainChunks_[i][j] = new Chunk*[CHUNK_COUNT_DEPTH];

			// Loop through z dimension
			for (int k = 0; k < CHUNK_COUNT_DEPTH; k++)
			{
				// Create a new Chunk
				Chunk* chunk = new Chunk;

				chunk->Initialise();
				chunk->GetTransform()->SetPosition(CHUNK_TOTAL_WIDTH * i, CHUNK_TOTAL_HEIGHT * j, CHUNK_TOTAL_DEPTH * k);
				chunk->Generate();

				TerrainChunks_[i][j][k] = chunk;
			}
		}
	}

	test.Initialise();
	test.GetTransform()->SetPosition(-1, -1, -0.5);
	test.SetName("gold");
	test.SetType(BlockType::Gold);
	test.SetSolid(true);
	test.SetShader("texture");
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

	//test.Render();
}
