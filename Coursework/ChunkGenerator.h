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

	void AddLayer(TerrainLayer terrainLayer);

	void GenerateChunk(Chunk& chunk);

private:
	void GenerateColumn(Chunk* chunk, int x, int z);
	void GenerateLayer(Chunk* chunk, TerrainLayer terrainLayer, int x, int z, int& currentHeight);
	void GenerateCave(Chunk* chunk, int x, int z, int& currentHeight);

	vector<TerrainLayer> Layers_;
};