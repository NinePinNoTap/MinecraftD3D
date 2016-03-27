#pragma once

#include <string>
#include <map>

#include "AssetManager.h"
#include "GameObject.h"
#include "Mesh3D.h"
#include "Material.h"
#include "Singleton.h"
#include "TXTLoader.h"

class BlockManager : public Singleton <BlockManager>
{
public:
	BlockManager()
	{

	}
	~BlockManager()
	{

	}

	GameObject* GetBlock(string blockname)
	{
		// Check if the font already exists
		if (BlockDatabase_.count(blockname))
		{
			// Don't continue
			return BlockDatabase_[blockname];
		}

		GameObject* loadedObj;
		Model* loadedModel;

		bool Result;
		Result = AssetManager::Instance()->LoadModel(*loadedModel, "cube.txt", "Sand.dds");
		if (!Result)
		{
			return 0;
		}

		loadedObj->Initialise();
		loadedObj->SetModel(loadedModel);

		return loadedObj;
	}

private:
	std::map<string, GameObject*> BlockDatabase_;
};