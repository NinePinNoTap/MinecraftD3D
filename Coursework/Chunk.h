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

	// Initiailise
	void Initialise(int x, int y, int z);

	// Shutdown
	void Shutdown();

	// Chunk Management
	void GenerateBlankChunk();
	void Build();

	// Frame
	void Frame();
	void Render();

	// Setter
	void SetBlocks(string blockName);

	// Getters
	bool IsVisible();

	D3DXVECTOR3 GetID()
	{
		return ChunkID_;
	}

	Block* GetBlock(int x, int y, int z)
	{
		return &Chunk_[x][y][z];
	}

private:
	
	Block*** Chunk_;
	bool IsVisible_;
	D3DXVECTOR3 ChunkID_;
	D3DXVECTOR3 Position_;
	InstancedGameObject* ChunkBlock_;
};