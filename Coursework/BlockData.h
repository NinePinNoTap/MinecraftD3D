#pragma once

#include <string>

enum BlockType
{
	Air,
	Dirt, Sand, Gravel,
	Water, Lava,
	Cobblestone, Stone,
	Gold, Diamond
};

struct BlockData
{
	BlockData() { }
	BlockData(string name, BlockType type, bool solid, D3DXVECTOR2 offset)
	{
		blockName = name;
		blockType = type;
		textureOffset = offset;
		isSolid = solid;
	}

	string blockName;
	BlockType blockType;
	D3DXVECTOR2 textureOffset;
	bool isSolid;
};
