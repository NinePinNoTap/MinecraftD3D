#pragma once

#include <map>
#include <thread>

#include "Block.h"
#include "Singleton.h"
#include "Utilities.h"
#include "ChunkColumn.h"
#include "Player.h"
#include "ManagedThread.h"

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
	void HandleThreads();
	void HandleActiveThreads();
	void HandleBuildList();

	void BuildChunk(D3DXVECTOR3 chunkIndex);

	void HandleChunks();
	void GenerateLocalChunks();

	// Blocks and Chunks
	map<string, ChunkColumn*> Map_;

	// Updating
	D3DXVECTOR3 LastChunkPosition_;
	vector<D3DXVECTOR3> UpdateList_;

	// Building
	vector<D3DXVECTOR3> LocalChunks_;
	vector<D3DXVECTOR3> BuildList_;
	vector<ManagedThread<void()>*> ManagedThreads_;
	thread ThreadHandler_;
	int MaxThreads_;

	// World Player
	Player* Player_;

	// Misc
	bool Result_;
};