#include "Block.h"
#include "BlockManager.h"

Block::Block() : GameObject()
{
	BlockName_ = "air";
	Type_ = BlockType::Air;
	IsSolid_ = false;

	for (int i = 0; i < 6; i++)
	{
		NeighbourBlocks_[i] = 0;
	}
}

Block::~Block()
{
}

// Frame
bool Block::Frame()
{
	//================================
	// Check if we need to be updated
	//================================

	if (Type_ == BlockType::Air)
	{
		return true;
	}

	//=========================
	// Handle frustum checking
	//=========================

	IsActive_ = ViewFrustumManager::Instance()->CheckCube(Transform_->GetPosition(), BLOCK_SIZE * 0.7);
	if (!IsActive_)
	{
		return true;
	}

	//===================
	// Handle Neighbours
	//===================

	HandleNeighbours();

	return true;
}

#include "Camera.h"
void Block::HandleNeighbours()
{
	bool showFace;
	int visibleFaces = 0;

	// Handle face visibility
	for (int i = 0; i < NO_OF_NEIGHBOURS; i++)
	{
		showFace = true;

		// Check if we can see the neighbour
		if (CheckNeighbour(i))
		{
			showFace = false;
		}
		else
		{
			visibleFaces++;
		}

		Model_->GetMesh(i)->SetActive(showFace);
	}

	// Disable entire object if we cant see it
	if (visibleFaces == NO_OF_NEIGHBOURS)
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
bool Block::SetName(string blockName)
{
	// Check if we are updating to a new block
	if (BlockName_ == blockName)
	{
		return true;
	}

	// Store the block name
	BlockName_ = blockName;

	// Load the new block
	BlockManager::Instance()->LoadBlock(&Model_, BlockName_);
	if (!Model_)
	{
		return false;
	}

	return true;
}

void Block::SetType(BlockType type)
{
	Type_ = type;
}

void Block::SetSolid(bool isSolid)
{
	IsSolid_ = isSolid;
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

bool Block::IsSolid()
{
	return IsSolid_;
}