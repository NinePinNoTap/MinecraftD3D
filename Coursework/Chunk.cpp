#include "Chunk.h"

Chunk::Chunk()
{
	Chunk_ = 0;
}

Chunk::~Chunk()
{
}

void Chunk::Initialise(int x, int y, int z)
{
	// Store the ID of the chunk
	ChunkID_ = D3DXVECTOR3(x, y, z);

	//==================
	// Create Transform
	//==================

	Transform_ = new Transform;
	Transform_->SetPosition(CHUNK_TOTAL_WIDTH * x, CHUNK_TOTAL_HEIGHT * y, CHUNK_TOTAL_DEPTH * z);

	//================
	// Generate Chunk
	//================

	GenerateBlankChunk();
		
	//==================
	// Initialise Flags
	//==================

	IsVisible_ = true;
}

void Chunk::Shutdown()
{
	// Delete the blocks
	for (int x = 0; x < BLOCK_COUNT_WIDTH; x++)
	{
		// Loop through y dimension
		for (int y = 0; y < BLOCK_COUNT_HEIGHT; y++)
		{
			delete[] Chunk_[x][y];
		}

		delete[] Chunk_[x];
	}

	delete[] Chunk_;
}

// Frame
void Chunk::Frame()
{
	// Loop through x dimension
	for (int i = 0; i < BLOCK_COUNT_WIDTH; i++)
	{
		// Loop through y dimension
		for (int j = 0; j < BLOCK_COUNT_HEIGHT; j++)
		{
			// Loop through z dimension
			for (int k = 0; k < BLOCK_COUNT_DEPTH; k++)
			{
				Chunk_[i][j][k].Frame();
			}
		}
	}
}

void Chunk::Render()
{
	if (!IsVisible_)
	{
		return;
	}

	// Loop through x dimension
	for (int i = 0; i < BLOCK_COUNT_WIDTH; i++)
	{
		// Loop through y dimension
		for (int j = 0; j < BLOCK_COUNT_HEIGHT; j++)
		{
			// Loop through z dimension
			for (int k = 0; k < BLOCK_COUNT_DEPTH; k++)
			{
				if (Chunk_[i][j][k].IsActive())
				{
					// Render
					Chunk_[i][j][k].Render();
				}
			}
		}
	}
}

// Terrain Generation
void Chunk::GenerateBlankChunk()
{
	D3DXVECTOR3 halfChunkOffset;

	// Calculate half dimensions of the chunk
	halfChunkOffset = D3DXVECTOR3(CHUNK_TOTAL_WIDTH, CHUNK_TOTAL_HEIGHT, CHUNK_COUNT_DEPTH) / 2;

	// Create the 3D array to store blocks in chunk
	Chunk_ = new Block**[BLOCK_COUNT_WIDTH];

	// Loop through x dimension
	for (int i = 0; i < BLOCK_COUNT_WIDTH; i++)
	{
		Chunk_[i] = new Block*[BLOCK_COUNT_HEIGHT];

		// Loop through y dimension
		for (int j = 0; j < BLOCK_COUNT_HEIGHT; j++)
		{
			Chunk_[i][j] = new Block[BLOCK_COUNT_DEPTH];

			// Loop through z dimension
			for (int k = 0; k < BLOCK_COUNT_DEPTH; k++)
			{
				// Initialise air block
				Chunk_[i][j][k] = Block();
				Chunk_[i][j][k].Initialise();
				Chunk_[i][j][k].SetShader("texture");

				// Set block world transform
				Chunk_[i][j][k].GetTransform()->SetPosition(Transform_->GetPosition());
				Chunk_[i][j][k].GetTransform()->Move(D3DXVECTOR3(i, j, k) * BLOCK_SIZE);
				Chunk_[i][j][k].GetTransform()->Move(-halfChunkOffset);
			}
		}
	}
}

// Getters
bool Chunk::IsVisible()
{
	return IsVisible_;
}

Transform* Chunk::GetTransform()
{
	return Transform_;
}