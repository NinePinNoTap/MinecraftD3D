#include "SkySphere.h"
#include "ShaderManager.h"

SkySphere::SkySphere() : GameObject()
{
	
}

SkySphere::SkySphere(const SkySphere& other)
{
}

SkySphere::~SkySphere()
{
}

bool SkySphere::initialise(const char* filename)
{
	TXTloader txtloader;

	//==============
	// create Model
	//==============

	// onload Model
	AssetManager::getInstance()->loadModel(&m_model, filename);
	if (!m_model)
	{
		return false;
	}

	// create Material
	m_model->addMaterial(new Material);

	//============
	// set Shader
	//============

	m_shader = ShaderManager::getInstance()->getShader("skysphere");

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
	m_blendMode = BlendMode::NoBlending;

	m_isActive = true;

	// initialise sky color for daytime
	m_colours[0][1] = D3DXVECTOR4(0.65f, 0.77f, 0.85f, 1.0f); //Top
	m_colours[0][0] = D3DXVECTOR4(0.18f, 0.35f, 0.54f, 1.0f); //Center

	// initialise the sky color for nighttime
	m_colours[1][0] = D3DXVECTOR4(0.0f, 0.2f, 0.2f, 1.0f); //Top
	m_colours[1][1] = D3DXVECTOR4(0.2f, 0.4f, 0.4f, 1.0f); //Center

	// initialise the current color for daytime
	m_model->getMaterial()->setVector4("TopColour", m_colours[0][0]);
	m_model->getMaterial()->setVector4("CenterColour", m_colours[0][1]);

	return true;
}

bool SkySphere::update()
{
	//==============
	// Track Camera
	//==============

	m_transform->setPosition(Camera::getInstance()->getTransform()->getPosition());

	return true;
}

bool SkySphere::render()
{
	if (!m_isActive || !m_shader || !m_model)
		return true;

	// Define how we want the model to be rendered
	setrenderModes();

	// render Reflection
	if (m_reflective == renderMode::On)
	{
		// Reverse the camera position as depth buffer and camera tracking is on
		float currentY = m_transform->getY();
		D3DXVECTOR3 reflectedCamera = Camera::getInstance()->getTransform()->getPosition();
		reflectedCamera.y *= -1;

		m_transform->setY(reflectedCamera.y);

		// Define how we want to see the model
		m_shader->setrenderMode(ProjectionMode::Perspective, ViewMode::Reflection);

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
	m_shader->setrenderMode(ProjectionMode::Perspective, ViewMode::View);

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

void SkySphere::toggleTime(bool timeMode)
{
	int ID = (int)timeMode;

	// update Material
	m_model->getMaterial()->setVector4("TopColour", m_colours[ID][0]);
	m_model->getMaterial()->setVector4("CenterColour", m_colours[ID][1]);
}