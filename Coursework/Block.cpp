#include "Block.h"
#include "BlockManager.h"

Block::Block()
{
	Name_ = "null";
	Type_ = BlockType::None;
	TextureOffset_ = D3DXVECTOR2(0, 0);
	IsSolid_ = false;
	IsActive_ = false;

	for (int i = 0; i < 6; i++)
	{
		NeighbourBlocks_[i] = 0;
	}
}

Block::Block(string name, BlockType type, D3DXVECTOR2 offset, bool solid)
{
	Name_ = name;
	Type_ = type;
	TextureOffset_ = offset;
	IsSolid_ = solid;
	IsActive_ = true;

	for (int i = 0; i < 6; i++)
	{
		NeighbourBlocks_[i] = 0;
	}
}

Block::~Block()
{
}

// Frame
void Block::Refresh()
{
	//================================
	// Check if we need to be updated
	//================================

	if (Type_ == BlockType::Air)
	{
		IsActive_ = false;
		return;
	}

	//=========================
	// Handle frustum checking
	//=========================
	//
	//IsActive_ = ViewFrustumManager::Instance()->CheckCube(Transform_->GetPosition(), BLOCK_SIZE * 0.7);
	//if (!IsActive_)
	//{
	//	return true;
	//}

	//===================
	// Handle Neighbours
	//===================

	HandleNeighbours();
}

void Block::CopyFrom(Block& block)
{
	// Copy non-static data
	Name_ = block.Name_;
	Type_ = block.Type_;
	TextureOffset_ = block.TextureOffset_;
	IsSolid_ = block.Type_;
	IsActive_ = true;
}

void Block::HandleNeighbours()
{
	int visibleSides = 0;

	// Count how many neighbours we can see
	for (int i = 0; i < 6; i++)
	{
		// Check if we can see the neighbour
		if (!CheckNeighbour(i))
		{
			visibleSides++;
		}
	}

	// Disable entire object if we cant see it
	if (visibleSides == 0)
	{
		IsActive_ = false;
	}
}

bool Block::CheckNeighbour(int i)
{
	// Make sure we have a neighbour in this slot
	if (NeighbourBlocks_[i] == 0)
	{
		// No neighbour
		return false;
	}

	// Check if the block is there and solid
	if (NeighbourBlocks_[i]->IsSolid())
	{
		return true;
	}

	return false;
}

// Setters
void Block::SetPosition(float x, float y, float z)
{
	Position_ = D3DXVECTOR3(x, y, z);
}

void Block::SetNeighbour(Direction direction, Block* block)
{
	NeighbourBlocks_[direction] = block;
}

// Getters
BlockType Block::GetType()
{
	return Type_;
}

InstanceData Block::GetInstance()
{
	InstanceData instanceData;
	instanceData.position = Position_;
	instanceData.textureOffset = TextureOffset_;
	instanceData.textureTotal = D3DXVECTOR2(4, 3);

	return instanceData;
}

bool Block::IsSolid()
{
	return IsSolid_;
}

bool Block::IsActive()
{
	return IsActive_;
}