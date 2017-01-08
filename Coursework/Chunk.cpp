#include "Chunk.h"
#include "ViewFrustumManager.h"

Chunk::Chunk()
{
	m_blocks = 0;
}

Chunk::~Chunk()
{
}

void Chunk::initialise(int x, int y, int z)
{
	//==================
	// create Transform
	//==================

	m_position = D3DXVECTOR3(x * World::ChunkSize, y * World::ChunkSize, z * World::ChunkSize);

	//===========================
	// create Representing Block
	//===========================

	m_chunkBlock.initialise("block.obj");
	m_chunkBlock.setShader("instancedlight");
	m_chunkBlock.setrenderModes(renderMode::On, renderMode::On, renderMode::On, BlendMode::NoBlending);

	//================
	// generate Chunk
	//================

	generateBlankChunk();

	//==================
	// initialise Flags
	//==================

	m_visible = true;
	m_generated = false;
	m_dirty = false;
}

void Chunk::terminate()
{
	//// Delete the blocks
	//for (int x = 0; x < World::ChunkSize; x++)
	//{
	//	// Loop through y dimension
	//	for (int y = 0; y < World::ChunkSize; y++)
	//	{
	//		delete[] m_blocks[x][y];
	//	}

	//	delete[] m_blocks[x];
	//}

	//delete[] m_blocks;

	m_chunkBlock.terminate();
}

// update
bool Chunk::update()
{
	float distanceAway;
	
	//==============
	// View Frustum
	//==============

	m_visible = ViewFrustumManager::getInstance()->checkCube(m_position + World::ChunkOffset, World::ChunkSize / 2);
	if (!m_visible)
	{
		return true;
	}

	//=================
	// distance Checks
	//=================

	// Check distance to player
	distanceAway = distance(Camera::getInstance()->getTransform()->getPosition(), m_position);

	// Check if we are out of range
	if (distanceAway >= World::UnloadRadius * World::ChunkSize)
	{
		m_visible = false;
		m_outOfRange = true;
	}
	else
	{
		m_visible = true;
		m_outOfRange = false;
	}

	return true;
}

bool Chunk::render()
{
	if (!m_visible)
	{
		return true;
	}
	
	// render Block
	m_result = m_chunkBlock.render();
	if (!m_result)
	{
		return false;
	}

	return true;
}

// Chunk Generating
void Chunk::generateBlankChunk()
{
	float xPos, yPos, zPos;

	// create the 3D array to store blocks in chunk
	m_blocks = new Block**[World::ChunkSize];

	// Loop through x dimension
	for (int x = 0; x < World::ChunkSize; x++)
	{
		m_blocks[x] = new Block*[World::ChunkSize];

		// Loop through y dimension
		for (int y = 0; y < World::ChunkSize; y++)
		{
			m_blocks[x][y] = new Block[World::ChunkSize];

			// Loop through z dimension
			for (int z = 0; z < World::ChunkSize; z++)
			{
				// Define position of the block
				xPos = m_position.x + x;
				yPos = m_position.y + y;
				zPos = m_position.z + z;

				// Store it
				m_blocks[x][y][z].clone(World::Blocks["air"]);
				m_blocks[x][y][z].setPosition(xPos, yPos, zPos);
			}
		}
	}

	// Loop through x dimension
	for (int x = 0; x < World::ChunkSize; x++)
	{
		// Loop through y dimension
		for (int y = 0; y < World::ChunkSize; y++)
		{
			// Loop through z dimension
			for (int z = 0; z < World::ChunkSize; z++)
			{
				// set block neighbours
				m_blocks[x][y][z].setNeighbour(Direction::Up, getBlock(x, y + 1, z));
				m_blocks[x][y][z].setNeighbour(Direction::Down, getBlock(x, y - 1, z));

				m_blocks[x][y][z].setNeighbour(Direction::Left, getBlock(x - 1, y, z));
				m_blocks[x][y][z].setNeighbour(Direction::Right, getBlock(x + 1, y, z));

				m_blocks[x][y][z].setNeighbour(Direction::Forward, getBlock(x, y, z + 1));
				m_blocks[x][y][z].setNeighbour(Direction::Backward, getBlock(x, y, z - 1));
			}
		}
	}
}

void Chunk::refreshVisible()
{
	D3DXVECTOR3 worldPos;

	// Loop through x dimension
	for (int x = 0; x < World::ChunkSize; x++)
	{
		// Loop through y dimension
		for (int y = 0; y < World::ChunkSize; y++)
		{
			// Loop through z dimension
			for (int z = 0; z < World::ChunkSize; z++)
			{
				worldPos = m_position + D3DXVECTOR3(x, y, z);

				// set block neighbours
				/*m_blocks[x][y][z].setNeighbour(Direction::Up, getBlock(worldPos.x, worldPos.y + 1, worldPos.z));
				m_blocks[x][y][z].setNeighbour(Direction::Down, getBlock(worldPos.x, worldPos.y - 1, worldPos.z));

				m_blocks[x][y][z].setNeighbour(Direction::Left, getBlock(worldPos.x - 1, worldPos.y, worldPos.z));
				m_blocks[x][y][z].setNeighbour(Direction::Right, getBlock(worldPos.x + 1, worldPos.y, worldPos.z));

				m_blocks[x][y][z].setNeighbour(Direction::Forward, getBlock(worldPos.x, worldPos.y, worldPos.z + 1));
				m_blocks[x][y][z].setNeighbour(Direction::Backward, getBlock(worldPos.x, worldPos.y, worldPos.z - 1));*/

				// Refresh the block visibility
				m_blocks[x][y][z].Refresh();

				if (m_blocks[x][y][z].isActive() && m_blocks[x][y][z].isSolid())
				{
					m_chunkBlock.addInstance(m_blocks[x][y][z].getInstance());
				}
			}
		}
	}

	// Compile the instance data to buffer
	m_chunkBlock.rebuildInstanceBuffer();
}

// setters
void Chunk::setBlocks(string blockName)
{
	// Loop through x dimension
	for (int x = 0; x < World::ChunkSize; x++)
	{
		// Loop through y dimension
		for (int y = 0; y < World::ChunkSize; y++)
		{
			// Loop through z dimension
			for (int z = 0; z < World::ChunkSize; z++)
			{
				// Store it
				m_blocks[x][y][z].clone(World::Blocks[blockName]);
			}
		}
	}
}

// getters
inline Block* Chunk::getBlock(int x, int y, int z)
{
	// Check the value is within range
	if (!rangeCheck(x, 0, World::ChunkSize - 1))
	{
		return 0;
	}

	if (!rangeCheck(y, 0, World::ChunkSize - 1))
	{
		return 0;
	}
	if (!rangeCheck(z, 0, World::ChunkSize - 1))
	{
		return 0;
	}

	// Return block
	return &m_blocks[x][y][z];
}

inline void Chunk::setBlock(int x, int y, int z, Block block)
{
	Block* targetBlock;

	// get the block
	targetBlock = getBlock(x, y, z);
	if (targetBlock)
	{
		// update the block
		targetBlock = &block;

		// Flag we need a refresh
		m_dirty = true;
	}

	// Clean Up
	targetBlock = 0;
}