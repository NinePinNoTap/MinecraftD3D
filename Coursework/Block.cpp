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

void Block::SetNeighbour(Direction direction, Block* block)
{
	switch (direction)
	{
		case Direction::Up:
			BlockUp_ = block;
			break;

		case Direction::Down:
			BlockDown_ = block;
			break;

		case Direction::Left:
			BlockLeft_ = block;
			break;

		case Direction::Right:
			BlockRight_ = block;
			break;

		case Direction::Forward:
			BlockForward_ = block;
			break;

		case Direction::Backward:
			BlockForward_ = block;
			break;
	}
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