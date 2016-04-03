#pragma once

#include <string>

#include "Block.h"
#include "Constants.h"
#include "ShaderManager.h"
#include "Transform.h"

class Chunk
{
public:
	Chunk();
	~Chunk();

	void Initialise(int x, int y, int z);
	void Shutdown();

	// Frame
	void Frame();
	void Render();
	void RefreshBlocks();

	Block* GetBlock(int x, int y, int z)
	{
		return &Chunk_[x][y][z];
	}

	// Getters
	bool IsVisible();
	Transform* GetTransform();
	D3DXVECTOR3 GetChunkID()
	{
		return ChunkID_;
	}

private:
	void GenerateBlankChunk();
	void GenerateTerrain();

	//void RefreshBlocks();

	void HandleBlock(int i, int j, int k);
	void HandleBlockFaces(int i, int j, int k);

	bool HandleBlockVisibility(int i, int j, int k);
	bool BlockIsEmpty(int x, int y, int z);

	bool BlockIsValid(int x, int y, int z);

	Block*** Chunk_;
	bool IsVisible_;
	Transform* Transform_;
	D3DXVECTOR3 ChunkID_;
};