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

void AssetManager::Shutdown()
{
	// De-allocate all audio clips
	for (std::map<string, AudioClip*>::iterator it = AudioDatabase_.begin(); it != AudioDatabase_.end(); ++it)
	{
		if (it->second)
		{
			it->second->Shutdown();
			it->second = 0;
		}
	}

	// De-allocate all fonts
	for (std::map<string, Font*>::iterator it = FontDatabase_.begin(); it != FontDatabase_.end(); ++it)
	{
		if (it->second)
		{
			it->second->Shutdown();
			it->second = 0;
		}
	}

	// De-allocate all model
	for (std::map<string, Model*>::iterator it = ModelDatabase_.begin(); it != ModelDatabase_.end(); ++it)
	{
		if (it->second)
		{
			it->second->Shutdown();
			it->second = 0;
		}
	}
}

void AssetManager::LoadAudio(AudioClip** audioClip, std::string filename, bool is3D)
{
	// Check if the font already exists
	if (AudioDatabase_.count(filename))
	{
		*audioClip = AudioDatabase_[filename];

		return;
	}

	// Construct filename path to txt
	std::string audioPath = Config::Directory::Audio + filename;
	const char* audioFilePath = audioPath.c_str();

	// Create audio clip
	AudioClip* loadedClip = new AudioClip;
	Result_ = loadedClip->LoadFile(audioFilePath, is3D);
	if (!Result_)
	{
		OutputToDebug("Could not load (AUDIO) : " + filename);
		audioClip = 0;
		return;
	}

	// Add to map
	AudioDatabase_[filename] = loadedClip;

	*audioClip = loadedClip;

	OutputToDebug("Loaded (AUDIO) : " + filename);

	return;
}

void AssetManager::LoadFont(Font** font, std::string filename, int letterCount)
{
	// Check if the font already exists
	if (FontDatabase_.count(filename))
	{
		// Don't continue
		*font = FontDatabase_[filename];
		return;
	}

	//===============
	// Load the Font
	//===============

	// Construct filename path to txt
	std::string txtPath = Config::Directory::Font + filename;
	const char* txtFilePath = txtPath.c_str();

	// Create Font
	Font* loadedFont = new Font;
	if (!loadedFont)
	{
		*font = 0;
		return;
	}
	Result_ = loadedFont->Initialise(txtFilePath, letterCount);
	if (!Result_)
	{
		OutputToDebug("Could not load (FONT) : " + filename);
		*font = 0;
		return;
	}

	// Add to map
	FontDatabase_[filename] = loadedFont;
	
	// Set font and return
	*font = loadedFont;

	OutputToDebug("Loaded (FONT) : " + filename);

	return;
}

void AssetManager::LoadModel(Model** model, std::string filename)
{
	// Check if the font already exists
	if (ModelDatabase_.count(filename))
	{
		vector<Mesh3D*> modelMesh = ModelDatabase_[filename]->GetAllMeshes();
		vector<Material*> modelMaterial = ModelDatabase_[filename]->GetAllMaterials();

		(*model) = new Model;
		
		for (int i = 0; i < modelMesh.size(); i++)
		{
			// Take a copy of the mesh
			Mesh3D* tempMesh = new Mesh3D(*modelMesh[i]);
			(*model)->AddMesh(tempMesh);
		}

		for (int i = 0; i < modelMaterial.size(); i++)
		{
			(*model)->AddMaterial(modelMaterial[i]);
		}

		// Cleanup
		modelMesh.clear();
		modelMaterial.clear();

		return;
	}

	OBJLoader objLoader;
	TXTLoader txtLoader;

	// Create the Model
	Model* loadedModel = new Model;

	// Check what format we are dealing with
	if (string(filename).find(".txt") != std::string::npos)
	{
		// Load TXT File
		Result_ = txtLoader.LoadModel(filename.c_str(), *loadedModel);
	}
	else
	{
		// Load Obj File
		Result_ = objLoader.LoadModel(filename.c_str(), *loadedModel);
	}

	// Check if we loaded the model properly
	if (Result_)
	{
		*model = loadedModel;

		OutputToDebug("Loaded (MODEL) : " + filename);

		// Add to the map
		ModelDatabase_[filename] = loadedModel;
	}
	else
	{
		OutputToDebug("Could not load (MODEL) : " + filename);
		*model = 0;
	}

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

	// Create the texture
	Texture* loadedTexture = new Texture;
	Result_ = loadedTexture->Initialise(Config::Directory::Texture + filename);
	if (!Result_)
	{
		*texture = 0;
		OutputToDebug("Could not load (TEXTURE) : " + filename);
		return;
	}

	// Add to the map
	TextureDatabase_[filename] = loadedTexture;

	*texture = loadedTexture;

	OutputToDebug("Loaded (TEXTURE) : " + filename);

	return;
}

void AssetManager::LoadTexture(Texture** texture, string keyName, Rect2D textureResolution)
{
	// Check if the texture is in the database
	if (TextureDatabase_.count(keyName))
	{
		*texture = TextureDatabase_[keyName];
		return;
	}

	// Create the texture
	Texture* createdTexture = new Texture;
	if (!createdTexture)
	{
		*texture = 0;
		return;
	}

	// Initialise it
	Result_ = createdTexture->Initialise(textureResolution);
	if (!Result_)
	{
		// Couldn't create
		*texture = 0;
		delete createdTexture;
		createdTexture = 0;
		return;
	}

	// Add to the map
	TextureDatabase_[keyName] = createdTexture;

	// Return it
	*texture = createdTexture;

	OutputToDebug("Created (TEXTURE) : " + keyName);
}