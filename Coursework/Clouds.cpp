#include "Clouds.h"
#include "DirectXManager.h"
#include "ShaderManager.h"

Clouds::Clouds() : GameObject()
{
}

Clouds::Clouds(const Clouds& other)
{
}

Clouds::~Clouds()
{
}

// set up
bool Clouds::initialise(string cloudTextureFilename, string perturbTextureFilename)
{
	PrimitiveFactory primitiveFactory;

	// set Properties
	m_scaleFactor = 0.3f;
	m_brightness = 0.5f;

	//==============
	// create Model
	//==============

	m_model = new Model;
	if (!m_model)
	{
		return false;
	}

	// onload Model
	m_result = primitiveFactory.createSkyPlane(100, 100.0f, 0.5f, 20, *m_model);
	if (!m_result)
	{
		return false;
	}

	// create the material
	Material* newMaterial = new Material;
	m_result = newMaterial->setTexture("BaseTexture", cloudTextureFilename);
	if (!m_result)
	{
		return false;
	}
	m_result = newMaterial->setTexture("PerturbTexture", perturbTextureFilename);
	if (!m_result)
	{
		return false;
	}

	// set material properties
	newMaterial->setFloat("TextureScale", m_scaleFactor);
	newMaterial->setFloat("TextureBrightness", m_brightness);
	m_model->addMaterial(newMaterial);

	//============
	// set Shader
	//============

	m_shader = ShaderManager::getInstance()->getShader("cloud");
	if (!m_shader)
	{
		return false;
	}

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

	m_frame = 0;

	m_reflective = renderMode::On;
	m_culled = renderMode::Off;
	m_depth = renderMode::Off;
	m_postprocessing = renderMode::On;
	m_blendMode = BlendMode::CloudBlending;

	m_isActive = true;

	return true;
}

// update
bool Clouds::update()
{
	//================
	// Animate Clouds
	//================

	m_frame += 0.0001f;
	if (m_frame > 1.0f)
	{
		m_frame -= 1.0f;
	}

	m_model->getMaterial()->setFloat("update", m_frame);

	//==============
	// Track Camera
	//==============

	m_transform->setPosition(Camera::getInstance()->getTransform()->getPosition());

	return true;
}

bool Clouds::render()
{
	if (!m_isActive || !m_shader || !m_model)
		return true;

	// Define how we want the model to be rendered
	setRenderModes();

	// render Reflection
	if (m_reflective == renderMode::On)
	{
		// Reverse the camera position as depth buffer and camera tracking is on
		float currentY = m_transform->getY();
		D3DXVECTOR3 reflectedCamera = Camera::getInstance()->getTransform()->getPosition();
		reflectedCamera.y *= -1;

		m_transform->setY(reflectedCamera.y);

		// Define how we want to see the model
		m_shader->setRenderMode(ProjectionMode::Perspective, ViewMode::Reflection);

		// get the reflection texture and set it as the render target
		Texture* reflectionTexture;
		AssetManager::getInstance()->loadTexture(&reflectionTexture, "ReflectionTexture");
		reflectionTexture->setRenderTarget();

		// render the model
		renderMeshes();

		// Reset render Target
		DirectXManager::getInstance()->setBackBufferRenderTarget();
		DirectXManager::getInstance()->ResetViewport();

		// Reset Transform
		m_transform->setY(currentY);
	}

	// Define how we want to see the model
	m_shader->setRenderMode(ProjectionMode::Perspective, ViewMode::View);

	// render to render Texture
	if (m_postprocessing == renderMode::On)
	{
		// get and set render texture
		Texture* renderTexture;
		AssetManager::getInstance()->loadTexture(&renderTexture, "renderTexture");
		renderTexture->setRenderTarget();

		// render the model
		renderMeshes();

		// Reset render Target
		DirectXManager::getInstance()->setBackBufferRenderTarget();
		DirectXManager::getInstance()->ResetViewport();

		// Clean Up
		renderTexture = 0;
	}

	// render Mesh
	renderMeshes();

	// Reset Pipeline settings
	resetRenderModes();

	return true;
}

// getters
float Clouds::getScale()
{
	return m_scaleFactor;
}

float Clouds::getBrightness()
{
	return m_brightness;
}
