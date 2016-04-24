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

class ChunkThread;

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

	void BuildChunk(D3DXVECTOR3 chunkIndex);

private:
	void HandleThreads();
	void HandleActiveThreads();
	void HandleBuildList();

	void HandleChunks();
	void GenerateLocalChunks();

	// Blocks and Chunks
	BlockManager* BlockManager_;
	map<string, ChunkColumn*> Map_;

	// Updating
	D3DXVECTOR3 LastChunkPosition_;

	// Building
	vector<D3DXVECTOR3> LocalChunks_;
	vector<D3DXVECTOR3> BuildList_;
	vector<ChunkThread*> ChunkThreads_;
	thread ThreadHandler_;
	int MaxThreads_;

	// World Player
	Player* Player_;

	// Misc
	bool Result_;
};

class ChunkThread
{
public:
	ChunkThread(D3DXVECTOR3 buildTarget)
	{
		VoxelWorld* vWorld = VoxelWorld::Instance();

		// Create a thread
		workingThread = std::thread([this, buildTarget, vWorld]()
		{
			isFinished = false;

			vWorld->BuildChunk(buildTarget);

			isFinished = true;
		});
	}
	~ChunkThread()
	{
		
	}
	void Join()
	{
		if (workingThread.joinable())
		{
			workingThread.join();
		}
	}
	bool isFinished;

private:
	std::thread workingThread;
};