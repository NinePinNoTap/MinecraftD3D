#pragma once

#include <vector>

#include "DirectXManager.h"
#include "Camera.h"
#include "Light.h"
#include "Utilities.h"
#include "Texture.h"
#include "ShaderManager.h"
#include "AudioClip.h"
#include "Text.h"

class Scene3D
{
public:
	Scene3D();
	~Scene3D();

	// Shutdown
	virtual void Shutdown() = 0;
	void ShutdownGameObjects();

	// Frame
	virtual bool Frame() = 0;
	virtual void Reset() = 0;
	
protected:
	virtual void GenerateMatrices() = 0;
	virtual void SetShaderManagerVars() = 0;
	void SetShaderManager2DVars();

	// Matrices
	D3DXMATRIX WorldMatrix_;
	D3DXMATRIX ViewMatrix_;
	D3DXMATRIX ProjectionMatrix_;
	D3DXMATRIX OrthoMatrix_;
	D3DXMATRIX BaseViewMatrix_;
	D3DXMATRIX ReflectionViewMatrix_;

	// Objects
	Texture* SceneTexture_;
	Text* Text_;

	// Flags
	bool Result_;

	// Rendering
	vector<GameObject*> Objects_;
};


