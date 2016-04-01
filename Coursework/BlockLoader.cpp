#include "BlockLoader.h"
#include "AssetManager.h"

BlockLoader::BlockLoader()
{

}

BlockLoader::~BlockLoader()
{

}

void BlockLoader::LoadBlock(Model** model, string blockName)
{
	Model* loadedModel = 0;

	//==================
	// Create the Model
	//==================

	AssetManager::Instance()->LoadModel(&loadedModel, "cube.txt");

	//=====================
	// Create the Material
	//=====================

	// Create a material for the model
	Material* newMaterial = new Material;
	newMaterial->SetTexture("BaseTexture", blockName + ".dds");

	// Remove any materials from the model
	loadedModel->ClearMaterials();

	// Apply material to model
	loadedModel->AddMaterial(newMaterial);

	//================
	// Finalise Model
	//================

	*model = new Model(*loadedModel);
}