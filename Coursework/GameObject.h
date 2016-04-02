#pragma once

#include <d3dx11.h>
#include <d3dx10math.h>

#include "Constants.h"
#include "Model.h"
#include "OBJLoader.h"
#include "Transform.h"
#include "Utilities.h"
#include "GameShader.h"

class GameObject
{
public:
	GameObject();
	~GameObject();

	// Initialising
	virtual bool Initialise();
	virtual bool Initialise(const char* filename);
	
	// Shutdown
	virtual void Shutdown();

	// Frames
	virtual bool Frame();
	virtual bool Render();

	// Rendering
	void SetRender(string shaderName);
	void SetReflectable(bool Flag);
	void SetActive(bool Flag);

	// Setters
	void SetModel(Model* model);

	// Getters
	Model* GetModel();
	Transform* GetTransform();

	bool IsReflectable();
	bool IsActive();
	float GetFrame();

protected:
	// Model
	Model* Model_;

	// Shader
	GameShader* Shader_;

	// Transform
	Transform* Transform_;

	// Animations
	float Frame_;
	
	// Flags
	bool Result_;
	bool IsReflectable_;
	bool IsActive_;
};