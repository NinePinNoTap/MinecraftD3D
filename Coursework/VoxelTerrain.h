#pragma once

#include "Chunk.h"
#include "Constants.h"
#include "BlockManager.h"

#include <map>

typedef std::map<std::string, Chunk*>::iterator it_type;
class VoxelTerrain
{
public:
	VoxelTerrain();
	~VoxelTerrain();

	// Initialising
	void Initialise();

	// Frame
	void Frame();
	void Render();

	// Setters - TODO
	void SetChunkSize(int chunkSize = 16) { } 
	void SetChunkCount(int chunkCount = 1) { }

private:
	void CreateChunks();
	void GenerateTerrain();
	void RefreshTerrain();

	void LinkBlocks();
	void SetBlockNeighbours(int x, int y, int z);

	Chunk* GetChunk(int x, int y, int z);
	Block* GetBlock(int x, int y, int z);

	BlockManager* BlockManager_;
	map<string, Chunk*> ChunkMap_;
};