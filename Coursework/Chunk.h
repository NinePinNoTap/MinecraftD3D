#pragma once

#include <string>

#include "Block.h"
#include "InstancedGameObject.h"
#include "ShaderManager.h"
#include "SimplexNoise.h"

using namespace Config;

class Chunk
{
public:
	Chunk();
	~Chunk();

	// Initiailise
	void initialise(int x, int y, int z);

	// terminate
	void terminate();

	// Chunk Management
	void refreshVisible();

	// update
	bool update();
	bool render();

	// setter
	void setBlock(int x, int y, int z, Block block);
	void setBlocks(string blockName);
	inline void setDirty(bool flag) { m_dirty = flag; }

	// getters
	inline bool isVisible() { return m_visible; }
	inline bool isDirty() { return m_dirty; }
	inline bool isOutOfRange() { return m_outOfRange; }
	inline D3DXVECTOR3 getPosition() { return m_position; }
	Block* getBlock(int x, int y, int z);

private:
	void generateBlankChunk();

	D3DXVECTOR3 m_position;
	InstancedGameObject m_chunkBlock;

	Block*** m_blocks;

	bool m_visible;
	bool m_dirty;
	bool m_generated;
	bool m_result;
	bool m_outOfRange;
};