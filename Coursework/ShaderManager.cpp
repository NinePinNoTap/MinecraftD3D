#include "ShaderManager.h"

ShaderManager::ShaderManager()
{
	// initialise pointers to 0
	m_cloudShader = 0;
	m_colourShader = 0;
	m_fireShader = 0;
	m_fontShader = 0;
	m_instancedLightShader = 0;
	m_instancedTextureShader = 0;
	m_lightShader = 0;
	m_oceanShader = 0;
	m_skySphereShader = 0;
	m_terrainShader = 0;
	m_terrainReflectionShader = 0;
	m_textureShader = 0;
}

ShaderManager::ShaderManager(const ShaderManager& other)
{
}

ShaderManager::~ShaderManager()
{
}

// Initialising
bool ShaderManager::initialise(HWND hwnd)
{
	//=============================
	// initialise the Cloud Shader
	//=============================

	m_cloudShader = new CloudShader;
	if (!m_cloudShader)
	{
		return false;
	}
	m_result = m_cloudShader->initialise(hwnd);
	if (!m_result)
	{
		return false;
	}

	//==============================
	// initialise the Colour Shader
	//==============================

	m_colourShader = new ColourShader;
	if (!m_colourShader)
	{
		return false;
	}
	m_result = m_colourShader->initialise(hwnd);
	if (!m_result)
	{
		return false;
	}

	//============================
	// initialise the Fire Shader
	//============================

	m_fireShader = new FireShader;
	if (!m_fireShader)
	{
		return false;
	}
	m_result = m_fireShader->initialise(hwnd);
	if (!m_result)
	{
		return false;
	}

	//============================
	// initialise the Font Shader
	//============================

	m_fontShader = new FontShader;
	if (!m_fontShader)
	{
		return false;
	}
	m_result = m_fontShader->initialise(hwnd);
	if (!m_result)
	{
		return false;
	}

	//=======================================
	// initialise the Instanced Light Shader
	//=======================================

	m_instancedLightShader = new InstancedLightShader;
	if (!m_instancedLightShader)
	{
		return false;
	}
	m_result = m_instancedLightShader->initialise(hwnd);
	if (!m_result)
	{
		return false;
	}

	//=========================================
	// initialise the Instanced Texture Shader
	//=========================================

	m_instancedTextureShader = new InstancedTextureShader;
	if (!m_instancedTextureShader)
	{
		return false;
	}
	m_result = m_instancedTextureShader->initialise(hwnd);
	if (!m_result)
	{
		return false;
	}

	//=============================
	// initialise the Light Shader
	//=============================

	m_lightShader = new LightShader;
	if (!m_lightShader)
	{
		return false;
	}
	m_result = m_lightShader->initialise(hwnd);
	if (!m_result)
	{
		return false;
	}

	//=============================
	// initialise the Ocean Shader
	//=============================

	m_oceanShader = new OceanShader;
	if (!m_oceanShader)
	{
		return false;
	}
	m_result = m_oceanShader->initialise(hwnd);
	if (!m_result)
	{
		return false;
	}

	//==================================
	// initialise the SkySphere Shader
	//==================================

	m_skySphereShader = new SkyShader;
	if (!m_skySphereShader)
	{
		return false;
	}
	m_result = m_skySphereShader->initialise(hwnd);
	if (!m_result)
	{
		return false;
	}

	//===============================
	// initialise the Terrain Shader
	//===============================

	m_terrainShader = new TerrainShader;
	if (!m_terrainShader)
	{
		return false;
	}
	m_result = m_terrainShader->initialise(hwnd);
	if (!m_result)
	{
		return false;
	}

	//==========================================
	// initialise the Terrain Reflection Shader
	//==========================================

	m_terrainReflectionShader = new TerrainReflectionShader;
	if (!m_terrainReflectionShader)
	{
		return false;
	}
	m_result = m_terrainReflectionShader->initialise(hwnd);
	if (!m_result)
	{
		return false;
	}

	//===============================
	// initialise the Texture Shader
	//===============================

	m_textureShader = new TextureShader;
	if (!m_textureShader)
	{
		return false;
	}
	m_result = m_textureShader->initialise(hwnd);
	if (!m_result)
	{
		return false;
	}

	//============================
	// initialise the Tint Shader
	//============================

	m_TintShader = new TintShader;
	if (!m_TintShader)
	{
		return false;
	}
	m_result = m_TintShader->initialise(hwnd);
	if (!m_result)
	{
		return false;
	}

	//=======================
	// setup Shader Database
	//=======================

	m_shaders["cloud"] = m_cloudShader;
	m_shaders["colour"] = m_colourShader;
	m_shaders["font"] = m_fontShader;
	m_shaders["instancedlight"] = m_instancedLightShader;
	m_shaders["instancedtexture"] = m_instancedTextureShader;
	m_shaders["ocean"] = m_oceanShader;
	m_shaders["light"] = m_lightShader;
	m_shaders["skysphere"] = m_skySphereShader;
	m_shaders["texture"] = m_textureShader;
	m_shaders["terrain"] = m_terrainShader;
	m_shaders["terrainreflection"] = m_terrainReflectionShader;
	m_shaders["tint"] = m_TintShader;

	return true;
}

