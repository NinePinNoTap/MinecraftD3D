#pragma once

#include <string>

#include "Model.h"
#include "Material.h"
#include "Singleton.h"

class BlockLoader : public Singleton <BlockLoader>
{
public:
	BlockLoader();
	~BlockLoader();

	void LoadBlock(Model** model, string blockName);
};