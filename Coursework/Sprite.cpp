#include "Sprite.h"

Sprite::Sprite() : GameObject()
{
}

Sprite::Sprite(const Sprite& other)
{
}

Sprite::~Sprite()
{
}

bool Sprite::initialise(Rect3D spriteResolution, string textureFilename)
{
	PrimitiveFactory primitiveFactory;

	//==============
	// create Model
	//==============

	m_model = new Model;
	if (!m_model)
	{
		return false;
	}

	// onload Model
	m_result = primitiveFactory.create2DBox(spriteResolution, *m_model);
	if (!m_result)
	{
		return false;
	}

	//=================
	// create Material
	//=================

	Material* newMaterial = new Material;
	newMaterial->setBaseTexture(textureFilename);
	m_model->addMaterial(newMaterial);

	//==================
	// create Transform
	//==================

	m_transform = new Transform;
	if (!m_transform)
	{
		return false;
	}

	//=================
	// initialise Vars
	//=================
	
	// render Modes
	m_reflective = renderMode::Off;
	m_culled = renderMode::Off;
	m_depth = renderMode::Off;
	m_blendMode = BlendMode::NoBlending;

	// Animations
	m_frame = 0;

	// Flags
	m_isActive = true;

	// Define Base Shader
	setShader("texture");

	// Clean Up
	newMaterial = 0;

	return true;
}

bool Sprite::render()
{
	if (!m_isActive || !m_shader || !m_model)
		return true;

	// Define how we want to see the model
	m_shader->setRenderMode(ProjectionMode::Orthographic, ViewMode::BaseView);

	// Define how we want the model to be rendered
	setRenderModes();

	// render Mesh
	renderMeshes();

	// Reset Pipeline settings
	resetRenderModes();

	return true;
}