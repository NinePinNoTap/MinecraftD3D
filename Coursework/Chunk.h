#pragma once

#include <string>

#include "Block.h"
#include "ShaderManager.h"
#include "Transform.h"
#include "Constants.h"
#include "BlockLoader.h"

class Chunk
{
public:
	Chunk();
	~Chunk();

	void Initialise();

	void Shutdown();

	void Refresh();

	bool CheckBlock(int x, int y, int z);

	// Frame
	void Update();

	void Render();

	// Getters
	Transform* GetTransform();

private:
	Block*** Chunk_;
	bool IsVisible_;
	Transform* Transform_;
};