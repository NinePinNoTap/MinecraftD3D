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

void AssetManager::terminate()
{
	// De-allocate all audio clips
	for (std::map<string, AudioClip*>::iterator it = m_audioClips.begin(); it != m_audioClips.end(); ++it)
	{
		if (it->second)
		{
			it->second->terminate();
			it->second = 0;
		}
	}

	// De-allocate all fonts
	for (std::map<string, Font*>::iterator it = m_fonts.begin(); it != m_fonts.end(); ++it)
	{
		if (it->second)
		{
			it->second->terminate();
			it->second = 0;
		}
	}

	// De-allocate all model
	for (std::map<string, Model*>::iterator it = m_models.begin(); it != m_models.end(); ++it)
	{
		if (it->second)
		{
			it->second->terminate();
			it->second = 0;
		}
	}
}

void AssetManager::loadAudio(AudioClip** audioClip, std::string filename, bool is3D)
{
	// Check if the font already exists
	if (m_audioClips.count(filename))
	{
		*audioClip = m_audioClips[filename];

		return;
	}

	// Construct filename path to txt
	std::string audioPath = Directory::Audio + filename;
	const char* audioFilePath = audioPath.c_str();

	// create audio clip
	AudioClip* loadedClip = new AudioClip;
	m_result = loadedClip->loadFile(audioFilePath, is3D);
	if (!m_result)
	{
		outputToDebug("(AUDIO) Could not load : " + filename);
		audioClip = 0;
		return;
	}

	// Add to map
	m_audioClips[filename] = loadedClip;

	*audioClip = loadedClip;

	outputToDebug("(AUDIO) loaded : " + filename);

	return;
}

void AssetManager::loadFont(Font** font, std::string filename, int letterCount)
{
	// Check if the font already exists
	if (m_fonts.count(filename))
	{
		// Don't continue
		*font = m_fonts[filename];
		return;
	}

	//===============
	// onload the Font
	//===============

	// Construct filename path to txt
	std::string txtPath = Directory::Font + filename;
	const char* txtFilePath = txtPath.c_str();

	// create Font
	Font* loadedFont = new Font;
	if (!loadedFont)
	{
		*font = 0;
		return;
	}
	m_result = loadedFont->initialise(txtFilePath, letterCount);
	if (!m_result)
	{
		outputToDebug("(FONT) Could not load : " + filename);
		*font = 0;
		return;
	}

	// Add to map
	m_fonts[filename] = loadedFont;
	
	// set font and return
	*font = loadedFont;

	outputToDebug("(FONT) loaded : " + filename);

	return;
}

void AssetManager::loadModel(Model** model, std::string filename)
{
	// Check if the model already exists
	if (m_models.count(filename))
	{
		vector<Mesh3D*> modelMesh = m_models[filename]->getAllMeshes();
		vector<Material*> modelMaterial = m_models[filename]->getAllMaterials();

		(*model) = new Model;
		
		for (unsigned int i = 0; i < modelMesh.size(); i++)
		{
			// Take a copy of the mesh
			Mesh3D* tempMesh = new Mesh3D(*modelMesh[i]);
			(*model)->addMesh(tempMesh);
		}

		for (unsigned int i = 0; i < modelMaterial.size(); i++)
		{
			(*model)->addMaterial(modelMaterial[i]);
		}

		// Cleanup
		modelMesh.clear();
		modelMaterial.clear();

		return;
	}

	OBJloader objloader;
	TXTloader txtloader;

	// create the Model
	Model* loadedModel = new Model;

	// Check what format we are dealing with
	if (string(filename).find(".txt") != std::string::npos)
	{
		// onload TXT File
		m_result = txtloader.loadModel(filename.c_str(), *loadedModel);
	}
	else
	{
		// onload Obj File
		m_result = objloader.loadModel(filename.c_str(), *loadedModel);
	}

	// Check if we loaded the model properly
	if (m_result)
	{
		*model = loadedModel;

		outputToDebug("(MODEL) loaded : " + filename);

		// Add to the map
		m_models[filename] = loadedModel;
	}
	else
	{
		outputToDebug("(MODEL) Could not load : " + filename);
		*model = 0;
	}

	return;
}

void AssetManager::loadTexture(Texture** texture, std::string filename)
{
	// Check if the font already exists
	if (m_textures.count(filename))
	{
		*texture = m_textures[filename];

		return;
	}

	// create the texture
	Texture* loadedTexture = new Texture;
	m_result = loadedTexture->initialise(Directory::Texture + filename);
	if (!m_result)
	{
		*texture = 0;
		outputToDebug("(TEXTURE) Could not load : " + filename);
		return;
	}

	// Add to the map
	m_textures[filename] = loadedTexture;

	*texture = loadedTexture;

	outputToDebug("(TEXTURE) loaded : " + filename);

	return;
}

void AssetManager::loadTexture(Texture** texture, string keyName, Rect2D textureResolution)
{
	// Check if the texture is in the database
	if (m_textures.count(keyName))
	{
		*texture = m_textures[keyName];
		return;
	}

	// create the texture
	Texture* createdTexture = new Texture;
	if (!createdTexture)
	{
		*texture = 0;
		return;
	}

	// initialise it
	m_result = createdTexture->initialise(textureResolution);
	if (!m_result)
	{
		// Couldn't create
		*texture = 0;
		delete createdTexture;
		createdTexture = 0;
		return;
	}

	// Add to the map
	m_textures[keyName] = createdTexture;

	// Return it
	*texture = createdTexture;

	outputToDebug("(RENDER TEXTURE) created : " + keyName);
}