#include "Chunk.h"

Chunk::Chunk()
{
	Chunk_ = 0;
}

Chunk::~Chunk()
{
}

#include "Block.h"
Block* block;

void Chunk::Initialise(int x, int y, int z)
{
	// Store the ID of the chunk
	ChunkID_ = D3DXVECTOR3(x, y, z);

	//==================
	// Create Transform
	//==================

	Transform_ = new Transform;
	Transform_->SetPosition(CHUNK_WIDTH * x, CHUNK_HEIGHT * y, CHUNK_DEPTH * z);

	//================
	// Generate Chunk
	//================

	GenerateBlankChunk();
		
	//==================
	// Initialise Flags
	//==================

	IsVisible_ = true;

	block = new Block;
	block->Initialise();
	block->SetName("gold");
	block->SetType(BlockType::Gold);
	block->SetActive(true);
	block->SetSolid(true);
	block->SetShader("instancedlight");

	block->GetModel()->GetMaterial()->SetBaseTexture("blocks.dds");
}

void Chunk::Shutdown()
{
	// Delete the blocks
	for (int x = 0; x < NO_OF_BLOCKS_WIDTH; x++)
	{
		// Loop through y dimension
		for (int y = 0; y < NO_OF_BLOCKS_HEIGHT; y++)
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
	for (int i = 0; i < NO_OF_BLOCKS_WIDTH; i++)
	{
		// Loop through y dimension
		for (int j = 0; j < NO_OF_BLOCKS_HEIGHT; j++)
		{
			// Loop through z dimension
			for (int k = 0; k < NO_OF_BLOCKS_DEPTH; k++)
			{
				Chunk_[i][j][k].Frame();
			}
		}
	}
}

bool needUpdate = true;
void Chunk::Render()
{
	if (!IsVisible_)
	{
		return;
	}

	if (needUpdate)
	{
		// Loop through x dimension
		for (int i = 0; i < NO_OF_BLOCKS_WIDTH; i++)
		{
			// Loop through y dimension
			for (int j = 0; j < NO_OF_BLOCKS_HEIGHT; j++)
			{
				// Loop through z dimension
				for (int k = 0; k < NO_OF_BLOCKS_DEPTH; k++)
				{
					if (Chunk_[i][j][k].IsActive() && Chunk_[i][j][k].IsSolid())
					{
						// Render
						//Chunk_[i][j][k].Render();
						block->AddInstance(Chunk_[i][j][k].GetTransform()->GetPosition(), D3DXVECTOR2(0, 0));
					}
				}
			}
		}

		block->RebuildInstanceBuffer();
		needUpdate = false;
	}

	block->Render();
}

// Terrain Generation
void Chunk::GenerateBlankChunk()
{
	D3DXVECTOR3 halfChunkOffset;

	// Calculate half dimensions of the chunk
	halfChunkOffset = D3DXVECTOR3(CHUNK_WIDTH, CHUNK_HEIGHT, NO_OF_CHUNKS_DEPTH) / 2;

	// Create the 3D array to store blocks in chunk
	Chunk_ = new Block**[NO_OF_BLOCKS_WIDTH];

	// Loop through x dimension
	for (int i = 0; i < NO_OF_BLOCKS_WIDTH; i++)
	{
		Chunk_[i] = new Block*[NO_OF_BLOCKS_HEIGHT];

		// Loop through y dimension
		for (int j = 0; j < NO_OF_BLOCKS_HEIGHT; j++)
		{
			Chunk_[i][j] = new Block[NO_OF_BLOCKS_DEPTH];

			// Loop through z dimension
			for (int k = 0; k < NO_OF_BLOCKS_DEPTH; k++)
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