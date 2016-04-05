#pragma once

#include <string>

#include "Block.h"
#include "BlockData.h"
#include "Model.h"
#include "Material.h"
#include "Singleton.h"

class BlockManager : public Singleton <BlockManager>
{
public:
	BlockManager();
	~BlockManager();

	void Initialise();

	void LoadBlock(Model** model, string blockName);

	BlockData GetData(string blockName);

private:
	std::map<string, BlockData> BlockDatabase_;
};