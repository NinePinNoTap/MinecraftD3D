#include "ChunkColumn.h"
#include "VoxelWorld.h"

ChunkColumn::ChunkColumn()
{
}

ChunkColumn::~ChunkColumn()
{
}

void ChunkColumn::Initialise(int x, int z, int chunkCount)
{
	ChunkPos_ = D3DXVECTOR3(x, 0, z);

	for (int i = 0; i < chunkCount; i++)
	{
		float tBefore = timeGetTime();

		// Created chunk
		Chunk* createdChunk = new Chunk;
		createdChunk->Initialise(x, i, z);
		
		// Add to list
		Chunks_.push_back(createdChunk);

		// Clean Up
		createdChunk = 0;

		OutputTimeDelay("Creation Time : " + GetKey(x,i,z), tBefore, timeGetTime());
	}

	// Generate Terrain Layers
	InitialiseLayers();

	// Generate chunks
	GenerateTerrain();

	OutputToDebug("Generated Terrain");

	for (int i = 0; i < chunkCount; i++)
	{
		Chunks_[i]->RefreshVisible();
	}

	OutputToDebug("Refreshed Terrain");
}

bool ChunkColumn::Frame()
{
	for (unsigned int i = 0; i < Chunks_.size(); i++)
	{
		Chunks_[i]->Frame();
	}

	return true;
}

bool ChunkColumn::Render()
{
	// Loop through chunks and render them
	for (unsigned int i = 0; i < Chunks_.size(); i++)
	{
		if (Chunks_[i]->IsVisible())
		{
			Result_ = Chunks_[i]->Render();
			if (!Result_)
			{
				return false;
			}
		}
	}

	return true;
}

void ChunkColumn::InitialiseLayers()
{
	Layers_.push_back(TerrainLayer("Bedrock",		"bedrock",	 0,		1,	 2,   1, LayerType::Absolute));

	Layers_.push_back(TerrainLayer("Base Stone",	"stone",	39,    63,   9,   1, LayerType::Absolute));
	//Layers_.push_back(TerrainLayer("Mountain 1",	"stone",	 1,   129,  30,   1, LayerType::Absolute));

	//Layers_.push_back(TerrainLayer("Mountain 2",	"stone",	 4,   111,  21,   1, LayerType::Additive));
	//Layers_.push_back(TerrainLayer("Stone Noise",	"stone",	 1,     7,   3,   1, LayerType::Additive));

	Layers_.push_back(TerrainLayer("Dirt 1",	    "dirt",		23,    16,   8,   1, LayerType::Additive));
}

void ChunkColumn::GenerateTerrain()
{
	D3DXVECTOR3 worldPos = ChunkPos_ * Config::World::ChunkSize;

	// Loop through and generate terrain for specific x/z coordinate
	for (int x = worldPos.x; x < worldPos.x + Config::World::ChunkSize; x++)
	{
		for (int z = worldPos.z; z < worldPos.z + Config::World::ChunkSize; z++)
		{
			GenerateColumn(x, z);
		}
	}
}

void ChunkColumn::GenerateColumn(int x, int z)
{
	int currentHeight;

	// Set initial height
	currentHeight = 0;

	// Loop through
	for (unsigned int i = 0; i < Layers_.size(); i++)
	{
		currentHeight = GenerateLayer(Layers_[i], x, z, currentHeight);
	}
}

int ChunkColumn::GenerateLayer(TerrainLayer terrainLayer, int x, int z, int currentHeight)
{
	int baseNoise, maxHeight;

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
		VoxelWorld::Instance()->SetBlock(x, y, z, terrainLayer.blockName);
	}

	if (terrainLayer.layerType == LayerType::Absolute)
	{
		// Increase current height 
		if (maxHeight > currentHeight)
		{
			currentHeight = maxHeight;
		}
	}

	return maxHeight;
}

Chunk* ChunkColumn::GetChunk(int y)
{
	if (RangeCheck(y, 0, Chunks_.size() - 1))
	{
		return Chunks_[y];
	}

	return 0;
}