#pragma once

#include <d3dx11.h>
#include <d3dx10math.h>


#include "Model.h"
#include "OBJLoader.h"
#include "Transform.h"
#include "Utilities.h"
#include "GameShader.h"

enum RenderMode { On, Off };
enum BlendMode { NoBlending, AlphaBlending, AlphaMasked, CloudBlending };

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
	void SetShader(string shaderName);
	void SetRenderModes(RenderMode canReflect, RenderMode useCulling, RenderMode useDepth, BlendMode blendMode);
	void SetActive(bool Flag);

	// Setters
	void SetModel(Model* model);

	// Getters
	Model* GetModel();
	Transform* GetTransform();

	bool IsActive();
	float GetFrame();

protected:
	virtual bool RenderMeshes();
	bool SendModelToPipeline(Mesh3D* objMesh);

	void SetRenderModes();
	void ResetRenderModes();

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
	bool IsActive_;
	RenderMode IsReflective_;
	RenderMode UseCulling_;
	RenderMode UseDepth_;
	RenderMode IsPostProcessed_;
	BlendMode BlendMode_;
};