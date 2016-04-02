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

	ColourShader_ = new GameShader;
	if (!ColourShader_) { return false; }
	Result_ = ColourShader_->InitialiseShader(hwnd, L"colour.vs", L"colour.ps");
	if (!Result_) { return false; }
	ColourShader_->AddBuffer<MatrixBuffer>(VertexShader);
	ColourShader_->AddBuffer<PixelBuffer>(PixelShader);

	//=============================
	// Initialise the Cloud Shader
	//=============================

	CloudShader_ = new GameShader;
	if (!CloudShader_) { return false; }
	Result_ = CloudShader_ -> InitialiseShader(hwnd, L"cloud.vs", L"cloud.ps");
	if (!Result_) { return false; }
	CloudShader_->AddBuffer<MatrixBuffer>(VertexShader);
	CloudShader_->AddBuffer<SkyBuffer>(PixelShader);
	CloudShader_->AddSamplerState(D3D11_FILTER_MIN_MAG_MIP_LINEAR, D3D11_TEXTURE_ADDRESS_WRAP);

	//============================
	// Initialise the Fire Shader
	//============================

	FireShader_ = new GameShader;
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

	LightShader_ = new GameShader;
	if(!LightShader_) { return false; }
	Result_ = LightShader_->InitialiseShader(hwnd, L"light.vs", L"light.ps");
	if(!Result_) { return false; }
	LightShader_->AddBuffer<MatrixBuffer>(VertexShader);
	LightShader_->AddBuffer<CameraBuffer>(VertexShader);
	LightShader_->AddBuffer<LightPositionBuffer>(VertexShader);
	LightShader_->AddBuffer<LightBuffer>(PixelShader);
	LightShader_->AddSamplerState(D3D11_FILTER_MIN_MAG_MIP_LINEAR, D3D11_TEXTURE_ADDRESS_WRAP);
	
	//=============================
	// Initialise the oceanBuffer Shader
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
	// Initialise the skyBuffer Sphere Shader
	//==================================

	SkySphereShader_ = new GameShader;
	if(!SkySphereShader_) { return false; }
	Result_ = SkySphereShader_->InitialiseShader(hwnd, L"skysphere.vs", L"skysphere.ps");
	if(!Result_) { return false; }
	SkySphereShader_->AddBuffer<MatrixBuffer>(VertexShader);
	SkySphereShader_->AddBuffer<GradientBuffer>(PixelShader);
	SkySphereShader_->AddSamplerState(D3D11_FILTER_MIN_MAG_MIP_LINEAR, D3D11_TEXTURE_ADDRESS_WRAP);
	
	//===============================
	// Initialise the Terrain Shader
	//===============================

	TerrainShader_ = new GameShader;
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

	TerrainReflectionShader_ = new GameShader;
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

	TextureShader_ = new GameShader;
	if(!TextureShader_) { return false; }
	Result_ = TextureShader_->InitialiseShader(hwnd, L"texture.vs", L"texture.ps");
	if(!Result_) { return false; }
	TextureShader_->AddBuffer<MatrixBuffer>(VertexShader);
	TextureShader_->AddSamplerState(D3D11_FILTER_MIN_MAG_MIP_LINEAR, D3D11_TEXTURE_ADDRESS_WRAP);

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
bool ShaderManager::CloudShader(Clouds* gameObject)
{
	Mesh3D* objMesh;
	Material* objMaterial;

	// Model Properties
	objMesh = gameObject->GetModel()->GetMesh();
	if (!objMesh)
	{
		MessageBox(NULL, L"No Model Attached - Clouds", L"Error", MB_OK);
		return false;
	}
	objMaterial = gameObject->GetModel()->GetMaterial();
	if (!objMaterial)
	{
		MessageBox(NULL, L"No Material Attached - Clouds", L"Error", MB_OK);
		return false;
	}

	// Model Properties
	int indexCount = objMesh->GetIndexCount();
	ID3D11ShaderResourceView* cloudTexture = objMaterial->GetTexture("BaseTexture")->GetTexture();
	ID3D11ShaderResourceView* perturbTexture = objMaterial->GetTexture("PerturbTexture")->GetTexture();

	// Create matrix buffer
	MatrixBuffer matrixBuffer = MatrixBuffer_;
	gameObject->GetTransform()->GetTranslationMatrix(matrixBuffer.world);
	TransposeMatrix(matrixBuffer);

	// Create the sky buffer
	SkyBuffer skyBuffer;
	skyBuffer.brightness = objMaterial->GetFloat("TextureBrightness");
	skyBuffer.scale = objMaterial->GetFloat("TextureScale");
	skyBuffer.translation = gameObject->GetFrame();
	skyBuffer.padding = 0.0f;

	// Update Buffers
	CloudShader_->UpdateBuffer(VertexShader, 0, matrixBuffer);
	CloudShader_->UpdateBuffer(PixelShader, 0, skyBuffer);
	CloudShader_->SendBuffersToShader();

	// Send Textures
	CloudShader_->SendTextureToShader(0, cloudTexture);
	CloudShader_->SendTextureToShader(1, perturbTexture);

	// Render using shader
	gameObject->Render();
	CloudShader_ -> Render(indexCount);
	
	return true;
}

