#include "ShaderManager.h"

ShaderManager::ShaderManager()
{
	// Initialise pointers to 0
	CloudShader_ = 0;
	ColourShader_ = 0;
	FireShader_ = 0;
	FontShader_ = 0;
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

	//=============================
	// Initialise the lightBuffer Shader
	//=============================

	LightShader_ = new LightShader;
	if(!LightShader_)
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

	OceanShader_ = new GameShader;
	if(!OceanShader_)
	{
		return false;
	}
	//Result_ = OceanShader_->Initialise(hwnd);
	//if (!Result_)
	//{
		//return false;
	//}
	OceanShader_->AddShader("ocean.vs");
	OceanShader_->AddShader("ocean.ps");
	OceanShader_->AddShader("ocean.hs");
	OceanShader_->AddShader("ocean.ds");
	OceanShader_->AddLayout("POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0);
	OceanShader_->AddLayout("TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0);
	OceanShader_->AddLayout("NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0);
	OceanShader_->AddLayout("TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0);
	OceanShader_->AddLayout("BINORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0);
	OceanShader_->AddBuffer<MatrixBuffer>(DomainShader);
	OceanShader_->AddBuffer<CameraBuffer>(DomainShader);
	OceanShader_->AddBuffer<WaveBuffer>(DomainShader);
	OceanShader_->AddBuffer<OceanBuffer>(PixelShader);
	OceanShader_->AddBuffer<TessellationBuffer>(HullShader);
	OceanShader_->AddSamplerState(D3D11_FILTER_MIN_MAG_MIP_LINEAR, D3D11_TEXTURE_ADDRESS_WRAP, 0.0f, 1, D3D11_COMPARISON_ALWAYS, D3DXVECTOR4(0, 0, 0, 0), 0, D3D11_FLOAT32_MAX);
	Result_ = OceanShader_->BuildShader(hwnd);
	if (!Result_)
	{
		OutputToDebug("Could not initialise ocean shader.");
		return false;
	}

	//==================================
	// Initialise the SkySphere Shader
	//==================================

	SkySphereShader_ = new SkyShader;
	if(!SkySphereShader_)
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
	if(!TerrainReflectionShader_)
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
	if(!TextureShader_)
	{
		return false;
	}
	Result_ = TextureShader_->Initialise(hwnd);
	if (!Result_)
	{
		return false;
	}

	//=======================
	// Setup Shader Database
	//=======================

	ShaderDatabase_["colour"] = ColourShader_;
	ShaderDatabase_["cloud"] = CloudShader_;
	ShaderDatabase_["light"] = LightShader_;
	ShaderDatabase_["font"] = FontShader_;
	ShaderDatabase_["skysphere"] = SkySphereShader_;
	ShaderDatabase_["texture"] = TextureShader_;
	ShaderDatabase_["terrain"] = TerrainShader_;
	ShaderDatabase_["terrainreflection"] = TerrainReflectionShader_;

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

// matrixBuffer Setters
void ShaderManager::SetWorldMatrix(D3DXMATRIX world)
{
	MatrixBuffer_.world = world;
}

void ShaderManager::SetViewMatrix(D3DXMATRIX view)
{
	MatrixBuffer_.view = view;
}

void ShaderManager::SetProjectionMatrix(D3DXMATRIX projection)
{
	MatrixBuffer_.projection = projection;
}

void ShaderManager::SetReflectionViewMatrix(D3DXMATRIX reflection)
{
	MatrixBuffer_.reflection = reflection;
}

bool ShaderManager::OceanRender(Ocean* gameObject, Texture* refraction, Texture* reflection)
{
	Mesh3D* objMesh;
	Material* objMaterial;

	// Model Properties
	objMesh = gameObject->GetModel()->GetMesh();
	if (!objMesh)
	{
		MessageBox(NULL, L"No Model Attached - Water", L"Error", MB_OK);
		return false;
	}
	objMaterial = gameObject->GetModel()->GetMaterial();
	if (!objMaterial)
	{
		MessageBox(NULL, L"No Material Attached - Water", L"Error", MB_OK);
		return false;
	}

	// Retrieve variables for rendering
	int indexCount = objMesh -> GetIndexCount();
	ID3D11ShaderResourceView* refractionTexture = refraction -> GetTexture();
	ID3D11ShaderResourceView* reflectionTexture = reflection -> GetTexture();
	ID3D11ShaderResourceView* normalTexture = objMaterial->GetNormalTexture();

	// Create camera buffer
	CameraBuffer cameraBuffer;
	cameraBuffer.cameraPosition = Camera::Instance()->GetTransform()->GetPosition();
	cameraBuffer.normalMapTiling = gameObject -> GetNormalMapTiling();
	cameraBuffer.padding = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	// Create ocean buffer
	OceanBuffer oceanBuffer;
	oceanBuffer.waterTranslation = gameObject->GetWaterTranslation();
	oceanBuffer.reflectRefractScale = gameObject->GetReflectRefractScale();
	oceanBuffer.refractionTint = gameObject->GetRefractionTint();
	oceanBuffer.lightDirection = Light::Instance() -> GetDirection();
	oceanBuffer.specularShininess = gameObject->GetSpecularShininess();
	oceanBuffer.padding = D3DXVECTOR2(0.0f, 0.0f);

	// Create wave buffer
	WaveBuffer waveBuffer;
	waveBuffer.waveTime = gameObject->GetFrame();
	waveBuffer.waveHeight = gameObject->GetWaveHeight() / 2;
	waveBuffer.padding = D3DXVECTOR2(0.0f, 0.0f);

	// Create tessellation buffer
	TessellationBuffer tessellationBuffer;
	tessellationBuffer.tessellationAmount = gameObject->GetTessellation();
	tessellationBuffer.padding = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	// Create the world matrix for the model
	MatrixBuffer matrixBuffer = MatrixBuffer_;
	gameObject->GetTransform()->GetWorldMatrix(matrixBuffer.world);
	TransposeMatrix(matrixBuffer);

	// Pass the textures to the shader
	OceanShader_->SendTextureToShader(0, refractionTexture);
	OceanShader_->SendTextureToShader(1, reflectionTexture);
	OceanShader_->SendTextureToShader(2, normalTexture);

	// Update Buffers
	OceanShader_->UpdateBuffer(DomainShader, 0, matrixBuffer);
	OceanShader_->UpdateBuffer(DomainShader, 1, cameraBuffer);
	OceanShader_->UpdateBuffer(DomainShader, 2, waveBuffer);
	OceanShader_->UpdateBuffer(PixelShader, 0, oceanBuffer);
	OceanShader_->UpdateBuffer(HullShader, 0, tessellationBuffer);
	OceanShader_->SendBuffersToShader();

	// Render using shader
	gameObject->Render();
	OceanShader_ -> Render(indexCount);
	
	return true;
}

bool ShaderManager::TextureRender(ParticleSystem* gameObject)
{
	Mesh3D* objMesh;
	Material* objMaterial;

	// Model Properties
	objMesh = gameObject->GetModel()->GetMesh();
	if (!objMesh)
	{
		MessageBox(NULL, L"No Model Attached - Texture", L"Error", MB_OK);
		return false;
	}
	objMaterial = gameObject->GetModel()->GetMaterial();
	if (!objMaterial)
	{
		MessageBox(NULL, L"No Material Attached - Texture", L"Error", MB_OK);
		return false;
	}

	// Model Properties
	int indexCount = objMesh->GetIndexCount();
	ID3D11ShaderResourceView* texture = objMaterial->GetBaseTexture();

	// Create matrix buffer
	MatrixBuffer matrixBuffer = MatrixBuffer_;
	gameObject->GetTransform()->GetTranslationMatrix(matrixBuffer.world);
	TransposeMatrix(matrixBuffer);

	// Update Buffers
	TextureShader_->UpdateBuffer(VertexShader, 0, matrixBuffer);
	TextureShader_->SendBuffersToShader();

	// Send Textures
	TextureShader_->SendTextureToShader(0, texture);

	// Render using shader
	gameObject->Render();
	TextureShader_->Render(indexCount);

	return true;
}
