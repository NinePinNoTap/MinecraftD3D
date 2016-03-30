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

class BlockLoader : public Singleton <BlockLoader>
{
public:
	BlockLoader()
	{

	}
	~BlockLoader()
	{

	}

	GameObject* GetBlock(string blockName)
	{
		GameObject* loadedObj = 0;
		Model* loadedModel;

		// Create the base gameobject
		loadedObj = new GameObject;
		loadedObj->Initialise();

		// Load the model
		AssetManager::Instance()->LoadModel(&loadedModel, "cube.txt");

		// Load the texture
		Material* newMaterial = new Material;
		newMaterial->SetBaseTexture(blockName + ".dds");
		loadedModel->UpdateMaterial(0, newMaterial);

		// Set the model on the gameobject
		loadedObj->SetModel(loadedModel);

		return loadedObj;
	}
};