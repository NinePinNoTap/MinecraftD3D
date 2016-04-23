#include "ShaderManager.h"

ShaderManager::ShaderManager()
{
	// Initialise pointers to 0
	CloudShader_ = 0;
	ColourShader_ = 0;
	FireShader_ = 0;
	FontShader_ = 0;
	InstancedLightShader_ = 0;
	InstancedTextureShader_ = 0;
	LightShader_ = 0;
	OceanShader_ = 0;
	SkySphereShader_ = 0;
	TerrainShader_ = 0;
	TerrainReflectionShader_ = 0;
	TextureShader_ = 0;
}

ShaderManager::ShaderManager(const ShaderManager& other)
{
}

ShaderManager::~ShaderManager()
{
}

// Initialising
bool ShaderManager::Initialise(HWND hwnd)
{
	//=============================
	// Initialise the Cloud Shader
	//=============================

	CloudShader_ = new CloudShader;
	if (!CloudShader_)
	{
		return false;
	}
	Result_ = CloudShader_->Initialise(hwnd);
	if (!Result_)
	{
		return false;
	}

	//==============================
	// Initialise the Colour Shader
	//==============================

	ColourShader_ = new ColourShader;
	if (!ColourShader_)
	{
		return false;
	}
	Result_ = ColourShader_->Initialise(hwnd);
	if (!Result_)
	{
		return false;
	}

	//============================
	// Initialise the Fire Shader
	//============================

	FireShader_ = new FireShader;
	if (!FireShader_)
	{
		return false;
	}
	Result_ = FireShader_->Initialise(hwnd);
	if (!Result_)
	{
		return false;
	}

	//============================
	// Initialise the Font Shader
	//============================

	FontShader_ = new FontShader;
	if (!FontShader_)
	{
		return false;
	}
	Result_ = FontShader_->Initialise(hwnd);
	if (!Result_)
	{
		return false;
	}

	//=======================================
	// Initialise the Instanced Light Shader
	//=======================================

	InstancedLightShader_ = new InstancedLightShader;
	if (!InstancedLightShader_)
	{
		return false;
	}
	Result_ = InstancedLightShader_->Initialise(hwnd);
	if (!Result_)
	{
		return false;
	}

	//=========================================
	// Initialise the Instanced Texture Shader
	//=========================================

	InstancedTextureShader_ = new InstancedTextureShader;
	if (!InstancedTextureShader_)
	{
		return false;
	}
	Result_ = InstancedTextureShader_->Initialise(hwnd);
	if (!Result_)
	{
		return false;
	}

	//=============================
	// Initialise the Light Shader
	//=============================

	LightShader_ = new LightShader;
	if (!LightShader_)
	{
		return false;
	}
	Result_ = LightShader_->Initialise(hwnd);
	if (!Result_)
	{
		return false;
	}

	//=============================
	// Initialise the Ocean Shader
	//=============================

	OceanShader_ = new OceanShader;
	if (!OceanShader_)
	{
		return false;
	}
	Result_ = OceanShader_->Initialise(hwnd);
	if (!Result_)
	{
		return false;
	}

	//==================================
	// Initialise the SkySphere Shader
	//==================================

	SkySphereShader_ = new SkyShader;
	if (!SkySphereShader_)
	{
		return false;
	}
	Result_ = SkySphereShader_->Initialise(hwnd);
	if (!Result_)
	{
		return false;
	}

	//===============================
	// Initialise the Terrain Shader
	//===============================

	TerrainShader_ = new TerrainShader;
	if (!TerrainShader_)
	{
		return false;
	}
	Result_ = TerrainShader_->Initialise(hwnd);
	if (!Result_)
	{
		return false;
	}

	//==========================================
	// Initialise the Terrain Reflection Shader
	//==========================================

	TerrainReflectionShader_ = new TerrainReflectionShader;
	if (!TerrainReflectionShader_)
	{
		return false;
	}
	Result_ = TerrainReflectionShader_->Initialise(hwnd);
	if (!Result_)
	{
		return false;
	}

	//===============================
	// Initialise the Texture Shader
	//===============================

	TextureShader_ = new TextureShader;
	if (!TextureShader_)
	{
		return false;
	}
	Result_ = TextureShader_->Initialise(hwnd);
	if (!Result_)
	{
		return false;
	}

	//============================
	// Initialise the Tint Shader
	//============================

	TintShader_ = new TintShader;
	if (!TintShader_)
	{
		return false;
	}
	Result_ = TintShader_->Initialise(hwnd);
	if (!Result_)
	{
		return false;
	}

	//=======================
	// Setup Shader Database
	//=======================

	ShaderDatabase_["cloud"] = CloudShader_;
	ShaderDatabase_["colour"] = ColourShader_;
	ShaderDatabase_["font"] = FontShader_;
	ShaderDatabase_["instancedlight"] = InstancedLightShader_;
	ShaderDatabase_["instancedtexture"] = InstancedTextureShader_;
	ShaderDatabase_["ocean"] = OceanShader_;
	ShaderDatabase_["light"] = LightShader_;
	ShaderDatabase_["skysphere"] = SkySphereShader_;
	ShaderDatabase_["texture"] = TextureShader_;
	ShaderDatabase_["terrain"] = TerrainShader_;
	ShaderDatabase_["terrainreflection"] = TerrainReflectionShader_;
	ShaderDatabase_["tint"] = TintShader_;

	return true;
}

