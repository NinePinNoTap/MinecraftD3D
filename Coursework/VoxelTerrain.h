#pragma once

#include "Chunk.h"
#include "Constants.h"
#include "BlockLoader.h"

class VoxelTerrain
{
public:
	VoxelTerrain();
	~VoxelTerrain();

	void Initialise();

	void Frame();
	void Render();

private:
	BlockLoader* BlockLoader_;
	Chunk**** TerrainChunks_;
};