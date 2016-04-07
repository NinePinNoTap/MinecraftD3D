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
	block->Initialise("block.obj");
	//block->SetName("gold");
	//block->SetType(BlockType::Gold);
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

void Chunk::Build()
{
	// Loop through x dimension
	for (int x = 0; x < NO_OF_BLOCKS_WIDTH; x++)
	{
		// Loop through y dimension
		for (int y = 0; y < NO_OF_BLOCKS_HEIGHT; y++)
		{
			// Loop through z dimension
			for (int z = 0; z < NO_OF_BLOCKS_DEPTH; z++)
			{
				Chunk_[x][y][z].Frame();
			}
		}
	}

	// Loop through x dimension
	for (int x = 0; x < NO_OF_BLOCKS_WIDTH; x++)
	{
		// Loop through y dimension
		for (int y = 0; y < NO_OF_BLOCKS_HEIGHT; y++)
		{
			// Loop through z dimension
			for (int z = 0; z < NO_OF_BLOCKS_DEPTH; z++)
			{
				if (Chunk_[x][y][z].IsActive() && Chunk_[x][y][z].IsSolid())
				{
					block->AddInstance(Chunk_[x][y][z].GetTransform()->GetPosition(), D3DXVECTOR2(rand()%4, rand()%3));
				}
			}
		}
	}

	block->RebuildInstanceBuffer();
}

// Frame
void Chunk::Frame()
{
	// Loop through x dimension
	for(int x = 0; x < NO_OF_BLOCKS_WIDTH; x++)
	{
		// Loop through y dimension
		for (int y = 0; y < NO_OF_BLOCKS_HEIGHT; y++)
		{
			// Loop through z dimension
			for (int z = 0; z < NO_OF_BLOCKS_DEPTH; z++)
			{
				//Chunk_[x][y][z].Frame();
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

	//if (needUpdate)
	//{
	//	// Loop through x dimension
	//	for(int x = 0; x < NO_OF_BLOCKS_WIDTH; x++)
	//	{
	//		// Loop through y dimension
	//		for (int y = 0; y < NO_OF_BLOCKS_HEIGHT; y++)
	//		{
	//			// Loop through z dimension
	//			for (int z = 0; z < NO_OF_BLOCKS_DEPTH; z++)
	//			{
	//				if (Chunk_[x][y][z].IsActive() && Chunk_[x][y][z].IsSolid())
	//				{
	//					// Render
	//					//Chunk_[x][y][z].Render();
	//					block->AddInstance(Chunk_[x][y][z].GetTransform()->GetPosition(), D3DXVECTOR2(0, 0));
	//				}
	//			}
	//		}
	//	}

	//	block->RebuildInstanceBuffer();
	//	needUpdate = false;
	//}

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
	for(int x = 0; x < NO_OF_BLOCKS_WIDTH; x++)
	{
		Chunk_[x] = new Block*[NO_OF_BLOCKS_HEIGHT];

		// Loop through y dimension
		for (int y = 0; y < NO_OF_BLOCKS_HEIGHT; y++)
		{
			Chunk_[x][y] = new Block[NO_OF_BLOCKS_DEPTH];

			// Loop through z dimension
			for (int z = 0; z < NO_OF_BLOCKS_DEPTH; z++)
			{
				// Initialise air block
				Chunk_[x][y][z] = Block();
				Chunk_[x][y][z].Initialise();
				Chunk_[x][y][z].SetShader("texture");

				// Set block world transform
				Chunk_[x][y][z].GetTransform()->SetPosition(Transform_->GetPosition());
				Chunk_[x][y][z].GetTransform()->Move(D3DXVECTOR3(x, y, z) * BLOCK_SIZE);
				Chunk_[x][y][z].GetTransform()->Move(-halfChunkOffset);
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