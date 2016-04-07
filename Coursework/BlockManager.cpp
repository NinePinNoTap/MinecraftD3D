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
	BlockDatabase_["air"] = BlockData("air", BlockType::Air, false, D3DXVECTOR2(0, 0));
	BlockDatabase_["dirt"] = BlockData("dirt", BlockType::Dirt, false, D3DXVECTOR2(0, 0));
	BlockDatabase_["sand"] = BlockData("sand", BlockType::Sand, false, D3DXVECTOR2(0, 0));
	BlockDatabase_["gravel"] = BlockData("gravel", BlockType::Gravel, false, D3DXVECTOR2(0, 0));
	BlockDatabase_["water"] = BlockData("water", BlockType::Water, false, D3DXVECTOR2(0, 0));
	BlockDatabase_["lava"] = BlockData("lava", BlockType::Lava, false, D3DXVECTOR2(0, 0));
	BlockDatabase_["cobblestone"] = BlockData("cobblestone", BlockType::Cobblestone, false, D3DXVECTOR2(0, 0));
	BlockDatabase_["stone"] = BlockData("stone", BlockType::Stone, false, D3DXVECTOR2(0, 0));
	BlockDatabase_["gold"] = BlockData("gold", BlockType::Stone, false, D3DXVECTOR2(0, 0));
	BlockDatabase_["diamond"] = BlockData("diamond", BlockType::Stone, false, D3DXVECTOR2(0, 0));
}

void BlockManager::LoadBlock(Model** model, string blockName)
{
	Model* loadedModel = 0;

	//==================
	// Create the Model
	//==================

	/*AssetManager::Instance()->LoadModel(&loadedModel, "block.obj");
	
	//=====================
	// Create the Material
	//=====================

	// Create a material for the model
	Material* newMaterial = new Material;
	newMaterial->SetBaseTexture(blockName + ".dds");

	// Remove any materials from the model
	loadedModel->ClearMaterials();

	// Apply material to model
	loadedModel->AddMaterial(newMaterial);*/

	//================
	// Finalise Model
	//================

	//*model = new Model(*loadedModel);
}

BlockData BlockManager::GetData(string blockName)
{
	return BlockDatabase_[blockName];
}