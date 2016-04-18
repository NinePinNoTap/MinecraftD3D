#include "WorldChunk.h"


WorldChunk::WorldChunk()
{
}

WorldChunk::~WorldChunk()
{
}

void WorldChunk::Initialise(int x, int z, int chunkCount)
{
	for (int i = 0; i < chunkCount; i++)
	{
		float tBefore = timeGetTime();

		// Created chunk
		Chunk* createdChunk = new Chunk;
		createdChunk->Initialise(x, i, z);
		createdChunk->Generate();
		
		// Add to list
		Chunks_.push_back(createdChunk);

		// Clean Up
		createdChunk = 0;

		OutputTimeDelay("Creation Time : ", tBefore, timeGetTime());
	}

	// Generate chunks
	GenerateTerrain();
}

void WorldChunk::Frame()
{
	for (unsigned int i = 0; i < Chunks_.size(); i++)
	{
		Chunks_[i]->Frame();
	}
}

void WorldChunk::Render()
{
	for (unsigned int i = 0; i < Chunks_.size(); i++)
	{
		if (Chunks_[i]->IsVisible())
		{
			Chunks_[i]->Render();
		}
	}
}

void WorldChunk::InitialiseLayers()
{
	// Add layer
	Layers_.push_back(TerrainLayer());
}

void WorldChunk::GenerateTerrain()
{
	int currentHeight;

	// Set initial height
	currentHeight = 0;

	// Loop through
	for (unsigned int i = 0; i < Layers_.size(); i++)
	{
		// Generate layers
		// Need to set blocks
		currentHeight += GenerateLayer(Layers_[i], currentHeight);
	}
}

int WorldChunk::GenerateLayer(TerrainLayer terrainLayer, int currentHeight)
{
	return 0;
}

Chunk* WorldChunk::GetChunk(int y)
{
	return Chunks_[y];
}