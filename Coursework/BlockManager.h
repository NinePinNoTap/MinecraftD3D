#pragma once

#include <string>

#include "Block.h"
#include "Model.h"
#include "Material.h"
#include "Singleton.h"

class BlockManager : public Singleton <BlockManager>
{
public:
	BlockManager();
	~BlockManager();

	void Initialise();

	Block GetBlock(string blockName);

private:
	std::map<string, Block> BlockDatabase_;
};