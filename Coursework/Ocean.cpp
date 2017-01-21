#include "Ocean.h"
#include "AssetManager.h"

Ocean::Ocean() : GameObject()
{
}

Ocean::Ocean(const Ocean& other)
{
}

Ocean::~Ocean()
{
}

// Initialising
bool Ocean::initialise(string textureFilename, Rect3D waterResolution)
{
	PrimitiveFactory primitiveFactory;
	Texture* refractionTexture;
	Texture* reflectionTexture;
	
	//==============
	// create Model
	//==============

	m_model = new Model;
	if (!m_model)
	{
		return false;
	}

	// onload Model
	m_result = primitiveFactory.createPlane(waterResolution, Rect3D(1, 1), 1.0f, *m_model);
	if (!m_result)
	{
		return false;
	}

	//=================
	// create Material
	//=================

	// Retrieve refraction and reflection texture pointers
	AssetManager::getInstance()->loadTexture(&refractionTexture, "RefractionTexture");
	AssetManager::getInstance()->loadTexture(&reflectionTexture, "ReflectionTexture");

	// create material
	Material* newMaterial = new Material;
	m_result = newMaterial->setNormalTexture(textureFilename);
	if (!m_result)
	{
		return false;
	}
	newMaterial->setTexture("RefractionTexture", refractionTexture);
	newMaterial->setTexture("ReflectionTexture", reflectionTexture);

	newMaterial->setFloat("WaterHeight", waterResolution.depth);
	newMaterial->setVector2("NormalMapTiling", D3DXVECTOR2(0.01f, 0.02f));
	newMaterial->setFloat("WaterTranslation", 0.0f);
	newMaterial->setFloat("ReflectRefractScale", 0.03f);
	newMaterial->setVector4("RefractionTint", D3DXVECTOR4(0.0f, 0.8f, 1.0f, 1.0f));
	newMaterial->setFloat("WaveHeight", 1.5f);
	newMaterial->setFloat("TessellationAmount", 58.0f);

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

	m_frame = 0.0f;
	m_waveSpeed = 0.025f;
	m_waterTranslation = 0.0f;

	m_reflective = renderMode::Off;
	m_culled = renderMode::Off;
	m_depth = renderMode::On;
	m_postprocessing = renderMode::On;
	m_blendMode = BlendMode::NoBlending;

	m_isActive = true;

	setShader("ocean");

	// Clean Up
	newMaterial = 0;
	refractionTexture = 0;
	reflectionTexture = 0;

	return true;
}

// terminate
void Ocean::terminate()
{
	// Release the texture object
	if (m_model)
	{
		m_model->terminate();
		delete m_model;
		m_model = 0;
	}

	if (m_transform)
	{
		delete m_transform;
		m_transform = 0;
	}

	return;
}

// update
bool Ocean::update()
{
	// update the position of the water to simulate motion
	m_waterTranslation += 0.003f;
	wrap(m_waterTranslation, 0, 1.0f);

	// update the waater time to simulate waves
	m_frame += m_waveSpeed;

	// update Material
	m_model->getMaterial()->setFloat("update", m_frame);
	m_model->getMaterial()->setFloat("WaterTranslation", m_waterTranslation);

	return true;
}

bool Ocean::render()
{
	if (!m_isActive || !m_shader || !m_model)
		return true;

	// Define how we want the model to be rendered
	setRenderModes();

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

bool Ocean::renderMeshes()
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

bool Ocean::sendModelToPipeline(Mesh3D* objMesh)
{
	unsigned int stride;
	unsigned int offset;
	ID3D11Buffer* vertexBuffer;
	ID3D11Buffer* indexBuffer;

	// set vertex buffer stride and offset
	stride = sizeof(VertexData);
	offset = 0;

	// get Mesh Buffers
	vertexBuffer = objMesh->getVertexBuffer();
	indexBuffer = objMesh->getIndexBuffer();

	// set the vertex buffer to active in the InputManager assembler so it can be rendered
	DirectXManager::getInstance()->getDeviceContext()->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);

	// set the index buffer to active in the InputManager assembler so it can be rendered
	DirectXManager::getInstance()->getDeviceContext()->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	// set the type of primitive that should be rendered from this vertex buffer
	DirectXManager::getInstance()->getDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_4_CONTROL_POINT_PATCHLIST);

	return true;
}

// Game settings
void Ocean::toggleTime(bool NightTimeMode)
{
	// Toggle between good weather and bad weather
	if (NightTimeMode)
	{
		m_waveSpeed = 0.06f;
		m_model->getMaterial()->setFloat("WaveHeight", 2.0f);
	}
	else
	{
		m_model->getMaterial()->setFloat("WaveHeight", 1.5f);
		m_waveSpeed = 0.025f;
	}
}