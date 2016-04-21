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
	bool Render();

	// Setter
	void ToggleTime(bool);

private:
	bool RenderMeshes();
	bool SendModelToPipeline(Mesh3D* objMesh);

	// Properties
	float WaterTranslation_;
	float WaveSpeed_;
};


