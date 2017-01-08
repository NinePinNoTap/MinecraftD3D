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

enum WorkerType { build, update, onUnload };

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

	void initialise();

	bool update();
	bool render();

	void setBlock(int x, int y, int z, string blockName);
	Block* getBlock(int x, int y, int z);

	inline float getBuildTime() { return (int)(m_averageloadTime / (float)m_buildCount) / 1000.0f; }

private:
	void handleThreads();
	void handleActiveThreads();
	void handleBuildList();

	void buildChunk(D3DXVECTOR3 chunkIndex);

	void handleChunks();
	void generateLocalChunks();

	// Blocks and Chunks
	map<string, Chunk*> m_chunkMap;
	ChunkGenerator* m_chunkGenerator;

	// Updating
	D3DXVECTOR3 m_lastChunkPosition;
	vector<D3DXVECTOR3> m_updateList;

	// Building
	vector<D3DXVECTOR3> m_localChunks;
	vector<ChunkTarget> m_chunkQueue;
	vector<ManagedThread<void()>*> m_managedThreads;
	ManagedThread<void()>* m_threadhandler;
	int m_maxThreads;

	// World Player
	Player* m_player;

	// Output
	float m_averageloadTime;
	int m_buildCount;

	// Misc
	bool m_result;
};