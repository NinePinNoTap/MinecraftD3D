#include "Block.h"
#include "BlockLoader.h"

Block::Block() : GameObject()
{
	BlockName_ = "air";
	Type_ = BlockType::Air;
	IsSolid_ = false;
}

Block::~Block()
{
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
	BlockLoader::Instance()->LoadBlock(&Model_, BlockName_);
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

void Block::SetFaceVisible(int index, bool flag)
{
	VisibleFace_[index] = flag;
}

// Getters
BlockType Block::GetType()
{
	return Type_;
}

bool Block::GetSolid()
{
	return IsSolid_;
}