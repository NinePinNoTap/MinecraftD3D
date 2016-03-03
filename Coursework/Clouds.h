#pragma once

#include <d3d11.h>
#include <d3dx10math.h>
#include <vector>

using namespace std;

#include "Primitive.h"
#include "PrimitiveMesh.h"
#include "TextureArray.h"
#include "Texture.h"


class Clouds : public Primitive
{
public:
	Clouds();
	Clouds(const Clouds&);
	~Clouds();

	// Initialising
	bool Initialise(WCHAR* cloudTextureFilename, WCHAR* perturbTextureFilename);

	// Shutdown
	void Shutdown();

	// Frame
	bool Frame();

	// Getters
	float GetScale();
	float GetBrightness();
	float GetTranslation();

private:
	// Textures
	bool LoadTextures(WCHAR* textureFilename1, WCHAR* textureFilename2);

	// Properties
	float ScaleFactor_;
	float Brightness_;
	float Translation_;
	float PlaneWidth_;
	float MaxHeight_;

	// Vertex Data
	int QuadCount_;
	int TextureRepeat_;
};


