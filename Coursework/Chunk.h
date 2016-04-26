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
	void Initialise(int x, int y, int z);
	void Generate();

	// Shutdown
	void Shutdown();

	// Chunk Management
	void RefreshVisible();

	// Frame
	bool Frame();
	bool Render();

	// Setter
	void SetBlocks(string blockName);
	inline void SetDirty(bool flag) { IsDirty_ = flag; }

	// Getters
	inline bool IsVisible()
	{
		return IsVisible_;
	}
	inline bool IsDirty() { return IsDirty_; }
	inline D3DXVECTOR3 GetPosition()
	{
		return Position_;
	}
	inline Block* GetBlock(int x, int y, int z)
	{
		// Check the value is within range
		if (!RangeCheck(x, 0, World::ChunkSize - 1))
		{
			return 0;
		}

		if (!RangeCheck(y, 0, World::ChunkSize - 1))
		{
			return 0;
		}
		if (!RangeCheck(z, 0, World::ChunkSize - 1))
		{
			return 0;
		}

		// Return block
		return &Blocks_[x][y][z];
	}

private:
	void GenerateBlankChunk();

	D3DXVECTOR3 Position_;
	InstancedGameObject* ChunkBlock_;

	Block*** Blocks_;

	bool IsVisible_;
	bool IsDirty_;
	bool IsGenerated_;
	bool Result_;
};