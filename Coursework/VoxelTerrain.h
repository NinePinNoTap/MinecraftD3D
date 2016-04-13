#pragma once

#include "Chunk.h"
#include "Constants.h"
#include "BlockManager.h"

#include <map>
#include <thread>

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
	void BuildChunks();

	void LinkBlocks(Chunk* chunk);
	void SetBlockNeighbours(int x, int y, int z);

	Chunk* GetChunk(int x, int y, int z);
	Block* GetBlock(int x, int y, int z);

	BlockManager* BlockManager_;
	map<string, Chunk*> ChunkMap_;

	// Chunk Building
	vector<D3DXVECTOR3> BuildList_;
	thread BuildThread_;

	// Chunk Updating
	float UpdateDelay_;
	DWORD LastChunkCheck_;
	D3DXVECTOR3 LastChunkPos_;
};