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

void Chunk::Generate()
{
	//================
	// Generate Chunk
	//================

	GenerateBlankChunk();
	GenerateTerrain();
	RefreshBlocks();
}

// Frame
void Chunk::Frame()
{
	// Check frustrum
	if (!IsVisible_)
	{
		return;
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
					Block temp = Chunk_[i][j][k];
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
				Chunk_[i][j][k].SetShader("light");

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
				}
				else if (j < 3)
				{
					Chunk_[i][j][k].SetName("dirt");
					Chunk_[i][j][k].SetType(BlockType::Dirt);
				}
				else
				{
					Chunk_[i][j][k].SetName("sand");
					Chunk_[i][j][k].SetType(BlockType::Sand);
				}

				Chunk_[i][j][k].SetSolid(true);
				Chunk_[i][j][k].SetActive(true);
			}
		}
	}
}

void Chunk::RefreshBlocks()
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
				HandleBlock(i, j, k);
			}
		}
	}
}

void Chunk::HandleBlock(int i, int j, int k)
{
	//===================
	// Check Block State
	//===================

	if (BlockIsEmpty(i, j, k))
	{
		// Block is empty
		Chunk_[i][j][k].SetActive(false);
		return;
	}

	//=====================
	// Check Hiddden State
	//=====================

	if (!HandleBlockVisibility(i, j, k))
	{
		// Block is hidden
		Chunk_[i][j][k].SetActive(false);
		return;
	}

	//==============================
	// Check what faces can be seen
	//==============================

	if (Chunk_[i][j][k].IsActive())
	{
		// Process faces
		HandleBlockFaces(i, j, k);
	}

	return;


	//===============================================
	// Check if the block is within the view frustum
	//===============================================
	
	if (!ViewFrustumManager::Instance()->CheckCube(Chunk_[i][j][k].GetTransform()->GetPosition(), BLOCK_SIZE / 2))
	{
		Chunk_[i][j][k].SetActive(false);
		return;
	}
}

bool Chunk::HandleBlockVisibility(int i, int j, int k)
{
	bool blockRight, blockLeft;
	bool blockUp, blockDown;
	bool blockForward, blockBackward;
	bool blockVisible;

	//==========================
	// Check Surrounding Blocks
	//==========================

	blockRight =	BlockIsEmpty(i + 1, j,		k);
	blockLeft =		BlockIsEmpty(i - 1, j,		k);
	blockUp =		BlockIsEmpty(i,		j + 1,	k);
	blockDown =		BlockIsEmpty(i,		j - 1,	k);
	blockForward =	BlockIsEmpty(i,		j,		k + 1);
	blockBackward = BlockIsEmpty(i,		j,		k - 1);

	blockVisible = (blockRight || blockLeft || blockUp || blockDown || blockForward || blockBackward);
	if (blockVisible)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void Chunk::HandleBlockFaces(int i, int j, int k)
{
	Block tempBlock;
	bool canSee[6];
	int ID = 0;

	//==========================
	// Check Surrounding Blocks
	//==========================

	// Up
	if (BlockIsValid(i, j + 1, k))
	{
		tempBlock = Chunk_[i][j + 1][k];
		canSee[ID] = !tempBlock.IsSolid();
	}
	else
	{
		canSee[ID] = true;
	}
	ID++;

	// Down
	if (BlockIsValid(i, j - 1, k))
	{
		tempBlock = Chunk_[i][j - 1][k];
		canSee[ID] = !tempBlock.IsSolid();
	}
	else
	{
		canSee[ID] = true;
	}
	ID++;

	// Left
	if (BlockIsValid(i - 1, j, k))
	{
		tempBlock = Chunk_[i - 1][j][k];
		canSee[ID] = !tempBlock.IsSolid();
	}
	else
	{
		canSee[ID] = true;
	}
	ID++;

	// Right
	if (BlockIsValid(i + 1, j, k))
	{
		tempBlock = Chunk_[i + 1][j][k];

		canSee[ID] = !tempBlock.IsSolid();
	}
	else
	{
		canSee[ID] = true;
	}
	ID++;

	// Forward
	if (BlockIsValid(i, j, k + 1))
	{
		tempBlock = Chunk_[i][j][k + 1];
		canSee[ID] = !tempBlock.IsSolid();
	}
	else
	{
		canSee[ID] = true;
	}
	ID++;

	// Backward
	if (BlockIsValid(i, j, k - 1))
	{
		tempBlock = Chunk_[i][j][k - 1];
		canSee[ID] = !tempBlock.IsSolid();
	}
	else
	{
		canSee[ID] = true;
	}
	ID++;
	
	//=====================
	// Set Face Visibility
	//=====================

	for (int id = 0; id < 6; id++)
	{
		Chunk_[i][j][k].GetModel()->GetMesh(id)->SetActive(canSee[id]);
	}
	//Chunk_[i][j][k].GetModel()->GetMesh(0)->SetActive(true);
}

bool Chunk::BlockIsEmpty(int i, int j, int k)
{
	if (BlockIsValid(i, j, k))
	{
		return !Chunk_[i][j][k].IsSolid();
	}

	return true;
}

bool Chunk::BlockIsValid(int i, int j, int k)
{
	bool xCheck, yCheck, zCheck;

	xCheck = RangeCheck(i, 0, BLOCK_COUNT_WIDTH - 1);
	yCheck = RangeCheck(j, 0, BLOCK_COUNT_HEIGHT - 1);
	zCheck = RangeCheck(k, 0, BLOCK_COUNT_DEPTH - 1);

	return xCheck && yCheck && zCheck;
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