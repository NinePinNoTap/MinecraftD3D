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

private:
	void HandleThreads();
	void HandleActiveThreads();
	void HandleBuildList();

	void BuildChunk(D3DXVECTOR3 chunkIndex);

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

struct ChunkThread
{
	ChunkThread()
	{
	}

	~ChunkThread()
	{
		TryJoin();
	}

	void SetFunction(std::function<void()> function)
	{
		workerFunction = function;
	}

	void Start()
	{
		isFinished = false;

		// Create a thread
		workerThread = std::thread([this]()
		{
			// Call the function
			workerFunction();

			// Flag its complete
			isFinished = true;
		});
	}

	bool TryJoin()
	{
		if (isFinished)
		{
			if (workerThread.joinable())
			{
				workerThread.join();
				return true;
			}
		}

		return false;
	}

	std::function<void()> workerFunction;
	std::thread workerThread;

	bool isFinished;
};