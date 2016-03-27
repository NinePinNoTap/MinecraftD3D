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
					Chunk_[i][j][k] = Block();
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
		int NoOfBlocks = 0;

		// Loop through x dimension
		for (int i = 0; i < BLOCKS_IN_CHUNK; i++)
		{
			// Loop through y dimension
			for (int j = 0; j < BLOCKS_IN_CHUNK; j++)
			{
				// Loop through z dimension
				for (int k = 0; k < BLOCKS_IN_CHUNK; k++)
				{
					bool xPos, xNeg;
					bool yPos, yNeg;
					bool zPos, zNeg;

					xPos = CheckBlock(i + 1, j, k);
					xNeg = CheckBlock(i - 1, j, k);
					yPos = CheckBlock(i, j + 1, k);
					yNeg = CheckBlock(i, j - 1, k);
					zPos = CheckBlock(i, j, k + 1);
					zNeg = CheckBlock(i, j, k - 1);

					bool result = xPos && xNeg && yPos && yNeg && zPos && zNeg;
					
					Chunk_[i][j][k].SetActive(result);

					if (result)
						NoOfBlocks++;
				}
			}
		}

		OutputToDebug(to_string(NoOfBlocks));
		OutputToDebug("\n");
	}

	bool CheckBlock(int x, int y, int z)
	{
		if (x < 0 || x >= BLOCKS_IN_CHUNK)
			return false;
		if (y < 0 || y >= BLOCKS_IN_CHUNK)
			return false;
		if (z < 0 || z >= BLOCKS_IN_CHUNK)
			return false;

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
						GameObject* block = BlockManager::Instance()->GetBlock("Sand");

						D3DXVECTOR3 position = Transform_->GetPosition();
						position += D3DXVECTOR3(i, j, k);

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