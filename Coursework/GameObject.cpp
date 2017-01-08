#include "GameObject.h"
#include "ShaderManager.h"

GameObject::GameObject()
{
	// initialise the pointers
	m_model = 0;
	m_shader = 0;
	m_transform = 0;

	// initialise default flags
	m_isActive = true;
}

GameObject::~GameObject()
{
}

bool GameObject::initialise()
{
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

	m_reflective = renderMode::Off;
	m_culled = renderMode::Off;
	m_depth = renderMode::On;
	m_postprocessing = renderMode::On;
	m_blendMode = BlendMode::NoBlending;

	m_isActive = true;

	return true;
}

bool GameObject::initialise(const char* filename)
{
	OBJloader objloader;

	//==============
	// create Model
	//==============

	AssetManager::getInstance()->loadModel(&m_model, filename);
	if (!m_model)
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

	m_reflective = renderMode::Off;
	m_culled = renderMode::Off;
	m_depth = renderMode::On;
	m_postprocessing = renderMode::On;
	m_blendMode = BlendMode::NoBlending;

	m_isActive = true;
	
	return true;
}

bool GameObject::initialise(const char* filename, string textureFilename)
{
	Material* createdMaterial;

	// Call parent function
	m_result = GameObject::initialise(filename);
	if (!m_result)
	{
		return false;
	}

	// create a Material
	createdMaterial = new Material;
	if (!createdMaterial)
	{
		return false;
	}
	m_result = createdMaterial->setBaseTexture(textureFilename);
	if (!m_result)
	{
		return false;
	}
	m_model->addMaterial(createdMaterial);

	// Clean Up
	createdMaterial = 0;

	return true;
}

// terminate
void GameObject::terminate()
{
	// Delete the transform
	if (m_transform)
	{
		m_transform = 0;
	}
}

// update
bool GameObject::update()
{
	return true;
}

bool GameObject::render()
{
	if (!m_isActive || !m_shader || !m_model)
		return true;

	// Define how we want the model to be rendered
	setrenderModes();

	// render Reflection
	if (m_reflective == renderMode::On)
	{
		// Define how we want to see the model
		m_shader->setrenderMode(ProjectionMode::Perspective, ViewMode::Reflection);

		// get and set the reflection texture
		Texture* reflectionTexture;
		AssetManager::getInstance()->loadTexture(&reflectionTexture, "ReflectionTexture");
		reflectionTexture->setRenderTarget();

		// render Model
		renderMeshes();

		DirectXManager::getInstance()->setBackBufferRenderTarget();
		DirectXManager::getInstance()->ResetViewport();

		// Clean Up
		reflectionTexture = 0;
	}

	// Define how we want to see the model
	m_shader->setrenderMode(ProjectionMode::Perspective, ViewMode::View);
	
	if (m_postprocessing == renderMode::On)
	{
		// get and set the reflection texture
		Texture* renderTexture;
		AssetManager::getInstance()->loadTexture(&renderTexture, "renderTexture");
		renderTexture->setRenderTarget();

		renderMeshes();

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

bool GameObject::renderMeshes()
{
	// render the model
	for (int i = 0; i < m_model->getMeshCount(); i++)
	{
		// Make sure the mesh is active for it to be rendered
		if (m_model->getMesh(i)->isActive())
		{
			// Send model to pipeline
			sendModelToPipeline(m_model->getMesh(i));

			// Send material to shader
			m_shader->prepare(m_model->getMaterial(i), m_transform);

			// render Object
			m_shader->render(m_model->getMesh(i)->getIndexCount());
		}
	}

	return true;
}

bool GameObject::sendModelToPipeline(Mesh3D* objMesh)
{
	unsigned int stride;
	unsigned int offset;
	ID3D11Buffer* vertexBuffer;
	ID3D11Buffer* indexBuffer;

	// set vertex buffer stride and offset
	stride = sizeof(VertexData);
	offset = 0;

	// get mesh buffers
	vertexBuffer = objMesh->getVertexBuffer();
	indexBuffer = objMesh->getIndexBuffer();

	// set the vertex buffer to active in the input assembler so it can be rendered
	DirectXManager::getInstance()->getDeviceContext()->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);

	// set the index buffer to active in the input assembler so it can be rendered
	DirectXManager::getInstance()->getDeviceContext()->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	// set the type of primitive that should be rendered from this vertex buffer
	DirectXManager::getInstance()->getDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	vertexBuffer = 0;
	indexBuffer = 0;

	return true;
}

void GameObject::setrenderModes()
{
	// Backface Culling
	if (m_culled == renderMode::On)
	{
		DirectXManager::getInstance()->setBackfaceCullingOn(true);
	}
	else
	{
		DirectXManager::getInstance()->setBackfaceCullingOn(false);
	}

	// Z Buffer
	if (m_depth == renderMode::On)
	{
		DirectXManager::getInstance()->setDepthBufferOn(true);
	}
	else
	{
		DirectXManager::getInstance()->setDepthBufferOn(false);
	}
	
	// Blending
	switch (m_blendMode)
	{
		case BlendMode::NoBlending:
			DirectXManager::getInstance()->setAlphaBlendingOn(false);
			break;

		case BlendMode::AlphaBlending:
			DirectXManager::getInstance()->setAlphaBlendingOn(true);
			break;

		case BlendMode::CloudBlending:
			DirectXManager::getInstance()->setCloudBlendingOn();
			break;

		case BlendMode::AlphaMasked:
			DirectXManager::getInstance()->setAlphaMaskingOn();
			break;
	}
}

void GameObject::resetRenderModes()
{
	DirectXManager::getInstance()->setAlphaBlendingOn(false);
	DirectXManager::getInstance()->setDepthBufferOn(true);
	DirectXManager::getInstance()->setBackfaceCullingOn(false);
}

// setters
void GameObject::setShader(string shaderName)
{
	m_shader = ShaderManager::getInstance()->getShader(shaderName);

	if (!m_shader)
	{
		outputToDebug("Could not set shader");
	}
}

void GameObject::setrenderModes(renderMode canReflect, renderMode useCulling, renderMode useDepth, BlendMode blendMode)
{
	m_reflective = canReflect;
	m_culled = useCulling;
	m_depth = useDepth;
	m_blendMode = blendMode;
}

void GameObject::setReflectionMode(renderMode canReflect)
{
	m_reflective = canReflect;
}

void GameObject::setCullingMode(renderMode useCulling)
{
	m_culled = useCulling;
}

void GameObject::setDepthMode(renderMode useDepth)
{
	m_depth = useDepth;
}

void GameObject::setBlendMode(BlendMode blendMode)
{
	m_blendMode = blendMode;
}

void GameObject::setActive(bool Flag)
{
	m_isActive = Flag;
}

void GameObject::setModel(Model* model)
{
	m_model = model;
}

// getters
Model* GameObject::getModel()
{
	return m_model;
}

Transform* GameObject::getTransform()
{	
	return m_transform;
}

bool GameObject::isActive()
{
	return m_isActive;
}

float GameObject::getFrame()
{
	return m_frame;
}