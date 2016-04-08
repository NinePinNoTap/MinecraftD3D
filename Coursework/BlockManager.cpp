#include "BlockManager.h"
#include "AssetManager.h"

BlockManager::BlockManager()
{

}

BlockManager::~BlockManager()
{

}

void BlockManager::Initialise()
{
	// Non-Solid
	BlockDatabase_["air"] =			Block("air",		 BlockType::Air,		D3DXVECTOR2(0, 0),	D3DXVECTOR2(4,3), false);

	// Solid
	BlockDatabase_["dirt"] =		Block("dirt",		 BlockType::Dirt,		 D3DXVECTOR2(0, 0), D3DXVECTOR2(4,3), true);
	BlockDatabase_["sand"] =		Block("sand",		 BlockType::Sand,		 D3DXVECTOR2(1, 0), D3DXVECTOR2(4,3), true);
	BlockDatabase_["wood"] =		Block("wood",		 BlockType::Wood,		 D3DXVECTOR2(3, 0), D3DXVECTOR2(4,3), true);
	BlockDatabase_["cobblestone"] = Block("cobblestone", BlockType::Cobblestone, D3DXVECTOR2(0, 1), D3DXVECTOR2(4,3), true);
	BlockDatabase_["stone"] =		Block("stone",		 BlockType::Stone,		 D3DXVECTOR2(1, 1), D3DXVECTOR2(4,3), true);
	BlockDatabase_["gravel"] =		Block("gravel",		 BlockType::Gravel,		 D3DXVECTOR2(1, 2), D3DXVECTOR2(4,3), true);
	BlockDatabase_["water"] =		Block("water",		 BlockType::Water,		 D3DXVECTOR2(0, 0), D3DXVECTOR2(4,3), true);
	BlockDatabase_["lava"] =		Block("lava",		 BlockType::Lava,		 D3DXVECTOR2(0, 0), D3DXVECTOR2(4,3), true);
	BlockDatabase_["gold"] =		Block("gold",		 BlockType::Gold,		 D3DXVECTOR2(0, 2), D3DXVECTOR2(4,3), true);
	BlockDatabase_["diamond"] =		Block("diamond",	 BlockType::Diamond,	 D3DXVECTOR2(1, 2), D3DXVECTOR2(4,3), true);
}

Block BlockManager::GetBlock(string blockName)
{
	return BlockDatabase_[blockName];
}