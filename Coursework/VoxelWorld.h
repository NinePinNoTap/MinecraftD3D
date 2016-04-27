#pragma once

#include <map>
#include <thread>

#include "Block.h"
#include "Singleton.h"
#include "Utilities.h"
#include "Player.h"
#include "ManagedThread.h"
#include "ChunkGenerator.h"

using namespace std;
using namespace Config;

typedef std::map<string, Chunk*>::iterator it_wc;

enum WorkerType { Build, Update, Unload };

struct ChunkTarget
{
	ChunkTarget(D3DXVECTOR3 target, WorkerType type)
	{
		chunkTarget = target;
		workerType = type;
	}
	D3DXVECTOR3 chunkTarget;
	WorkerType workerType;
};

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

	inline float GetBuildTime() { return (int)(AverageLoadTime_ / (float)BuildCount_) / 1000.0f; }

private:
	void HandleThreads();
	void HandleActiveThreads();
	void HandleBuildList();

	void BuildChunk(D3DXVECTOR3 chunkIndex);

	void HandleChunks();
	void GenerateLocalChunks();

	// Blocks and Chunks
	map<string, Chunk*> Map_;
	ChunkGenerator* ChunkGenerator_;

	// Updating
	D3DXVECTOR3 LastChunkPosition_;
	vector<D3DXVECTOR3> UpdateList_;

	// Building
	vector<D3DXVECTOR3> LocalChunks_;
	vector<ChunkTarget> ChunkQueue_;
	vector<ManagedThread<void()>*> ManagedThreads_;
	ManagedThread<void()>* ThreadHandler_;
	int MaxThreads_;

	// World Player
	Player* Player_;

	// Output
	float AverageLoadTime_;
	int BuildCount_;

	// Misc
	bool Result_;
};