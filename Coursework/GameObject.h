#pragma once

#include <d3dx11.h>
#include <d3dx10math.h>


#include "Model.h"
#include "OBJloader.h"
#include "Transform.h"
#include "Utilities.h"
#include "GameShader.h"

enum renderMode { On, Off };
enum BlendMode { NoBlending, AlphaBlending, AlphaMasked, CloudBlending };

class GameObject
{
public:
	GameObject();
	~GameObject();

	// Initialising
	virtual bool initialise();
	virtual bool initialise(const char* filename);
	virtual bool initialise(const char* filename, string textureFilename);
	
	// terminate
	virtual void terminate();

	// Frames
	virtual bool update();
	virtual bool render();

	// rendering
	void setShader(string shaderName);
	void setrenderModes(renderMode canReflect, renderMode useCulling, renderMode useDepth, BlendMode blendMode);
	void setReflectionMode(renderMode canReflect);
	void setCullingMode(renderMode useCulling);
	void setDepthMode(renderMode useDepth);
	void setBlendMode(BlendMode blendMode);
	void setActive(bool Flag);

	// setters
	void setModel(Model* model);

	// getters
	Model* getModel();
	Transform* getTransform();

	bool isActive();
	float getFrame();

protected:
	virtual bool renderMeshes();
	bool sendModelToPipeline(Mesh3D* objMesh);

	void setrenderModes();
	void resetRenderModes();

	// Model
	Model* m_model;

	// Shader
	GameShader* m_shader;

	// Transform
	Transform* m_transform;

	// Animations
	float m_frame;
	
	// Flags
	bool m_result;
	bool m_isActive;
	renderMode m_reflective;
	renderMode m_culled;
	renderMode m_depth;
	renderMode m_postprocessing;
	BlendMode m_blendMode;
};