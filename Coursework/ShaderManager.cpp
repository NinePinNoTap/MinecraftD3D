#include "ShaderManager.h"

ShaderManager::ShaderManager()
{
	// Initialise pointers to 0
	CloudShader_ = 0;
	ColourShader_ = 0;
	FireShader_ = 0;
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
	//==============================
	// Initialise the Colour Shader
	//==============================

	ColourShader_ = new ColourShader;
	if (!ColourShader_) { return false; }
	Result_ = ColourShader_->InitialiseShader(hwnd, L"colour.vs", L"colour.ps");
	if (!Result_) { return false; }
	ColourShader_->AddBuffer<MatrixBuffer>(VertexShader);
	ColourShader_->AddBuffer<PixelBuffer>(PixelShader);

	//=============================
	// Initialise the Cloud Shader
	//=============================

	CloudShader_ = new CloudShader;
	if (!CloudShader_) { return false; }
	Result_ = CloudShader_ -> InitialiseShader(hwnd, L"cloud.vs", L"cloud.ps");
	if (!Result_) { return false; }
	CloudShader_->AddBuffer<MatrixBuffer>(VertexShader);
	CloudShader_->AddBuffer<SkyBuffer>(PixelShader);
	CloudShader_->AddSamplerState(D3D11_FILTER_MIN_MAG_MIP_LINEAR, D3D11_TEXTURE_ADDRESS_WRAP);

	//============================
	// Initialise the Fire Shader
	//============================

	FireShader_ = new FireShader;
	if (!FireShader_) { return false; }
	Result_ = FireShader_->InitialiseShader(hwnd, L"fire.vs", L"fire.ps");
	if (!Result_) { return false; }
	FireShader_->AddBuffer<MatrixBuffer>(VertexShader);
	FireShader_->AddBuffer<NoiseBuffer>(VertexShader);
	FireShader_->AddBuffer<DistortionBuffer>(PixelShader);
	FireShader_->AddSamplerState(D3D11_FILTER_MIN_MAG_MIP_LINEAR, D3D11_TEXTURE_ADDRESS_WRAP);
	FireShader_->AddSamplerState(D3D11_FILTER_MIN_MAG_MIP_LINEAR, D3D11_TEXTURE_ADDRESS_CLAMP);

	//============================
	// Initialise the Font Shader
	//============================

	FontShader_ = new GameShader;
	if (!FontShader_) { return false; }
	Result_ = FontShader_->InitialiseShader(hwnd, L"font.vs", L"font.ps");
	if (!Result_) { return false; }
	FontShader_->AddBuffer<MatrixBuffer>(VertexShader);
	FontShader_->AddBuffer<PixelBuffer>(PixelShader);
	FontShader_->AddSamplerState(D3D11_FILTER_MIN_MAG_MIP_LINEAR, D3D11_TEXTURE_ADDRESS_WRAP);

	//=============================
	// Initialise the lightBuffer Shader
	//=============================

	LightShader_ = new LightShader;
	if(!LightShader_) { return false; }
	Result_ = LightShader_->InitialiseShader(hwnd, L"light.vs", L"light.ps");
	if(!Result_) { return false; }
	LightShader_->AddBuffer<MatrixBuffer>(VertexShader);
	LightShader_->AddBuffer<CameraBuffer>(VertexShader);
	LightShader_->AddBuffer<LightPositionBuffer>(VertexShader);
	LightShader_->AddBuffer<LightBuffer>(PixelShader);
	LightShader_->AddSamplerState(D3D11_FILTER_MIN_MAG_MIP_LINEAR, D3D11_TEXTURE_ADDRESS_WRAP);
	
	//=============================
	// Initialise the Ocean Shader
	//=============================

	OceanShader_ = new GameShader;
	if(!OceanShader_) { return false; }
	Result_ = OceanShader_->InitialiseShader(hwnd, L"ocean.vs", L"ocean.ps", L"ocean.hs", L"ocean.ds");
	if(!Result_) { return false; }
	OceanShader_->AddBuffer<MatrixBuffer>(DomainShader);
	OceanShader_->AddBuffer<CameraBuffer>(DomainShader);
	OceanShader_->AddBuffer<WaveBuffer>(DomainShader);
	OceanShader_->AddBuffer<OceanBuffer>(PixelShader);
	OceanShader_->AddBuffer<TessellationBuffer>(HullShader);
	OceanShader_->AddSamplerState(D3D11_FILTER_MIN_MAG_MIP_LINEAR, D3D11_TEXTURE_ADDRESS_WRAP);
	
	//==================================
	// Initialise the SkySphere Shader
	//==================================

	SkySphereShader_ = new SkyShader;
	if(!SkySphereShader_) { return false; }
	Result_ = SkySphereShader_->InitialiseShader(hwnd, L"skysphere.vs", L"skysphere.ps");
	if(!Result_) { return false; }
	SkySphereShader_->AddBuffer<MatrixBuffer>(VertexShader);
	SkySphereShader_->AddBuffer<GradientBuffer>(PixelShader);
	SkySphereShader_->AddSamplerState(D3D11_FILTER_MIN_MAG_MIP_LINEAR, D3D11_TEXTURE_ADDRESS_WRAP);
	
	//===============================
	// Initialise the Terrain Shader
	//===============================

	TerrainShader_ = new TerrainShader;
	if(!TerrainShader_) { return false; }
	Result_ = TerrainShader_->InitialiseShader(hwnd, L"terrain.vs", L"terrain.ps");
	if(!Result_) { return false; }
	TerrainShader_->AddBuffer<MatrixBuffer>(VertexShader);
	TerrainShader_->AddBuffer<LightPositionBuffer>(VertexShader);
	TerrainShader_->AddBuffer<LightBuffer>(PixelShader);
	TerrainShader_->AddSamplerState(D3D11_FILTER_MIN_MAG_MIP_LINEAR, D3D11_TEXTURE_ADDRESS_WRAP);
	
	//==========================================
	// Initialise the Terrain Reflection Shader
	//==========================================

	TerrainReflectionShader_ = new TerrainReflectionShader;
	if(!TerrainReflectionShader_) { return false; }
	Result_ = TerrainReflectionShader_->InitialiseShader(hwnd, L"terrainreflection.vs", L"terrainreflection.ps");
	if(!Result_) { return false; }
	TerrainReflectionShader_->AddBuffer<MatrixBuffer>(VertexShader);
	TerrainReflectionShader_->AddBuffer<ClipPlaneBuffer>(VertexShader);
	TerrainReflectionShader_->AddBuffer<LightBuffer>(PixelShader);
	TerrainReflectionShader_->AddSamplerState(D3D11_FILTER_MIN_MAG_MIP_LINEAR, D3D11_TEXTURE_ADDRESS_WRAP);
	
	//===============================
	// Initialise the Texture Shader
	//===============================

	TextureShader_ = new TextureShader;
	if(!TextureShader_) { return false; }
	Result_ = TextureShader_->InitialiseShader(hwnd, L"texture.vs", L"texture.ps");
	if(!Result_) { return false; }
	TextureShader_->AddBuffer<MatrixBuffer>(VertexShader);
	TextureShader_->AddSamplerState(D3D11_FILTER_MIN_MAG_MIP_LINEAR, D3D11_TEXTURE_ADDRESS_WRAP);

	//=======================
	// Setup Shader Database
	//=======================

	ShaderDatabase_["colour"] = ColourShader_;
	ShaderDatabase_["cloud"] = CloudShader_;
	ShaderDatabase_["light"] = LightShader_;
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

// Rendering
bool ShaderManager::FontRender(Text::SentenceType* sentence, ID3D11ShaderResourceView* texture)
{
	// Model Properties
	int indexCount = sentence->indexCount;

	// Create the pixel buffer
	PixelBuffer pixelBuffer;
	pixelBuffer.pixelColor.x = sentence->colour.r;
	pixelBuffer.pixelColor.y = sentence->colour.g;
	pixelBuffer.pixelColor.z = sentence->colour.b;
	pixelBuffer.pixelColor.w = sentence->colour.a;

	// Create matrix buffer
	MatrixBuffer matrixBuffer = MatrixBuffer_;
	TransposeMatrix(matrixBuffer);

	// Update Buffers
	FontShader_->UpdateBuffer(VertexShader, 0, matrixBuffer);
	FontShader_->UpdateBuffer(PixelShader, 0, pixelBuffer);
	FontShader_->SendBuffersToShader();

	// Pass the texture to the shader
	FontShader_->SendTextureToShader(0, texture);

	// Render using shader
	FontShader_->Render(indexCount);
	
	return true;
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
	Material* objMaterial;

	objMaterial = gameObject->GetModel()->GetMaterial();
	if (!objMaterial)
	{
		MessageBox(NULL, L"No Material Attached - Texture", L"Error", MB_OK);
		return false;
	}

	// Model Properties
	int indexCount = gameObject->GetIndexCount();
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
