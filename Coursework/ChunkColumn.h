#pragma once

#include <vector>

#include "Chunk.h"
#include "TerrainLayer.h"
#include "ChunkGenerator.h"

class ChunkColumn
{
public:
	ChunkColumn();
	~ChunkColumn();

	// Initialise
	void Initialise(int x, int z, int chunkHeight);

	// Shutdown
	void Shutdown();

	// Frame
	bool Frame();
	bool Render();

	// Getters
	Chunk* GetChunk(int y);

private:
	vector<Chunk*> Chunks_;
	ChunkGenerator* ChunkGenerator_;

	D3DXVECTOR3 ChunkPos_;

	bool Result_;
};

