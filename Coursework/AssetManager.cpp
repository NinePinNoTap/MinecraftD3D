#include "AssetManager.h"


AssetManager::AssetManager()
{

}
AssetManager::AssetManager(const AssetManager&)
{

}
AssetManager::~AssetManager()
{

}

void AssetManager::Initialise()
{

}
void AssetManager::Shutdown()
{
	// De-allocate all fonts
	for (std::map<string, Font*>::iterator it = FontDB_.begin(); it != FontDB_.end(); ++it)
	{
		it->second->Shutdown();
		delete it->second;
		it->second = 0;
	}
}

void AssetManager::LoadAudio(std::string filename)
{

}

void AssetManager::LoadFont(std::string filename)
{
	// Check if the font already exists
	if (FontDB_.count(filename))
	{
		// Don't continue
		return;
	}

	// Construct filename path to txt
	std::string txtPath = "Data/Font/";
	txtPath += filename;
	txtPath += ".txt";
	const char* txtFilePath = txtPath.c_str();

	// Construct filename path to dds
	std::wstring ddsPath = L"Data/Font/";
	ddsPath += std::wstring(filename.begin(), filename.end());
	ddsPath += L".dds";
	WCHAR* ddsFilePath = (WCHAR*)ddsPath.c_str();

	Font* font = new Font;
	font->Initialise(txtFilePath, ddsFilePath);

	FontDB_[filename] = font;
}

void AssetManager::LoadModel(std::string filename)
{
	// Check if the font already exists
	if (ModelDB_.count(filename))
	{
		// Don't continue
		return;
	}

	// Construct filename path to dds
	/*std::wstring ddsPath = L"Data/Models/";
	ddsPath += std::wstring(filename.begin(), filename.end());
	ddsPath += L".obj";
	WCHAR* objFilePath = (WCHAR*)ddsPath.c_str();

	// Create the mesh and load in the file
	Mesh3D* mesh = new Mesh3D;
	mesh->LoadModel("Data/Models/", filename.c_str());
	mesh->
	Texture* texture = new Texture;
	texture->Initialise(objFilePath);*/

	// Add to the map
	//ModelDB_[filename] = mesh;
}

void AssetManager::LoadTexture(std::string filename)
{
	// Check if the font already exists
	if (TextureDB_.count(filename))
	{
		// Don't continue
		return;
	}

	// Construct filename path to dds
	std::wstring ddsPath = L"Data/Textures/";
	ddsPath += std::wstring(filename.begin(), filename.end());
	ddsPath += L".dds";
	WCHAR* ddsFilePath = (WCHAR*)ddsPath.c_str();

	// Create the texture
	Texture* texture = new Texture;
	texture->Initialise(ddsFilePath);

	// Add to the map
	TextureDB_[filename] = texture;
}

Font* AssetManager::GetFont(std::string filename)
{
	return FontDB_[filename];
}

Texture* AssetManager::GetTexture(std::string filename)
{
	return TextureDB_[filename];
}