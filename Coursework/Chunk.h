#pragma once

#include <string>

#include "Block.h"
#include "BlockManager.h"
#include "Constants.h"
#include "InstancedGameObject.h"
#include "ShaderManager.h"

class Chunk
{
public:
	Chunk();
	~Chunk();

	// Initiailise
	void Initialise(int x, int y, int z);
	void Generate();

	// Shutdown
	void Shutdown();

	// Chunk Management
	void RefreshVisible();

	// Frame
	void Frame();
	void Render();

	// Setter
	void SetBlocks(string blockName);

	// Getters
	inline bool IsVisible()
	{
		return IsVisible_;
	}
	inline D3DXVECTOR3 GetPosition()
	{
		return Position_;
	}
	inline Block* GetBlock(int x, int y, int z)
	{
		// Make sure we are trying to access a valid node
		if (RangeCheck(x, 0, CHUNK_BLOCKS) && RangeCheck(y, 0, CHUNK_BLOCKS) && RangeCheck(z, 0, CHUNK_BLOCKS))
		{
			return &Blocks_[x][y][z];
		}

		return 0;
	}

private:
	void GenerateBlankChunk();
	void GenerateChunk();

	D3DXVECTOR3 Position_;
	InstancedGameObject* ChunkBlock_;

	Block*** Blocks_;

	bool IsVisible_;
	bool IsGenerated_;
};