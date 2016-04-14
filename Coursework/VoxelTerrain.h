#pragma once

#include "Chunk.h"
#include "Constants.h"
#include "BlockManager.h"

#include <map>
#include <thread>

typedef std::map<std::string, Chunk*>::iterator it_type;

static D3DXVECTOR3 DIRECTION_OFFSETS[9] = {
	{ 0, 0, 0 }, // Centre
	{ -1, 0, 0 }, { 1, 0, 0 }, // Left / Right
	{ 0, 0, 1 }, { 0, 0, -1 }, // Forward / Backward
	{ -1, 0, 1 }, { 1, 0, 1 }, // Forward Left / Right
	{ -1, 0, -1 }, { 1, 0, -1 }, // Backward Left / Right
};

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
	void TryBuildChunk(D3DXVECTOR3 index);
	void BuildChunks();

	void HandleChunks();
	void HandleChunkGeneration();

	void UpdateWorld();
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
	D3DXVECTOR3 LastChunkPos_;
	thread UpdateThread_;
	bool NeedsUpdating_;
};