bool ShaderManager::ColourShader(GameObject* gameObject)
{
	Mesh3D* objMesh;
	Material* objMaterial;

	// Model Properties
	objMesh = gameObject->GetModel()->GetMesh();
	if (!objMesh)
	{
		MessageBox(NULL, L"No Model Attached - Colour", L"Error", MB_OK);
		return false;
	}
	objMaterial = gameObject->GetModel()->GetMaterial();
	if (!objMaterial)
	{
		MessageBox(NULL, L"No Material Attached - Colour", L"Error", MB_OK);
		return false;
	}

	int indexCount = objMesh->GetIndexCount();

	// Create the world matrix for the model
	MatrixBuffer matrixBuffer = MatrixBuffer_;
	gameObject->GetTransform()->GetWorldMatrix(matrixBuffer.world);
	TransposeMatrix(matrixBuffer);

	// Create pixel buffer
	PixelBuffer pixelBuffer;
	pixelBuffer.pixelColor = D3DXVECTOR4(1, 1, 1, 1);

	// Update Buffers
	ColourShader_->UpdateBuffer(VertexShader, 0, matrixBuffer);
	ColourShader_->UpdateBuffer(PixelShader, 0, pixelBuffer);
	ColourShader_->SendBuffersToShader();

	// Render using shader
	gameObject->Render();
	ColourShader_->Render(indexCount);

	return true;
}

bool ShaderManager::FireShader(Fire* gameObject)
{
	Mesh3D* objMesh;
	Material* objMaterial;

	// Model Properties
	objMesh = gameObject->GetModel()->GetMesh();
	if (!objMesh)
	{
		MessageBox(NULL, L"No Model Attached - DrawFire", L"Error", MB_OK);
		return false;
	}

	objMaterial = gameObject->GetModel()->GetMaterial();
	if (!objMaterial)
	{
		MessageBox(NULL, L"No Material Attached - DrawFire", L"Error", MB_OK);
		return false;
	}

	int indexCount = objMesh->GetIndexCount();
	ID3D11ShaderResourceView* fireTexture = objMaterial->GetBaseTexture();
	ID3D11ShaderResourceView* noiseTexture = objMaterial->GetNoiseTexture();
	ID3D11ShaderResourceView* alphaTexture = objMaterial->GetAlphaTexture();

	// Create noise buffer
	NoiseBuffer noiseBuffer;
	noiseBuffer.frameTime = gameObject -> GetFrame();
	noiseBuffer.scrollSpeeds = gameObject->GetSpeed();
	noiseBuffer.scales = gameObject->GetTiling();
	noiseBuffer.padding = 0.0f;

	// Create the distortion buffer
	DistortionBuffer distortionBuffer;
	gameObject->GetDistortion(distortionBuffer.distortion1, distortionBuffer.distortion2, distortionBuffer.distortion3);
	distortionBuffer.distortionScale = gameObject->GetDistortionAmount();
	distortionBuffer.distortionBias = gameObject->GetDistortionBias();

	// Create matrix buffer
	MatrixBuffer matrixBuffer = MatrixBuffer_;
	gameObject->GetTransform()->GetWorldMatrix(matrixBuffer.world);
	TransposeMatrix(matrixBuffer);

	// Update Buffers
	FireShader_->UpdateBuffer(VertexShader, 0, matrixBuffer);
	FireShader_->UpdateBuffer(VertexShader, 1, noiseBuffer);
	FireShader_->UpdateBuffer(PixelShader, 0, distortionBuffer);
	FireShader_->SendBuffersToShader();

	// Send textures
	FireShader_->SendTextureToShader(0, fireTexture);
	FireShader_->SendTextureToShader(1, noiseTexture);
	FireShader_->SendTextureToShader(2, alphaTexture);

	// Render using shader
	gameObject->Render();
	FireShader_ -> Render(indexCount);

	return true;
}

bool ShaderManager::FontShader(Text::SentenceType* sentence, ID3D11ShaderResourceView* texture)
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

