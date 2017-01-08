#include "Block.h"
#include "Config.h"

using namespace Config;

Block::Block()
{
	m_blockName = "null";
	m_blockType = BlockType::None;
	m_textureOffset = D3DXVECTOR2(0, 0);
	m_totalTextures = D3DXVECTOR2(16, 16);
	m_isSolid = false;
	m_isActive = false;

	for (int i = 0; i < 6; i++)
	{
		m_neighbours[i] = 0;
	}
}

Block::Block(string blockName, BlockType blockType, D3DXVECTOR2 textureOffset, D3DXVECTOR2 totalTextures, bool isSolid)
{
	m_blockName = blockName;
	m_blockType = blockType;
	m_textureOffset = textureOffset;
	m_totalTextures = totalTextures;
	m_isSolid = isSolid;
	m_isActive = true;

	for (int i = 0; i < 6; i++)
	{
		m_neighbours[i] = 0;
	}
}

Block::~Block()
{
}

// update
void Block::Refresh()
{
	//================================
	// Check if we need to be updated
	//================================

	if (m_blockType == BlockType::Air)
	{
		m_isActive = false;
		return;
	}

	//===================
	// handle Neighbours
	//===================

	handleNeighbours();
}

void Block::clone(Block& block)
{
	// Copy non-static data
	m_blockName = block.m_blockName;
	m_blockType = block.m_blockType;
	m_textureOffset = block.m_textureOffset;
	m_totalTextures = block.m_totalTextures;
	m_isSolid = block.m_isSolid;
	m_isActive = true;
}

void Block::handleNeighbours()
{
	int visibleSides = 0;

	// Count how many neighbours we can see
	for (int i = 0; i < 6; i++)
	{
		// Check if we can see the neighbour
		if (!checkNeighbour(i))
		{
			visibleSides++;
		}
	}

	// Disable entire object if we cant see it
	if (visibleSides == 0)
	{
		m_isActive = false;
	}
}

bool Block::checkNeighbour(int i)
{
	// Make sure we have a neighbour in this slot
	if (m_neighbours[i] == 0)
	{
		// No neighbour
		return false;
	}

	// Check if the block is there and solid
	if (m_neighbours[i]->isSolid())
	{
		return true;
	}

	return false;
}

// setters
void Block::setPosition(float x, float y, float z)
{
	m_position = D3DXVECTOR3(x, y, z);

	m_boundingBox = BoundingBox(Rect3D(World::BlockSize), m_position);
}

void Block::setNeighbour(Direction direction, Block* block)
{
	m_neighbours[direction] = block;
}

// getters
InstanceData Block::getInstance()
{
	InstanceData instanceData;
	instanceData.position = m_position;
	instanceData.textureOffset = m_textureOffset;
	instanceData.textureTotal = m_totalTextures;

	return instanceData;
}