// terminate
void ShaderManager::terminate()
{
	//==================
	// terminate Shaders
	//==================

	if (m_colourShader)
	{
		m_colourShader->terminate();
		delete m_colourShader;
		m_colourShader = 0;
	}

	if (m_cloudShader)
	{
		m_cloudShader->terminate();
		delete m_cloudShader;
		m_cloudShader = 0;
	}

	if (m_fireShader)
	{
		m_fireShader->terminate();
		delete m_fireShader;
		m_fireShader = 0;
	}

	if (m_fontShader)
	{
		m_fontShader->terminate();
		delete m_fontShader;
		m_fontShader = 0;
	}

	if (m_lightShader)
	{
		m_lightShader->terminate();
		delete m_lightShader;
		m_lightShader = 0;
	}

	if (m_oceanShader)
	{
		m_oceanShader->terminate();
		delete m_oceanShader;
		m_oceanShader = 0;
	}

	if (m_skySphereShader)
	{
		m_skySphereShader->terminate();
		delete m_skySphereShader;
		m_skySphereShader = 0;
	}

	if (m_terrainShader)
	{
		m_terrainShader->terminate();
		delete m_terrainShader;
		m_terrainShader = 0;
	}

	if (m_terrainReflectionShader)
	{
		m_terrainReflectionShader->terminate();
		delete m_terrainReflectionShader;
		m_terrainReflectionShader = 0;
	}

	if (m_textureShader)
	{
		m_textureShader->terminate();
		delete m_textureShader;
		m_textureShader = 0;
	}
}

bool ShaderManager::ParticleRender(ParticleSystem* gameObject)
{
	//Mesh3D* objMesh;
	//Material* objMaterial;

	//// Model Properties
	//objMesh = gameObject->getModel()->getMesh();
	//if (!objMesh)
	//{
	//	MessageBox(NULL, L"No Model Attached - Texture", L"Error", MB_OK);
	//	return false;
	//}
	//objMaterial = gameObject->getModel()->getMaterial();
	//if (!objMaterial)
	//{
	//	MessageBox(NULL, L"No Material Attached - Texture", L"Error", MB_OK);
	//	return false;
	//}

	//// Model Properties
	//int indexCount = objMesh->getIndexCount();
	//ID3D11ShaderResourceView* texture = objMaterial->getBaseTexture();

	//// create matrix buffer
	//MatrixBuffer matrixBuffer = m_matrixBuffer;;
	//gameObject->getTransform()->getTranslationMatrix(matrixBuffer.world);
	//transposeMatrixBuffer(matrixBuffer);

	//// update Buffers
	//m_textureShader->updateBuffer(VertexShader, 0, matrixBuffer);
	//m_textureShader->sendBuffersToShader();

	//// Send Textures
	//m_textureShader->sendTextureToShader(0, texture);

	//// render using shader
	//gameObject->render();
	//m_textureShader->render(indexCount);

	return true;
}
