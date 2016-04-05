#pragma once

#include <d3d11.h>
#include <d3dx10math.h>
#include <vector>

#include "DirectXManager.h"
#include "GameObject.h"
#include "Material.h"
#include "PrimitiveFactory.h"
#include "Texture.h"

using namespace std;

class Ocean : public GameObject
{
public:
	Ocean();
	Ocean(const Ocean&);
	~Ocean();

	// Initialising
	bool Initialise(string textureFilename, Rect3D WaterResolution);

	// Shutdown
	void Shutdown();

	// Frame
	bool Frame();

	// Setter
	void ToggleTime(bool);

	// Getters
	float GetTessellation();
	float GetWaterHeight();
	D3DXVECTOR2 GetNormalMapTiling();
	float GetWaterTranslation();
	float GetWaveHeight();
	float GetReflectRefractScale();
	D3DXVECTOR4 GetRefractionTint();
	float GetSpecularShininess();

private:
	bool SendModelToPipeline(Mesh3D* objMesh);

	// Properties
	D3DXVECTOR2 NormalMapTiling_;
	D3DXVECTOR4 RefractionTint_;

	float Tessellation_;
	float WaterHeight_;
	float ReflectRefractScale_;
	float WaterShininess_;
	float WaterTranslation_;
	float WaveSpeed_;
	float WaveHeight_;
};

