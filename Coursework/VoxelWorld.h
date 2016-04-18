#pragma once

#include <map>
#include <thread>

#include "Block.h"
#include "BlockManager.h"
#include "Singleton.h"
#include "Utilities.h"
#include "WorldChunk.h"

using namespace std;

typedef std::map<string, WorldChunk*>::iterator it_wc;

static D3DXVECTOR2 ChunkLocalOffsets[9] {
	{  0,  0 },				// Centre
	{ -1,  0 }, { 1,  0 }, // Left / Right
	{  0,  1 }, { 0, -1 }, // Forward / Backward
	{ -1,  1 }, { 1,  1 }, // Forward Left / Right
	{ -1, -1 }, { 1, -1 },	// Backward Left / Right
};

class VoxelWorld : public Singleton<VoxelWorld>
{
public:
	VoxelWorld();
	~VoxelWorld();

	void Initialise();

	void Frame();
	void Render();

	void SetBlock(int x, int y, int z, string blockName);
	Block* GetBlock(int x, int y, int z);

private:
	void AddChunkToBuildList(D3DXVECTOR2 chunkIndex);
	void BuildChunksInBuildList();

	void HandleChunks();
	void GenerateLocalChunks();

	BlockManager* BlockManager_;
	map<string, WorldChunk*> Map_;

	// Updating
	D3DXVECTOR2 LastWorldChunk_;

	// Building
	vector<D3DXVECTOR2> BuildList_;
	thread BuildThread_;
};

