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
	// De-allocate all audio clips
	/*for (std::map<string, AudioClip*>::iterator it = AudioDatabase_.begin(); it != AudioDatabase_.end(); ++it)
	{
	it->second->Shutdown();
	delete it->second;
	it->second = 0;
	}*/

	// De-allocate all fonts
	for (std::map<string, Font*>::iterator it = FontDatabase_.begin(); it != FontDatabase_.end(); ++it)
	{
		it->second->Shutdown();
		delete it->second;
		it->second = 0;
	}
	// De-allocate all model
	for (std::map<string, Model*>::iterator it = ModelDatabase_.begin(); it != ModelDatabase_.end(); ++it)
	{
		it->second->Shutdown();
		delete it->second;
		it->second = 0;
	}
	// De-allocate all textures
	for (std::map<string, Texture*>::iterator it = TextureDatabase_.begin(); it != TextureDatabase_.end(); ++it)
	{
		it->second->Shutdown();
		delete it->second;
		it->second = 0;
	}
}

void AssetManager::LoadAudio(AudioClip** audioClip, std::string filename, bool is3D)
{
	// Check if the font already exists
	if (AudioDatabase_.count(filename))
	{
		OutputDebugString(L"Already loaded\n");

		*audioClip = AudioDatabase_[filename];

		return;
	}

	// Construct filename path to txt
	std::string audioPath = AUDIO_DIR + filename + ".wav";
	const char* audioFilePath = audioPath.c_str();

	// Create audio clip
	AudioClip* loadedClip = new AudioClip;
	Result_ = loadedClip->LoadFile(audioFilePath, is3D);
	if (!Result_)
	{
		OutputDebugString(L"Could not load audio\n");
		audioClip = 0;
		return;
	}

	// Add to map
	AudioDatabase_[filename] = loadedClip;

	OutputDebugString(L"Added audio\n");

	*audioClip = loadedClip;

	return;
}

void AssetManager::LoadFont(Font** font, std::string filename)
{
	// Check if the font already exists
	if (FontDatabase_.count(filename))
	{
		// Don't continue
		*font = FontDatabase_[filename];
		return;
	}

	// Construct filename path to txt
	std::string txtPath = FONT_DIR + filename + ".txt";
	const char* txtFilePath = txtPath.c_str();

	// Construct filename path to dds
	std::wstring ddsPath = L"Data/Font/";
	ddsPath += std::wstring(filename.begin(), filename.end());
	ddsPath += L".dds";
	WCHAR* ddsFilePath = (WCHAR*)ddsPath.c_str();

	// Create Font
	Font* loadedFont = new Font;
	Result_ = loadedFont->Initialise(txtFilePath, ddsFilePath);
	if (!Result_)
	{
		*font = 0;
		return;
	}

	// Add to map
	FontDatabase_[filename] = loadedFont;

	*font = loadedFont;

	return;
}

void AssetManager::LoadModel(Model** model, std::string filename)
{
	// Check if the font already exists
	if (ModelDatabase_.count(filename))
	{
		// Don't continue
		*model = ModelDatabase_[filename];

		return;
	}

	OBJLoader objLoader;

	// Create the Model
	Model* loadedModel = new Model;
	Result_ = loadedModel->Initialise();
	if (!Result_)
	{
		*model = 0;
		return;
	}

	// Load the Model
	Result_ = objLoader.LoadModel(filename.c_str(), *loadedModel);

	// Check if we loaded the model properly
	if (Result_)
	{
		*model = loadedModel;

		// Add to the map
		ModelDatabase_[filename] = loadedModel;
	}
	else
	{
		*model = 0;
	}

	return;
}

void AssetManager::LoadModel(Model** model, std::string filename, wstring textureFilename)
{
	// Check if the font already exists
	if (ModelDatabase_.count(filename))
	{
		// Don't continue
		*model = ModelDatabase_[filename];

		return;
	}

	TXTLoader txtLoader;
	Model* loadedModel;

	// Create the Model
	loadedModel = new Model;
	Result_ = loadedModel->Initialise();
	if (!Result_)
	{
		*model = 0;
		return;
	}

	// Load the Model
	Result_ = txtLoader.LoadModel(filename.c_str(), *loadedModel);

	// Check if we loaded the model properly
	if (!Result_)
	{
		*model = 0;
		return;
	}

	// Load the Texture
	loadedModel->GetMaterial()->SetTextureArray(std::vector<wstring>(1, textureFilename));

	// Return loaded model
	*model = loadedModel;

	// Add to the map
	ModelDatabase_[filename] = loadedModel;

	return;
}

void AssetManager::LoadTexture(Texture** texture, std::string filename)
{
	// Check if the font already exists
	if (TextureDatabase_.count(filename))
	{
		*texture = TextureDatabase_[filename];

		return;
	}

	// Construct filename path to dds
	std::wstring ddsPath = L"Data/Textures/";
	ddsPath += std::wstring(filename.begin(), filename.end());
	ddsPath += L".dds";
	WCHAR* ddsFilePath = (WCHAR*)ddsPath.c_str();

	// Create the texture
	Texture* loadedTexture = new Texture;
	Result_ = loadedTexture->Initialise(ddsFilePath);
	if (!Result_)
	{
		*texture = 0;
	}

	// Add to the map
	TextureDatabase_[filename] = loadedTexture;

	*texture = loadedTexture;

	return;
}