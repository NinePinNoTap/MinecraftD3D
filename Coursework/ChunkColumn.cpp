#include "ChunkColumn.h"
#include "VoxelWorld.h"

ChunkColumn::ChunkColumn()
{
}

ChunkColumn::~ChunkColumn()
{
}

// Initialising
void ChunkColumn::Initialise(int x, int z, int chunkCount)
{
	// Define the world position of the base
	ChunkPos_ = D3DXVECTOR3(x, 0, z);

	// Create a generator for the chunks
	ChunkGenerator_ = new ChunkGenerator;
	ChunkGenerator_->AddLayer(TerrainLayer("Bedrock", "bedrock", 0, 1, 2, 1, LayerType::Absolute));
	ChunkGenerator_->AddLayer(TerrainLayer("Base Stone", "stone", 39, 63, 9, 1, LayerType::Absolute));
	ChunkGenerator_->AddLayer(TerrainLayer("Dirt 1", "dirt", 23, 16, 8, 1, LayerType::Additive));

	// Generate chunks and build them
	for (int i = chunkCount - 1; i >= 0; i--)
	{
		// Created chunk
		Chunk* createdChunk = new Chunk;
		createdChunk->Initialise(x, i, z);
		ChunkGenerator_->GenerateChunk(*createdChunk);
		createdChunk->RefreshVisible();

		// Add to list
		Chunks_.insert(Chunks_.begin(), createdChunk);

		// Clean Up
		createdChunk = 0;
	}

	// Do a final refresh
	for (int i = 0; i < chunkCount; i++)
	{
		Chunks_[i]->RefreshVisible();
	}
}

// Shutdown
void ChunkColumn::Shutdown()
{
	if (ChunkGenerator_)
	{
		ChunkGenerator_ = 0;
	}

	for (unsigned int i = 0; i < Chunks_.size(); i++)
	{
		// Add to list
		Chunks_[i]->Shutdown();
		Chunks_[i] = 0;
	}

	Chunks_.clear();
}

// Frame
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

// Getters
Chunk* ChunkColumn::GetChunk(int y)
{
	if (RangeCheck(y, 0, Chunks_.size() - 1))
	{
		return Chunks_[y];
	}

	return 0;
}