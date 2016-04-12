#pragma once

#include "Chunk.h"
#include "Constants.h"
#include "BlockManager.h"
#include "Text.h"

#include <map>

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

	Chunk* GetChunk(int x, int y, int z);
	Block* GetBlock(int x, int y, int z);

	BlockManager* BlockManager_;
	map<string, Chunk> ChunkMap_;

	Text *In, *Out;
};