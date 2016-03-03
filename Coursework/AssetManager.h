#pragma once

#include <string>
#include <map>

#include "Singleton.h"
#include "Font.h"
#include "ModelMesh.h"
#include "Sound3D.h"
#include "Texture.h"

class AssetManager
{
public:
	AssetManager();
	AssetManager(const AssetManager&);
	~AssetManager();

	void Initialise();
	void Shutdown();

	void LoadAudio(std::string filename);
	void LoadFont(std::string filename);
	void LoadModel(std::string filename);
	void LoadTexture(std::string filename);

	Font* GetFont(std::string filename);
	Texture* GetTexture(std::string filename);

private:
	std::map<std::string, Font*> FontDB_;
	std::map<std::string, Texture*> TextureDB_;
	std::map<std::string, ModelMesh*> ModelDB_;
};