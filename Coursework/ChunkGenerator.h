#pragma once

#include <vector>

#include "Block.h"
#include "Chunk.h"
#include "TerrainLayer.h"

class ChunkGenerator
{
public:
	ChunkGenerator();
	~ChunkGenerator();

	void addLayer(TerrainLayer terrainLayer);

	void generateChunk(Chunk& chunk);

private:
	void generateColumn(Chunk* chunk, int x, int z);
	void generateLayer(Chunk* chunk, TerrainLayer terrainLayer, int x, int z, int& currentHeight);
	void generateCave(Chunk* chunk, int x, int z, int& currentHeight);

	vector<TerrainLayer> m_layers;
};