#pragma once

#include "Block.h"
#include "ShaderManager.h"
#include "Transform.h"
#include "Constants.h"
#include "BlockManager.h"

class Chunk
{
public:
	Chunk() { }
	~Chunk() { }

	void Initialise()
	{
		//
		// Build Blank Chunk
		//

		// Create the 3D array to store blocks in chunk
		Chunk_ = new Block**[BLOCKS_IN_CHUNK];

		// Loop through x dimension
		for (int i = 0; i < BLOCKS_IN_CHUNK; i++)
		{
			Chunk_[i] = new Block*[BLOCKS_IN_CHUNK];

			// Loop through y dimension
			for (int j = 0; j < BLOCKS_IN_CHUNK; j++)
			{
				Chunk_[i][j] = new Block[BLOCKS_IN_CHUNK];

				// Loop through z dimension
				for (int k = 0; k < BLOCKS_IN_CHUNK; k++)
				{
					int r = rand() % 5;

					BlockInfo info;

					if (r <= 1) info = DIRT;
					else if (r == 2) info = SAND;
					else if (r == 3) info = COBBLESTONE;
					else if (r == 4) info = GOLD;

					Chunk_[i][j][k] = Block(info);
				}
			}
		}

		Refresh();

		//==================
		// Create Transform
		//==================

		Transform_ = new Transform;

		//==================
		// Initialise Flags
		//==================

		IsVisible_ = true;
	}

	void Shutdown()
	{
		// Delete the blocks
		for (int i = 0; i < BLOCKS_IN_CHUNK; i++)
		{
			// Loop through y dimension
			for (int j = 0; j < BLOCKS_IN_CHUNK; j++)
			{
				delete[] Chunk_[i][j];
			}

			delete[] Chunk_[i];
		}

		delete[] Chunk_;
	}

#include <string>

	void Refresh()
	{
		int activeBlocks = 0;

		// Loop through x dimension
		for (int i = 0; i < BLOCKS_IN_CHUNK; i++)
		{
			// Loop through y dimension
			for (int j = 0; j < BLOCKS_IN_CHUNK; j++)
			{
				// Loop through z dimension
				for (int k = 0; k < BLOCKS_IN_CHUNK; k++)
				{
					bool xPositive, xNegative;
					bool yPositive, yNegative;
					bool zPositive, zNegative;

					// Check if theres block in that particle position
					xPositive = CheckBlock(i + 1, j, k);
					xNegative = CheckBlock(i - 1, j, k);
					yPositive = CheckBlock(i, j + 1, k);
					yNegative = CheckBlock(i, j - 1, k);
					zPositive = CheckBlock(i, j, k + 1);
					zNegative = CheckBlock(i, j, k - 1);

					bool result = !(xPositive && xNegative && yPositive && yNegative && zPositive && zNegative);
					
					Chunk_[i][j][k].SetActive(result);

					if (!result)
						activeBlocks++;
				}
			}
		}

		int n = (BLOCKS_IN_CHUNK - 2);
		if (n < 0)
			n = 0;
		int total = n * n * n;

		OutputToDebug("Blocks Not Rendered : ");
		OutputToDebug(to_string(activeBlocks));
		OutputToDebug(" in ");
		OutputToDebug(to_string(BLOCKS_IN_CHUNK));
		OutputToDebug(" x ");
		OutputToDebug(to_string(BLOCKS_IN_CHUNK));
		OutputToDebug(" x ");
		OutputToDebug(to_string(BLOCKS_IN_CHUNK));
		OutputToDebug(" | Expected : ");
		OutputToDebug(to_string(total));
		OutputToDebug("\n");
	}

	bool CheckBlock(int x, int y, int z)
	{
		// Check if the block is out of range
		if (x < 0 || x >= BLOCKS_IN_CHUNK)
		{
			return false;
		}
		if (y < 0 || y >= BLOCKS_IN_CHUNK)
		{
			return false;
		}
		if (z < 0 || z >= BLOCKS_IN_CHUNK)
		{
			return false;
		}

		// Check the type of block
		switch (Chunk_[x][y][z].GetInfo().type)
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
	void Update()
	{
		// Check frustrum
		// Set IsActive_ based on frustum state
	}

	void Render()
	{
		if (!IsVisible_)
			return;

		GameObject* block;
		// Loop through x dimension
		for (int i = 0; i < BLOCKS_IN_CHUNK; i++)
		{
			// Loop through y dimension
			for (int j = 0; j < BLOCKS_IN_CHUNK; j++)
			{
				// Loop through z dimension
				for (int k = 0; k < BLOCKS_IN_CHUNK; k++)
				{
					if (Chunk_[i][j][k].GetActive())
					{
						string name = Chunk_[i][j][k].GetInfo().name;
						block = BlockManager::Instance()->GetBlock(name);

						// Get position of block 
						D3DXVECTOR3 position = Transform_->GetPosition();
						position += D3DXVECTOR3(i, j, k) * BLOCK_SIZE * 1.1;

						// Update block position
						block->GetTransform()->SetPosition(position);

						// Render
						ShaderManager::Instance()->TextureRender(block);
					}
				}
			}
		}
	}

	// Getters
	Transform* GetTransform() { return Transform_; }

private:
	Block*** Chunk_;
	bool IsVisible_;
	Transform* Transform_;
};