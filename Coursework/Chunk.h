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

	void Initialise();
	void Shutdown();

	void Generate();

	// Frame
	void Frame();
	void Render();

	// Getters
	bool IsVisible();
	Transform* GetTransform();

private:
	void GenerateBlankChunk();
	void GenerateTerrain();

	void RefreshBlocks();

	void HandleBlock(int i, int j, int k);
	void HandleBlockFaces(int i, int j, int k);

	bool HandleBlockVisibility(int i, int j, int k);
	bool BlockIsEmpty(int x, int y, int z);

	bool BlockIsValid(int x, int y, int z);

	Block*** Chunk_;
	bool IsVisible_;
	Transform* Transform_;
};