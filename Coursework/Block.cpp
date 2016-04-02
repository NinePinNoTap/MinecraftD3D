#include "Block.h"
#include "BlockLoader.h"

Block::Block() : GameObject()
{
	BlockName_ = "air";
	Type_ = BlockType::Air;
	IsSolid_ = false;
}

Block::Block(string blockName, BlockType blockType, bool isSolid) : GameObject()
{
	BlockName_ = blockName;
	Type_ = blockType;
	IsSolid_ = isSolid;

	Initialise();
}

Block::~Block()
{
}

bool Block::Initialise()
{
	//==============
	// Create Model
	//==============

	BlockLoader::Instance()->LoadBlock(&Model_, BlockName_);
	if (!Model_)
	{
		return false;
	}

	//==================
	// Create Transform
	//==================

	Transform_ = new Transform;
	if (!Transform_)
	{
		return false;
	}

	//=================
	// Initialise Vars
	//=================

	Frame_ = 0;
	IsReflectable_ = true;
	IsActive_ = true;
}

// Setters
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