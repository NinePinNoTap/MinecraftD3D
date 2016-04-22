#pragma once

#include <map>
#include <thread>

#include "Block.h"
#include "BlockManager.h"
#include "Singleton.h"
#include "Utilities.h"
#include "ChunkColumn.h"
#include "Player.h"

using namespace std;
using namespace Config;

typedef std::map<string, ChunkColumn*>::iterator it_wc;

class VoxelWorld : public Singleton<VoxelWorld>
{
public:
	VoxelWorld();
	~VoxelWorld();

	void Initialise();

	bool Frame();
	bool Render();

	void SetBlock(int x, int y, int z, string blockName);
	Block* GetBlock(int x, int y, int z);

private:
	void BuildChunksInBuildList();

	void HandleChunks();
	void GenerateLocalChunks();

	// Blocks and Chunks
	BlockManager* BlockManager_;
	map<string, ChunkColumn*> Map_;

	// Updating
	D3DXVECTOR3 LastChunkPosition_;

	// Building
	vector<D3DXVECTOR3> BuildList_;
	thread BuildThread_;

	vector<D3DXVECTOR3> BuildOrder_;

	Player* Player_;

	bool Result_;
};