bool ShaderManager::LightShader(GameObject* gameObject)
{
	Mesh3D* objMesh;
	Material* objMaterial;

	// Model Properties
	objMesh = gameObject->GetModel()->GetMesh();
	if (!objMesh)
	{
		MessageBox(NULL, L"No Model Attached - lightBuffer", L"Error", MB_OK);
		return false;
	}

	objMaterial = gameObject->GetModel()->GetMaterial();
	if (!objMaterial)
	{
		MessageBox(NULL, L"No Material Attached - lightBuffer", L"Error", MB_OK);
		return false;
	}

	int indexCount = objMesh -> GetIndexCount();
	ID3D11ShaderResourceView* texture = objMaterial -> GetBaseTexture();

	// Create camera buffer
	CameraBuffer cameraBuffer;
	cameraBuffer.cameraPosition = Camera::Instance()->GetTransform()->GetPosition();
	cameraBuffer.normalMapTiling = D3DXVECTOR2(0.0f, 0.0f);
	cameraBuffer.padding = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	// Create the light buffer
	LightBuffer lightBuffer;
	lightBuffer.lightDirection = Light::Instance()->GetDirection();
	lightBuffer.ambientColor = Light::Instance()->GetAmbientColor();
	lightBuffer.diffuseColor = Light::Instance()->GetDiffuseColor();
	lightBuffer.specularColor = Light::Instance()->GetSpecularColor();
	lightBuffer.specularPower = objMaterial->GetFloat("SpecularPower");

	// Create light position buffer
	LightPositionBuffer lightPositionBuffer;
	lightPositionBuffer.lightPosition = D3DXVECTOR4(Light::Instance()->GetTransform()->GetPosition(), 1.0f);

	// Create matrix buffer
	MatrixBuffer matrixBuffer = MatrixBuffer_;
	gameObject->GetTransform()->GetWorldMatrix(matrixBuffer.world);
	TransposeMatrix(matrixBuffer);

	// Pass the texture to the shader
	LightShader_->SendTextureToShader(0, texture);

	// Update Buffers
	LightShader_->UpdateBuffer(VertexShader, 0, matrixBuffer);
	LightShader_->UpdateBuffer(VertexShader, 1, cameraBuffer);
	LightShader_->UpdateBuffer(VertexShader, 2, lightPositionBuffer);
	LightShader_->UpdateBuffer(PixelShader, 0, lightBuffer);
	LightShader_->SendBuffersToShader();

	// Render using shader
	gameObject->Render();
	LightShader_ -> Render(indexCount);

	return true;
}

bool ShaderManager::OceanShader(Water* gameObject, Texture* refraction, Texture* reflection)
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

bool ShaderManager::SkyShader(GameObject* gameObject)
{
	Mesh3D* objMesh;
	Material* objMaterial;
	
	objMesh = gameObject->GetModel()->GetMesh();
	if (!objMesh)
	{
		MessageBox(NULL, L"No Model Attached - SkySphere", L"Error", MB_OK);
		return false;
	}
	objMaterial = gameObject->GetModel()->GetMaterial();
	if (!objMaterial)
	{
		MessageBox(NULL, L"No Material Attached - SkySphere", L"Error", MB_OK);
		return false;
	}

	// Model Properties
	int indexCount = objMesh->GetIndexCount();

	// Create matrixBuffer Buffer
	MatrixBuffer matrixBuffer = MatrixBuffer_;
	gameObject->GetTransform()->GetTranslationMatrix(matrixBuffer.world);
	TransposeMatrix(matrixBuffer);

	// Create the sky buffer
	GradientBuffer gradientBuffer;
	gradientBuffer.topColor = objMaterial->GetVector4("TopColour");
	gradientBuffer.centerColor = objMaterial->GetVector4("CenterColour");

	// Update Buffers
	SkySphereShader_->UpdateBuffer(VertexShader, 0, matrixBuffer);
	SkySphereShader_->UpdateBuffer(PixelShader, 0, gradientBuffer);
	SkySphereShader_->SendBuffersToShader();

	// Render using shader
	gameObject->Render();
	SkySphereShader_ -> Render(indexCount);
	
	return true;
}

