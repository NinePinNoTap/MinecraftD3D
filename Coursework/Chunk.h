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

	// Shutdown
	void Shutdown();

	// Chunk Management
	void RefreshVisible();

	// Frame
	bool Frame();
	bool Render();

	// Setter
	void SetBlock(int x, int y, int z, Block block);
	void SetBlocks(string blockName);
	inline void SetDirty(bool flag) { IsDirty_ = flag; }

	// Getters
	inline bool IsVisible() { return IsVisible_; }
	inline bool IsDirty() { return IsDirty_; }
	inline bool IsOutOfRange() { return OutOfRange_; }
	inline D3DXVECTOR3 GetPosition() { return Position_; }
	Block* GetBlock(int x, int y, int z);

private:
	void GenerateBlankChunk();

	D3DXVECTOR3 Position_;
	InstancedGameObject* ChunkBlock_;

	Block*** Blocks_;

	bool IsVisible_;
	bool IsDirty_;
	bool IsGenerated_;
	bool Result_;
	bool OutOfRange_;
};