// Shutdown
void ShaderManager::Shutdown()
{
	//==================
	// Shutdown Shaders
	//==================

	if (ColourShader_)
	{
		ColourShader_->Shutdown();
		delete ColourShader_;
		ColourShader_ = 0;
	}

	if (CloudShader_)
	{
		CloudShader_->Shutdown();
		delete CloudShader_;
		CloudShader_ = 0;
	}

	if (FireShader_)
	{
		FireShader_->Shutdown();
		delete FireShader_;
		FireShader_ = 0;
	}

	if (FontShader_)
	{
		FontShader_->Shutdown();
		delete FontShader_;
		FontShader_ = 0;
	}

	if (LightShader_)
	{
		LightShader_->Shutdown();
		delete LightShader_;
		LightShader_ = 0;
	}

	if (OceanShader_)
	{
		OceanShader_->Shutdown();
		delete OceanShader_;
		OceanShader_ = 0;
	}

	if (SkySphereShader_)
	{
		SkySphereShader_->Shutdown();
		delete SkySphereShader_;
		SkySphereShader_ = 0;
	}

	if (TerrainShader_)
	{
		TerrainShader_->Shutdown();
		delete TerrainShader_;
		TerrainShader_ = 0;
	}

	if (TerrainReflectionShader_)
	{
		TerrainReflectionShader_->Shutdown();
		delete TerrainReflectionShader_;
		TerrainReflectionShader_ = 0;
	}

	if (TextureShader_)
	{
		TextureShader_->Shutdown();
		delete TextureShader_;
		TextureShader_ = 0;
	}
}

bool ShaderManager::TextureRender(ParticleSystem* gameObject)
{
	//Mesh3D* objMesh;
	//Material* objMaterial;

	//// Model Properties
	//objMesh = gameObject->GetModel()->GetMesh();
	//if (!objMesh)
	//{
	//	MessageBox(NULL, L"No Model Attached - Texture", L"Error", MB_OK);
	//	return false;
	//}
	//objMaterial = gameObject->GetModel()->GetMaterial();
	//if (!objMaterial)
	//{
	//	MessageBox(NULL, L"No Material Attached - Texture", L"Error", MB_OK);
	//	return false;
	//}

	//// Model Properties
	//int indexCount = objMesh->GetIndexCount();
	//ID3D11ShaderResourceView* texture = objMaterial->GetBaseTexture();

	//// Create matrix buffer
	//MatrixBuffer matrixBuffer = MatrixBuffer_;;
	//gameObject->GetTransform()->GetTranslationMatrix(matrixBuffer.world);
	//TransposeMatrix(matrixBuffer);

	//// Update Buffers
	//TextureShader_->UpdateBuffer(VertexShader, 0, matrixBuffer);
	//TextureShader_->SendBuffersToShader();

	//// Send Textures
	//TextureShader_->SendTextureToShader(0, texture);

	//// Render using shader
	//gameObject->Render();
	//TextureShader_->Render(indexCount);

	return true;
}