bool ShaderManager::TerrainShader(Terrain* gameObject)
{
	Mesh3D* objMesh;
	Material* objMaterial;

	// Model Properties
	objMesh = gameObject->GetModel()->GetMesh();
	if (!objMesh)
	{
		MessageBox(NULL, L"No Model Attached - Terrain", L"Error", MB_OK);
		return false;
	}
	objMaterial = gameObject->GetModel()->GetMaterial();
	if (!objMaterial)
	{
		MessageBox(NULL, L"No Material Attached - Terrain", L"Error", MB_OK);
		return false;
	}

	// Retrieve variables for rendering
	int indexCount = objMesh -> GetIndexCount();
	ID3D11ShaderResourceView* colorTexture = objMaterial->GetBaseTexture();
	ID3D11ShaderResourceView* normalTexture = objMaterial->GetNormalTexture();

	// Create the light buffer
	LightBuffer lightBuffer;
	lightBuffer.ambientColor = Light::Instance()->GetAmbientColor();
	lightBuffer.diffuseColor = Light::Instance()->GetDiffuseColor();
	lightBuffer.lightDirection = Light::Instance()->GetDirection();

	// Create the light position buffer
	LightPositionBuffer lightPositionBuffer;
	lightPositionBuffer.lightPosition = D3DXVECTOR4(Light::Instance()->GetTransform()->GetPosition(), 1.0f);

	// Create matrix buffer
	MatrixBuffer matrixBuffer = MatrixBuffer_;
	gameObject->GetTransform()->GetWorldMatrix(matrixBuffer.world);
	TransposeMatrix(matrixBuffer);

	// Pass the textures to the shaders
	TerrainShader_->SendTextureToShader(0, colorTexture);
	TerrainShader_->SendTextureToShader(1, normalTexture);

	// Update Buffers
	TerrainShader_->UpdateBuffer(VertexShader, 0, matrixBuffer);
	TerrainShader_->UpdateBuffer(VertexShader, 1, lightPositionBuffer);
	TerrainShader_->UpdateBuffer(PixelShader, 0, lightBuffer);
	TerrainShader_->SendBuffersToShader();

	// Render using shader
	gameObject->Render();
	TerrainShader_ -> Render(indexCount);
	
	return true;
}

bool ShaderManager::TerrainShader(Terrain* gameObject, D3DXVECTOR4 clipPlane)
{
	Mesh3D* objMesh;
	Material* objMaterial;

	// Model Properties
	objMesh = gameObject->GetModel()->GetMesh();
	if (!objMesh)
	{
		MessageBox(NULL, L"No Model Attached - Terrain", L"Error", MB_OK);
		return false;
	}
	objMaterial = gameObject->GetModel()->GetMaterial();
	if (!objMaterial)
	{
		MessageBox(NULL, L"No Material Attached - Terrain", L"Error", MB_OK);
		return false;
	}

	// Model Properties
	int indexCount = objMesh -> GetIndexCount();
	ID3D11ShaderResourceView* colorTexture = objMaterial->GetBaseTexture();
	ID3D11ShaderResourceView* normalTexture = objMaterial->GetNormalTexture();

	// Create the light buffer
	LightBuffer lightBuffer;
	lightBuffer.ambientColor = Light::Instance()->GetAmbientColor();
	lightBuffer.diffuseColor = Light::Instance()->GetDiffuseColor();
	lightBuffer.lightDirection = Light::Instance()->GetDirection();

	// Create the light position buffer
	LightPositionBuffer lightPositionBuffer;
	lightPositionBuffer.lightPosition = D3DXVECTOR4(Light::Instance()->GetTransform()->GetPosition(), 1.0f);

	// Create the clip plane buffer
	ClipPlaneBuffer clipPlaneBuffer;
	clipPlaneBuffer.clipPlane = clipPlane;

	// Create matrix buffer
	MatrixBuffer matrixBuffer = MatrixBuffer_;
	gameObject->GetTransform()->GetWorldMatrix(matrixBuffer.world);
	TransposeMatrix(matrixBuffer);

	// Pass the textures to the shader
	TerrainReflectionShader_->SendTextureToShader(0, colorTexture);
	TerrainReflectionShader_->SendTextureToShader(1, normalTexture);

	// Update Buffers
	TerrainReflectionShader_->UpdateBuffer(VertexShader, 0, matrixBuffer);
	TerrainReflectionShader_->UpdateBuffer(VertexShader, 1, clipPlaneBuffer);
	TerrainReflectionShader_->UpdateBuffer(PixelShader, 0, lightBuffer);
	TerrainReflectionShader_->SendBuffersToShader();

	// Render using shader
	gameObject->Render();
	TerrainReflectionShader_ -> Render(indexCount);
	
	return true;
}

bool ShaderManager::TextureShader(GameObject* gameObject)
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

	int indexCount = objMesh->GetIndexCount();
	ID3D11ShaderResourceView* texture = objMaterial->GetBaseTexture();

	// Create the world matrix for the model
	MatrixBuffer matrixBuffer = MatrixBuffer_;
	gameObject->GetTransform()->GetWorldMatrix(matrixBuffer.world);
	TransposeMatrix(matrixBuffer);

	// Update Buffers
	TextureShader_->UpdateBuffer(VertexShader, 0, matrixBuffer);
	TextureShader_->SendBuffersToShader();

	// Pass the texture into the shader
	TextureShader_->SendTextureToShader(0, texture);

	// Render using shader
	gameObject->Render();
	TextureShader_->Render(indexCount);

	return true;
}

bool ShaderManager::TextureShader(ParticleSystem* gameObject)
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
