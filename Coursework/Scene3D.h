#pragma once

#include <vector>

#include "DirectXManager.h"
#include "Camera.h"
#include "Light.h"
#include "Utilities.h"
#include "Texture.h"
#include "ShaderManager.h"
#include "Sound3D.h"
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

	// Model Loading
	void SetDirectory(string directory);

	// Sound Loading
	Sound3D* CreateSound(char* filename, float Volume = 1.0f, bool is3D = false, D3DXVECTOR3 position = DEFAULT);
	
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
	Camera* Camera_;
	Light* Light_;
	Texture* SceneTexture_;
	Text* Text_;

	// Model Loading
	string FileDirectory_;

	// Flags
	bool Result_;

	// Rendering
	vector<GameObject*> Objects_;
};


