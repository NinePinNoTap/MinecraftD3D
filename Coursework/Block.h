#pragma once

#include <string>

#include "BlockInfo.h"
#include "Constants.h"

using namespace std;

class Block
{
public:

	// Basic block
	Block()
	{
		Info_ = AIR;
		IsActive_ = true;
	}

	// Custom Block
	Block(string name, BlockType type, bool solid)
	{
		Info_.name = name;
		Info_.type = type;
		Info_.solid = solid;
		IsActive_ = true;
	}

	// Pre-defined block
	Block(BlockInfo info)
	{
		Info_ = info;
		IsActive_ = true;
	}

	~Block()
	{
	}

	// Setters
	void SetName(string name) { Info_.name = name; }
	void SetType(BlockType type) { Info_.type = type; }
	void SetActive(bool flag) { IsActive_ = flag; }

	// Getters
	BlockInfo GetInfo() { return Info_; }
	bool GetActive() { return IsActive_; }

private:
	BlockInfo Info_;
	bool IsActive_;
};