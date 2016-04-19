#include "Block.h"
#include "BlockManager.h"

Block::Block()
{
	BlockName_ = "null";
	BlockType_ = BlockType::None;
	TextureOffset_ = D3DXVECTOR2(0, 0);
	TotalTextures_ = D3DXVECTOR2(16, 16);
	IsSolid_ = false;
	IsActive_ = false;

	for (int i = 0; i < 6; i++)
	{
		NeighbourBlocks_[i] = 0;
	}
}

Block::Block(string blockName, BlockType blockType, D3DXVECTOR2 textureOffset, D3DXVECTOR2 totalTextures, bool isSolid)
{
	BlockName_ = blockName;
	BlockType_ = blockType;
	TextureOffset_ = textureOffset;
	TotalTextures_ = totalTextures;
	IsSolid_ = isSolid;
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

	if (BlockType_ == BlockType::Air)
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
	BlockName_ = block.BlockName_;
	BlockType_ = block.BlockType_;
	TextureOffset_ = block.TextureOffset_;
	TotalTextures_ = block.TotalTextures_;
	IsSolid_ = block.IsSolid_;
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
	return BlockType_;
}

InstanceData Block::GetInstance()
{
	InstanceData instanceData;
	instanceData.position = Position_;
	instanceData.textureOffset = TextureOffset_;
	instanceData.textureTotal = TotalTextures_;

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