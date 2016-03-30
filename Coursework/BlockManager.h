#pragma once

#include <string>
#include <map>

#include "AssetManager.h"
#include "GameObject.h"
#include "Mesh3D.h"
#include "Model.h"
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

	GameObject* GetBlock(string blockName)
	{
		if (BlockDatabase_.count(blockName))
		{
			return BlockDatabase_[blockName];
		}

		GameObject* loadedObj = 0;
		Model* loadedModel = 0;

		// Create the base gameobject
		loadedObj = new GameObject;
		loadedObj->Initialise();

		// Load the model
		AssetManager::Instance()->LoadModel(&loadedModel, "cube.txt");
		if (!loadedModel)
		{
			return 0;
		}

		// Load the texture
		Material* newMaterial = new Material;
		newMaterial->SetBaseTexture(blockName + ".dds");
		loadedModel->AddMaterial(newMaterial);

		// Set the model on the gameobject
		loadedObj->SetModel(loadedModel);

		BlockDatabase_[blockName] = loadedObj;

		return loadedObj;
	}

private:
	std::map<string, GameObject*> BlockDatabase_;
};