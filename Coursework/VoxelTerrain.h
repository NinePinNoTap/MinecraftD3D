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
	void RefreshTerrain();

	void LinkBlocks();
	void SetBlockNeighbours(int x, int y, int z);
	Block* GetBlock(int x, int y, int z);

	BlockManager* BlockManager_;
	Chunk**** TerrainChunks_;
};