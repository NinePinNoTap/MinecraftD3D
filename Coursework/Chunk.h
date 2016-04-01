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

	// Frame
	void Frame();
	void Render();

	// Getters
	bool IsVisible();
	Transform* GetTransform();

private:
	void GenerateChunk();
	void RefreshVisible();
	bool CheckBlock(int x, int y, int z);

	Block*** Chunk_;
	bool IsVisible_;
	Transform* Transform_;
};