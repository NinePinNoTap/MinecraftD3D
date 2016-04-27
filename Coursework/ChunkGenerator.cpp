#include "ChunkGenerator.h"

ChunkGenerator::ChunkGenerator()
{
}


ChunkGenerator::~ChunkGenerator()
{
}

void ChunkGenerator::AddLayer(TerrainLayer terrainLayer)
{
	Layers_.push_back(terrainLayer);
}

void ChunkGenerator::GenerateChunk(Chunk& chunk)
{
	// Make sure we have layers to work with
	if (Layers_.empty())
	{
		// Add default layers
		Layers_.push_back(TerrainLayer("Layer 1", "bedrock", 1, 1, 1, 1, LayerType::Additive));
		Layers_.push_back(TerrainLayer("Layer 2", "stone", 1, 1, 1, 1, LayerType::Additive));
		Layers_.push_back(TerrainLayer("Layer 3", "dirt", 1, 1, 1, 1, LayerType::Additive));
		OutputToDebug("No layers found. Creating default terrain.");
	}

	//==================
	// Generate Terrain
	//==================

	for (int x = chunk.GetPosition().x; x < chunk.GetPosition().x + World::ChunkSize; x++)
	{
		for (int z = chunk.GetPosition().z; z < chunk.GetPosition().z + World::ChunkSize; z++)
		{
			GenerateColumn(&chunk, x, z);
		}
	}
}

void ChunkGenerator::GenerateColumn(Chunk* chunk, int x, int z)
{
	int currentHeight;

	// Set the starting height we will work with
	currentHeight = 0;

	// Loop through and generate terrain using layers
	for (unsigned int i = 0; i < Layers_.size(); i++)
	{
		if (Layers_[i].blockName == "air")
		{
			GenerateCave(chunk, x, z, currentHeight);
		}
		else
		{
			GenerateLayer(chunk, Layers_[i], x, z, currentHeight);
		}
	}
}

void ChunkGenerator::GenerateLayer(Chunk* chunk, TerrainLayer terrainLayer, int x, int z, int& currentHeight)
{
	int maxHeight;
	int baseNoise;
	D3DXVECTOR3 blockLocal;
	Block* currentTarget;

	// Generate noise using layer settings
	baseNoise = GetNoise(x, 0, z, terrainLayer.frequency, terrainLayer.amplitude, terrainLayer.exponent);

	//=============================
	// Define Maximum Layer Height
	//=============================

	if (terrainLayer.layerType == LayerType::Absolute)
	{
		// Calculate highest point we generate up to
		maxHeight = terrainLayer.baseHeight + baseNoise;
	}
	else if (terrainLayer.layerType == LayerType::Additive)
	{
		// Calculate highest point we generate up to
		maxHeight = currentHeight + terrainLayer.baseHeight + baseNoise;
	}

	//======================
	// Create Terrain Layer
	//======================

	for (float y = currentHeight; y < maxHeight; y++)
	{
		// Calculate its local position in the chunk
		blockLocal = D3DXVECTOR3(x, y, z) - chunk->GetPosition();

		// Update the block
		currentTarget = chunk->GetBlock(blockLocal.x, blockLocal.y, blockLocal.z);
		if (currentTarget)
		{
			currentTarget->CopyFrom(World::Blocks[terrainLayer.blockName]);
		}
	}

	// Update returned value
	currentHeight = maxHeight;
}

void ChunkGenerator::GenerateCave(Chunk* chunk, int x, int z, int& currentHeight)
{
	Block* currentTarget;
	D3DXVECTOR3 blockLocal;
	int caveBottom, caveHeight;
	
	// Calculate noise height
	caveBottom = GetNoise(x, -1000, z, 500, 70, 1);
	caveHeight = caveBottom + GetNoise(x, 1000, z, 50, 35, 1) - 17;

	if (caveHeight > caveBottom)
	{
		caveBottom -= caveHeight / 2;
		int caveTop = caveHeight / 2;
		
		for (int y = caveBottom; y < caveTop; y++)
		{
			// Calculate its local position in the chunk
			blockLocal = D3DXVECTOR3(x, y, z) - chunk->GetPosition();

			// Update the block
			currentTarget = chunk->GetBlock(blockLocal.x, blockLocal.y, blockLocal.z);
			if (currentTarget)
			{
				currentTarget->CopyFrom(World::Blocks["air"]);
			}
		}

		// Change current height
		if (caveTop > currentHeight)
			currentHeight = caveBottom;
	}
}