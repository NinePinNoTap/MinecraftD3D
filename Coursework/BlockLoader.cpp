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

	// Load the model
	AssetManager::Instance()->LoadModel(&loadedModel, "cube.txt");

	// Load the texture
	Material* newMaterial = new Material;
	newMaterial->SetBaseTexture(blockName + ".dds");

	// Apply material to model
	loadedModel->UpdateMaterial(0, newMaterial);

	// Take a copy of the model
	*model = new Model(*loadedModel);
}