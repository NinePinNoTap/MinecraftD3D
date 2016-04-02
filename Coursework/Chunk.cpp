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

	GenerateChunk();
	RefreshVisible();

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

void Chunk::GenerateChunk()
{
	D3DXVECTOR3 blockPosition;

	//
	// Build Blank Chunk
	//

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
				Chunk_[i][j][k] = Block();
			}
		}
	}

	//
	// Fill the Chunk
	//

	PerlinNoiseGenerator perlinNoise;

	// Generate a random seed to use
	perlinNoise.SetSeed(rand()%10000);

	// Update blocks
	for (int i = 0; i < BLOCK_COUNT_WIDTH; i++)
	{
		for (int k = 0; k < BLOCK_COUNT_DEPTH; k++)
		{
			// Create noise
			double x = (GetTransform()->GetPosition().x + (double)k) / ((double)BLOCK_COUNT_WIDTH);
			double y = (GetTransform()->GetPosition().y + (double)i) / ((double)BLOCK_COUNT_HEIGHT);
			float noise = perlinNoise.CreateNoise(x, y, 0.8f);
			float val = floor(BLOCK_COUNT_HEIGHT * noise);

			// Loop through z dimension -- SELF PERLIN NOISE
			for (int j = 0; j < val; j++)
			{
				if (j < 1)
				{
					Chunk_[i][j][k] = Block("cobblestone", BlockType::Cobblestone, true);
				}
				else if (j < 3)
				{
					Chunk_[i][j][k] = Block("dirt", BlockType::Dirt, true);
				}
				else
				{
					Chunk_[i][j][k] = Block("sand", BlockType::Sand, true);
				}

				// Calculate block position inside the world
				blockPosition = GetTransform()->GetPosition();
				blockPosition += D3DXVECTOR3(i, j, k) * BLOCK_SIZE;

				// Apply transform to block
				Chunk_[i][j][k].GetTransform()->SetPosition(blockPosition);
			}
		}
	}
}

void Chunk::RefreshVisible()
{
	bool xPositive, xNegative;
	bool yPositive, yNegative;
	bool zPositive, zNegative;
	int activeBlocks = 0;

	// Loop through x dimension
	for (int i = 0; i < BLOCK_COUNT_WIDTH; i++)
	{
		// Loop through y dimension
		for (int j = 0; j < BLOCK_COUNT_HEIGHT; j++)
		{
			// Loop through z dimension
			for (int k = 0; k < BLOCK_COUNT_DEPTH; k++)
			{
				// Make sure we can see the block
				if (!ViewFrustumManager::Instance()->CheckCube(Chunk_[i][j][k].GetTransform()->GetPosition(), BLOCK_SIZE / 2))
				{
					Chunk_[i][j][k].SetActive(false);
					continue;
				}

				// Check if the block is air
				if (Chunk_[i][j][k].GetType() == BlockType::Air)
				{
					// Skip as we auto dont want to see it
					Chunk_[i][j][k].SetActive(false);
				}
				else if (!ViewFrustumManager::Instance()->CheckCube(Chunk_[i][j][k].GetTransform()->GetPosition(), 0.5f))
				{
					Chunk_[i][j][k].SetActive(false);
				}
				else
				{
					// Check if theres block in that particle position
					xPositive = CheckBlock(i + 1, j, k);
					xNegative = CheckBlock(i - 1, j, k);
					yPositive = CheckBlock(i, j + 1, k);
					yNegative = CheckBlock(i, j - 1, k);
					zPositive = CheckBlock(i, j, k + 1);
					zNegative = CheckBlock(i, j, k - 1);

					// THIS IS WHERE WE PERFORM MESH UPDATING FOR DIFFERENT SIDES

					bool result = !(xPositive && xNegative && yPositive && yNegative && zPositive && zNegative);

					Chunk_[i][j][k].SetActive(result);
				}			

				// Debugging
				if (Chunk_[i][j][k].IsActive())
				{
					activeBlocks++;
				}
			}
		}
	}

	OutputToDebug("Rendered Blocks : ");
	OutputToDebug(to_string(activeBlocks));
	OutputToDebug(" / ");
	OutputToDebug(to_string(TOTAL_BLOCKS_IN_CHUNK));
	OutputToDebug("\n");
}

bool Chunk::CheckBlock(int x, int y, int z)
{
	// Check if the block is out of range
	if (x < 0 || x >= BLOCK_COUNT_WIDTH)
	{
		return false;
	}
	if (y < 0 || y >= BLOCK_COUNT_HEIGHT)
	{
		return false;
	}
	if (z < 0 || z >= BLOCK_COUNT_DEPTH)
	{
		return false;
	}

	// Check the type of block
	switch (Chunk_[x][y][z].GetType())
	{
		// AIR - Simulates no block
		case BlockType::Air:
			return false;
			break;

		default:
			return true;
			break;
	}

	return true;
}

// Frame
void Chunk::Frame()
{
	// Check frustrum
	// Set IsActive_ based on frustum state
	RefreshVisible();
}

void Chunk::Render()
{
	if (!IsVisible_)
		return;

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
					Mesh3D* mesh = Chunk_[i][j][k].GetModel()->GetMesh();
					Material* material = Chunk_[i][j][k].GetModel()->GetMaterial();
					Transform* transform = Chunk_[i][j][k].GetTransform();

					// Render
					ShaderManager::Instance()->LightShader(&Chunk_[i][j][k]);
				}
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