#pragma once

#include <string>
#include <map>
#include <vector>

#include "Singleton.h"
#include "AudioClip.h"
#include "Font.h"
#include "Model.h"
#include "Texture.h"
#include "Rect.h"
#include "OBJloader.h"
#include "TXTloader.h"

using namespace std;

class AssetManager : public Singleton<AssetManager>
{
public:
	AssetManager();
	AssetManager(const AssetManager&);
	~AssetManager();

	// terminate
	void terminate();

	// loaders
	void loadAudio(AudioClip** clip, std::string filename, bool is3D = false);
	void loadFont(Font** font, std::string fontFilename, int letterCount);
	void loadModel(Model** model, std::string filename);
	void loadTexture(Texture** texture, std::string filename);
	void loadTexture(Texture** texture, string keyName, Rect2D textureResolution);

private:
	std::map<std::string, AudioClip*> m_audioClips;
	std::map<std::string, Font*> m_fonts;
	std::map<std::string, Model*> m_models;
	std::map<std::string, Texture*> m_textures;

	bool m_result;
};