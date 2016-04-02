#include "Chunk.h"
#include "ViewFrustumManager.h"

Chunk::Chunk()
{
	Chunk_ = 0;
}

Chunk::~Chunk()
{
}

void Chunk::Initialise()
{
	//==================
	// Create Transform
	//==================

	Transform_ = new Transform;

	//================
	// Generate Chunk
	//================

	GenerateBlankChunk();
	GenerateTerrain();

	//==================
	// Initialise Flags
	//==================

	IsVisible_ = true;
}

void Chunk::Shutdown()
{
	// Delete the blocks
	for (int i = 0; i < BLOCK_COUNT_WIDTH; i++)
	{
		// Loop through y dimension
		for (int j = 0; j < BLOCK_COUNT_HEIGHT; j++)
		{
			delete[] Chunk_[i][j];
		}

		delete[] Chunk_[i];
	}

	delete[] Chunk_;
}

// Frame
void Chunk::Frame()
{
	// Check frustrum
	if (!IsVisible_)
	{
		return;
	}

	RefreshVisibleBlocks();
	RefreshVisibleFaces();
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

			// Loop through z dimension -- SELF PERLIN NOISE
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

void Chunk::GenerateTerrain()
{
	PerlinNoiseGenerator perlinNoise;

	// Generate a random seed for the noise to use
	perlinNoise.SetSeed(rand() % 10000);

	// Loop through x dimension
	for (int i = 0; i < BLOCK_COUNT_WIDTH; i++)
	{
		// Loop through z dimension
		for (int k = 0; k < BLOCK_COUNT_DEPTH; k++)
		{
			// Create noise values based on x and y coordinate
			double x = (GetTransform()->GetPosition().x + (double)k) / ((double)BLOCK_COUNT_WIDTH);
			double y = (GetTransform()->GetPosition().y + (double)i) / ((double)BLOCK_COUNT_HEIGHT);
			float noise = perlinNoise.CreateNoise(x, y, 0.8f);
			float height = floor(BLOCK_COUNT_HEIGHT * noise);

			// Loop through y dimension
			for (int j = 0; j < height; j++)
			{
				if (j < 1)
				{
					Chunk_[i][j][k].SetName("cobblestone");
					Chunk_[i][j][k].SetType(BlockType::Cobblestone);
					Chunk_[i][j][k].SetSolid(true);
				}
				else if (j < 3)
				{
					Chunk_[i][j][k].SetName("dirt");
					Chunk_[i][j][k].SetType(BlockType::Dirt);
					Chunk_[i][j][k].SetSolid(true);
				}
				else
				{
					Chunk_[i][j][k].SetName("sand");
					Chunk_[i][j][k].SetType(BlockType::Sand);
					Chunk_[i][j][k].SetSolid(true);
				}
				Chunk_[i][j][k].SetActive(true);
			}
		}
	}
}

void Chunk::RefreshVisibleBlocks()
{
	bool blockVisible;

	// Loop through x dimension
	for (int i = 0; i < BLOCK_COUNT_WIDTH; i++)
	{
		// Loop through y dimension
		for (int j = 0; j < BLOCK_COUNT_HEIGHT; j++)
		{
			// Loop through z dimension
			for (int k = 0; k < BLOCK_COUNT_DEPTH; k++)
			{
				// Check if the block is air
				if (Chunk_[i][j][k].GetType() == BlockType::Air)
				{
					// Skip as we auto dont want to see it
					Chunk_[i][j][k].SetActive(false);
					continue;
				}

				// Check if we can see the block
				blockVisible = ViewFrustumManager::Instance()->CheckCube(Chunk_[i][j][k].GetTransform()->GetPosition(), BLOCK_SIZE / 2);

				// Set block state
				Chunk_[i][j][k].SetActive(blockVisible);
			}
		}
	}
}

void Chunk::RefreshVisibleFaces()
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
				// Check if we can see it
				if (Chunk_[i][j][k].IsActive())
				{
					// Check if the block is air
					if (Chunk_[i][j][k].GetType() == BlockType::Air)
					{
						// Skip as we auto dont want to see it
						Chunk_[i][j][k].SetActive(false);
					}
					else
					{
						// Check if we can see the block
						CheckBlockVisibility(i, j, k);
					}
				}
			}
		}
	}
}

void Chunk::CheckBlockVisibility(int i, int j, int k)
{
	bool xPositive, xNegative;
	bool yPositive, yNegative;
	bool zPositive, zNegative;
	bool blockVisible;

	// Check if theres block in that particular position
	xPositive = CheckBlockTaken(i + 1, j, k);
	xNegative = CheckBlockTaken(i - 1, j, k);
	yPositive = CheckBlockTaken(i, j + 1, k);
	yNegative = CheckBlockTaken(i, j - 1, k);
	zPositive = CheckBlockTaken(i, j, k + 1);
	zNegative = CheckBlockTaken(i, j, k - 1);

	//============================================================\\
	// THIS IS WHERE WE PERFORM MESH UPDATING FOR DIFFERENT SIDES \\
	//============================================================\\

	blockVisible = !(xPositive && xNegative && yPositive && yNegative && zPositive && zNegative);

	Chunk_[i][j][k].SetActive(blockVisible);
}

bool Chunk::CheckBlockTaken(int i, int j, int k)
{
	// Check if the block is out of range
	if (i < 0 || i >= BLOCK_COUNT_WIDTH)
	{
		return false;
	}
	if (j < 0 || j >= BLOCK_COUNT_HEIGHT)
	{
		return false;
	}
	if (k < 0 || k >= BLOCK_COUNT_DEPTH)
	{
		return false;
	}

	switch (Chunk_[i][j][k].GetType())
	{
		case BlockType::Air:
			return false;
			break;

		default:
			return true;
			break;
	}

	return true;
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