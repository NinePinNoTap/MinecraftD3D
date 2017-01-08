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
	bool initialise(string textureFilename, Rect3D WaterResolution);

	// terminate
	void terminate();

	// update
	bool update();
	bool render();

	// setter
	void toggleTime(bool);

private:
	bool renderMeshes();
	bool sendModelToPipeline(Mesh3D* objMesh);

	// Properties
	float m_waterTranslation;
	float m_waveSpeed;
};


