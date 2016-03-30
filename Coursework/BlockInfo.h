#pragma once

#include <string>
using namespace std;

enum BlockType
{
	Air, Sand, Dirt, Cobblestone, Gold
};

struct BlockInfo
{
	BlockInfo()
	{
		name = "Air";
		type = BlockType::Air;
		solid = false;
	}

	BlockInfo(string n, BlockType t, bool s)
	{
		name = n;
		type = t;
		solid = s;
	}

	string name;
	BlockType type;
	bool solid;
};