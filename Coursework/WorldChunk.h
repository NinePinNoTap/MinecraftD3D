#pragma once

#include <vector>

#include "Chunk.h"
#include "TerrainLayer.h"

class WorldChunk
{
public:
	WorldChunk();
	~WorldChunk();

	// Initialise
	void Initialise(int x, int z, int chunkHeight);

	void Frame();
	void Render();

	Chunk* GetChunk(int y);

private:
	void InitialiseLayers();
	void GenerateTerrain();
	void GenerateColumn(int x, int z);
	int GenerateLayer(TerrainLayer terrainLayer, int x, int z, int currentHeight);

	vector<Chunk*> Chunks_;
	vector<TerrainLayer> Layers_;

	D3DXVECTOR2 ChunkID_;
};

