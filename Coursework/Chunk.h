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
	void Update();
	void Render();

	// Getters
	Transform* GetTransform();

private:
	void GenerateChunk();
	void RefreshVisible();
	bool CheckBlock(int x, int y, int z);

	Block*** Chunk_;
	bool IsVisible_;
	Transform* Transform_;
};