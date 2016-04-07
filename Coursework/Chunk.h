#pragma once

#include <string>

#include "Block.h"
#include "BlockManager.h"
#include "Constants.h"
#include "InstancedGameObject.h"
#include "ShaderManager.h"
#include "Transform.h"

class Chunk
{
public:
	Chunk();
	~Chunk();

	void Initialise(int x, int y, int z);
	void Shutdown();

	void Build();

	// Frame
	void Frame();
	void Render();

	// Getters
	bool IsVisible();
	Transform* GetTransform();
	D3DXVECTOR3 GetChunkID()
	{
		return ChunkID_;
	}
	Block* GetBlock(int x, int y, int z)
	{
		return &Chunk_[x][y][z];
	}

private:
	void GenerateBlankChunk();
	
	Block*** Chunk_;
	Block* block;
	bool IsVisible_;
	Transform* Transform_;
	D3DXVECTOR3 ChunkID_;
	InstancedGameObject* ChunkBlock_;
};