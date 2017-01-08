#include "ChunkGenerator.h"

ChunkGenerator::ChunkGenerator()
{
}


ChunkGenerator::~ChunkGenerator()
{
}

void ChunkGenerator::addLayer(TerrainLayer terrainLayer)
{
	m_layers.push_back(terrainLayer);
}

void ChunkGenerator::generateChunk(Chunk& chunk)
{
	// Make sure we have layers to work with
	if (m_layers.empty())
	{
		// Add default layers
		m_layers.push_back(TerrainLayer("Layer 1", "bedrock", 1, 1, 1, 1, LayerType::Additive));
		m_layers.push_back(TerrainLayer("Layer 2", "stone", 1, 1, 1, 1, LayerType::Additive));
		m_layers.push_back(TerrainLayer("Layer 3", "dirt", 1, 1, 1, 1, LayerType::Additive));
		outputToDebug("No layers found. Creating default terrain.");
	}

	//==================
	// generate Terrain
	//==================

	for (int x = chunk.getPosition().x; x < chunk.getPosition().x + World::ChunkSize; x++)
	{
		for (int z = chunk.getPosition().z; z < chunk.getPosition().z + World::ChunkSize; z++)
		{
			generateColumn(&chunk, x, z);
		}
	}
}

void ChunkGenerator::generateColumn(Chunk* chunk, int x, int z)
{
	int currentHeight;

	// set the starting height we will work with
	currentHeight = 0;

	// Loop through and generate terrain using layers
	for (unsigned int i = 0; i < m_layers.size(); i++)
	{
		if (m_layers[i].blockName == "air")
		{
			generateCave(chunk, x, z, currentHeight);
		}
		else
		{
			generateLayer(chunk, m_layers[i], x, z, currentHeight);
		}
	}
}

void ChunkGenerator::generateLayer(Chunk* chunk, TerrainLayer terrainLayer, int x, int z, int& currentHeight)
{
	int maxHeight;
	int baseNoise;
	D3DXVECTOR3 blockLocal;
	Block* currentTarget;

	// generate noise using layer settings
	baseNoise = getNoise(x, 0, z, terrainLayer.frequency, terrainLayer.amplitude, terrainLayer.exponent);

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
	// create Terrain Layer
	//======================

	for (float y = currentHeight; y < maxHeight; y++)
	{
		// Calculate its local position in the chunk
		blockLocal = D3DXVECTOR3(x, y, z) - chunk->getPosition();

		// update the block
		currentTarget = chunk->getBlock(blockLocal.x, blockLocal.y, blockLocal.z);
		if (currentTarget)
		{
			currentTarget->clone(World::Blocks[terrainLayer.blockName]);
		}
	}

	// update returned value
	currentHeight = maxHeight;
}

void ChunkGenerator::generateCave(Chunk* chunk, int x, int z, int& currentHeight)
{
	Block* currentTarget;
	D3DXVECTOR3 blockLocal;
	int caveBottom, caveHeight;
	
	// Calculate noise height
	caveBottom = getNoise(x, -1000, z, 500, 70, 1);
	caveHeight = caveBottom + getNoise(x, 1000, z, 50, 35, 1) - 17;

	if (caveHeight > caveBottom)
	{
		caveBottom -= caveHeight / 2;
		int caveTop = caveHeight / 2;
		
		for (int y = caveBottom; y < caveTop; y++)
		{
			// Calculate its local position in the chunk
			blockLocal = D3DXVECTOR3(x, y, z) - chunk->getPosition();

			// update the block
			currentTarget = chunk->getBlock(blockLocal.x, blockLocal.y, blockLocal.z);
			if (currentTarget)
			{
				currentTarget->clone(World::Blocks["air"]);
			}
		}

		// Change current height
		if (caveTop > currentHeight)
			currentHeight = caveBottom;
	}
}