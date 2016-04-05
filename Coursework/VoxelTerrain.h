#pragma once

#include "Chunk.h"
#include "Constants.h"
#include "BlockManager.h"

class VoxelTerrain
{
public:
	VoxelTerrain();
	~VoxelTerrain();

	void Initialise();

	void Frame();
	void Render();

private:
	void CreateChunks();

	void GenerateTerrain();
	void GenerateChunkTerrain(Chunk* chunk);

	void LinkBlocks();
	void LinkBlocksInChunk(Chunk* chunk);
	void SetBlockNeighbours(Chunk* chunk, int x, int y, int z);
	Block* GetBlock(D3DXVECTOR3 currentChunkID, int x, int y, int z);

	BlockManager* BlockManager_;
	Chunk**** TerrainChunks_;
};