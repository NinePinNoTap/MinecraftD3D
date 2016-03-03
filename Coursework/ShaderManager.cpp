#include "ShaderManager.h"

ShaderManager::ShaderManager()
{
	// Initialise pointers to 0
	CloudShader_ = 0;
	FireShader_ = 0;
	LightShader_ = 0;
	OceanShader_ = 0;
	ParticleShader_ = 0;
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

	CloudShader_ = new GameShader;
	if (!CloudShader_) { return false; }
	Result_ = CloudShader_ -> InitialiseShader(hwnd, L"Data/Shaders/cloud.vs", L"Data/Shaders/cloud.ps");
	if (!Result_) { return false; }
	CloudShader_->AddBuffer<MatrixCBuffer>(VertexShader);
	CloudShader_->AddBuffer<SkyCBuffer>(PixelShader);
	CloudShader_->AddSamplerState(D3D11_FILTER_MIN_MAG_MIP_LINEAR, D3D11_TEXTURE_ADDRESS_WRAP);

	//============================
	// Initialise the Fire Shader
	//============================

	FireShader_ = new GameShader;
	if (!FireShader_) { return false; }
	Result_ = FireShader_->InitialiseShader(hwnd, L"Data/Shaders/fire.vs", L"Data/Shaders/fire.ps");
	if (!Result_) { return false; }
	FireShader_->AddBuffer<MatrixCBuffer>(VertexShader);
	FireShader_->AddBuffer<NoiseCBuffer>(VertexShader);
	FireShader_->AddBuffer<DistortionCBuffer>(PixelShader);
	FireShader_->AddSamplerState(D3D11_FILTER_MIN_MAG_MIP_LINEAR, D3D11_TEXTURE_ADDRESS_WRAP);
	FireShader_->AddSamplerState(D3D11_FILTER_MIN_MAG_MIP_LINEAR, D3D11_TEXTURE_ADDRESS_CLAMP);

	//============================
	// Initialise the Font Shader
	//============================

	FontShader_ = new GameShader;
	if (!FontShader_) { return false; }
	Result_ = FontShader_->InitialiseShader(hwnd, L"Data/Shaders/font.vs", L"Data/Shaders/font.ps");
	if (!Result_) { return false; }
	FontShader_->AddBuffer<MatrixCBuffer>(VertexShader);
	FontShader_->AddBuffer<PixelCBuffer>(PixelShader);
	FontShader_->AddSamplerState(D3D11_FILTER_MIN_MAG_MIP_LINEAR, D3D11_TEXTURE_ADDRESS_WRAP);

	//=============================
	// Initialise the Light Shader
	//=============================

	LightShader_ = new GameShader;
	if(!LightShader_) { return false; }
	Result_ = LightShader_->InitialiseShader(hwnd, L"Data/Shaders/light.vs", L"Data/Shaders/light.ps");
	if(!Result_) { return false; }
	LightShader_->AddBuffer<MatrixCBuffer>(VertexShader);
	LightShader_->AddBuffer<CameraCBuffer>(VertexShader);
	LightShader_->AddBuffer<LightPositionCBuffer>(VertexShader);
	LightShader_->AddBuffer<LightCBuffer>(PixelShader);
	LightShader_->AddSamplerState(D3D11_FILTER_MIN_MAG_MIP_LINEAR, D3D11_TEXTURE_ADDRESS_WRAP);
	
	//=============================
	// Initialise the Ocean Shader
	//=============================

	OceanShader_ = new GameShader;
	if(!OceanShader_) { return false; }
	Result_ = OceanShader_->InitialiseShader(hwnd, L"Data/Shaders/ocean.vs", L"Data/Shaders/ocean.ps", L"Data/Shaders/ocean.hs", L"Data/Shaders/ocean.ds");
	if(!Result_) { return false; }
	OceanShader_->AddBuffer<MatrixCBuffer>(DomainShader);
	OceanShader_->AddBuffer<CameraCBuffer>(DomainShader);
	OceanShader_->AddBuffer<WaveCBuffer>(DomainShader);
	OceanShader_->AddBuffer<OceanCBuffer>(PixelShader);
	OceanShader_->AddBuffer<TessellationCBuffer>(HullShader);
	OceanShader_->AddSamplerState(D3D11_FILTER_MIN_MAG_MIP_LINEAR, D3D11_TEXTURE_ADDRESS_WRAP);

	//================================
	// Initialise the Particle Shader
	//================================

	ParticleShader_ = new GameShader;
	if(!ParticleShader_) { return false; }
	Result_ = ParticleShader_->InitialiseShader(hwnd, L"Data/Shaders/particle.vs", L"Data/Shaders/particle.ps");
	if(!Result_) { return false; }
	ParticleShader_->AddBuffer<MatrixCBuffer>(VertexShader);
	ParticleShader_->AddSamplerState(D3D11_FILTER_MIN_MAG_MIP_LINEAR, D3D11_TEXTURE_ADDRESS_WRAP);
	
	//==================================
	// Initialise the Sky Sphere Shader
	//==================================

	SkySphereShader_ = new GameShader;
	if(!SkySphereShader_) { return false; }
	Result_ = SkySphereShader_->InitialiseShader(hwnd, L"Data/Shaders/skysphere.vs", L"Data/Shaders/skysphere.ps");
	if(!Result_) { return false; }
	SkySphereShader_->AddBuffer<MatrixCBuffer>(VertexShader);
	SkySphereShader_->AddBuffer<GradientCBuffer>(PixelShader);
	SkySphereShader_->AddSamplerState(D3D11_FILTER_MIN_MAG_MIP_LINEAR, D3D11_TEXTURE_ADDRESS_WRAP);
	
	//===============================
	// Initialise the Terrain Shader
	//===============================

	TerrainShader_ = new GameShader;
	if(!TerrainShader_) { return false; }
	Result_ = TerrainShader_->InitialiseShader(hwnd, L"Data/Shaders/terrain.vs", L"Data/Shaders/terrain.ps");
	if(!Result_) { return false; }
	TerrainShader_->AddBuffer<MatrixCBuffer>(VertexShader);
	TerrainShader_->AddBuffer<LightPositionCBuffer>(VertexShader);
	TerrainShader_->AddBuffer<LightCBuffer>(PixelShader);
	TerrainShader_->AddSamplerState(D3D11_FILTER_MIN_MAG_MIP_LINEAR, D3D11_TEXTURE_ADDRESS_WRAP);
	
	//==========================================
	// Initialise the Terrain Reflection Shader
	//==========================================

	TerrainReflectionShader_ = new GameShader;
	if(!TerrainReflectionShader_) { return false; }
	Result_ = TerrainReflectionShader_->InitialiseShader(hwnd, L"Data/Shaders/terrainreflection.vs", L"Data/Shaders/terrainreflection.ps");
	if(!Result_) { return false; }
	TerrainReflectionShader_->AddBuffer<MatrixCBuffer>(VertexShader);
	TerrainReflectionShader_->AddBuffer<ClipPlaneCBuffer>(VertexShader);
	TerrainReflectionShader_->AddBuffer<LightCBuffer>(PixelShader);
	TerrainReflectionShader_->AddSamplerState(D3D11_FILTER_MIN_MAG_MIP_LINEAR, D3D11_TEXTURE_ADDRESS_WRAP);
	
	//===============================
	// Initialise the Texture Shader
	//===============================

	TextureShader_ = new GameShader;
	if(!TextureShader_) { return false; }
	Result_ = TextureShader_->InitialiseShader(hwnd, L"Data/Shaders/texture.vs", L"Data/Shaders/texture.ps");
	if(!Result_) { return false; }
	TextureShader_->AddBuffer<MatrixCBuffer>(VertexShader);
	TextureShader_->AddSamplerState(D3D11_FILTER_MIN_MAG_MIP_LINEAR, D3D11_TEXTURE_ADDRESS_WRAP);

	return true;
}

// Shutdown
void ShaderManager::Shutdown()
{
	//=========================================
	// Go through each shader and shut it down
	//=========================================

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

// Matrix Setters
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
bool ShaderManager::CloudRender(Clouds* Obj)
{
	Mesh* ObjMesh;
	Material* ObjMat;

	// Model Properties
	ObjMesh = Obj->GetMesh();
	if (!ObjMesh)
	{
		MessageBox(NULL, L"No Model Attached - Clouds", L"Error", MB_OK);
		return false;
	}
	ObjMat = ObjMesh->GetMaterial();
	if (!ObjMat)
	{
		MessageBox(NULL, L"No Material Attached - Clouds", L"Error", MB_OK);
		return false;
	}

	// Model Properties
	int indexCount = ObjMesh->GetIndexCount();
	ID3D11ShaderResourceView* textureArray = ObjMat -> GetTextureArray();

	// Create matrix buffer
	MatrixCBuffer Matrix = MatrixBuffer_;
	Obj->GetTransform()->GetTranslationMatrix(Matrix.world);
	TransposeMatrix(Matrix);

	// Create the sky buffer
	SkyCBuffer Sky;
	Sky.brightness = Obj->GetBrightness();
	Sky.scale = Obj->GetScale();
	Sky.translation = Obj->GetTranslation();
	Sky.padding = 0.0f;

	// Update Buffers
	CloudShader_->UpdateBuffer(VertexShader, 0, Matrix);
	CloudShader_->UpdateBuffer(PixelShader, 0, Sky);
	CloudShader_->SendBuffersToShader();

	// Send Textures
	CloudShader_->SendTextureToShader(0, textureArray);

	// Render using shader
	ObjMesh -> Render();
	CloudShader_ -> Render(indexCount);
	
	return true;
}

bool ShaderManager::FireRender(Fire* Obj)
{
	Mesh* ObjMesh;
	Material* ObjMat;

	// Model Properties
	ObjMesh = Obj -> GetMesh();
	if (!ObjMesh)
	{
		MessageBox(NULL, L"No Model Attached - DrawFire", L"Error", MB_OK);
		return false;
	}
	ObjMat = ObjMesh->GetMaterial();
	if (!ObjMat)
	{
		MessageBox(NULL, L"No Material Attached - DrawFire", L"Error", MB_OK);
		return false;
	}

	int indexCount = ObjMesh->GetIndexCount();
	ID3D11ShaderResourceView* fireTexture = ObjMat->GetBaseTexture();
	ID3D11ShaderResourceView* noiseTexture = ObjMat->GetNoiseTexture();
	ID3D11ShaderResourceView* alphaTexture = ObjMat->GetAlphaTexture();

	// Create noise buffer
	NoiseCBuffer Noise;
	Noise.frameTime = Obj -> GetFrame();
	Noise.scrollSpeeds = Obj->GetSpeed();
	Noise.scales = Obj->GetTiling();
	Noise.padding = 0.0f;

	// Create the distortion buffer
	DistortionCBuffer Distortion;
	Obj->GetDistortion(Distortion.distortion1, Distortion.distortion2, Distortion.distortion3);
	Distortion.distortionScale = Obj->GetDistortionAmount();
	Distortion.distortionBias = Obj->GetDistortionBias();

	// Create matrix buffer
	MatrixCBuffer Matrix = MatrixBuffer_;
	Obj->GetTransform()->GetWorldMatrix(Matrix.world);
	TransposeMatrix(Matrix);

	// Update Buffers
	FireShader_->UpdateBuffer(VertexShader, 0, Matrix);
	FireShader_->UpdateBuffer(VertexShader, 1, Noise);
	FireShader_->UpdateBuffer(PixelShader, 0, Distortion);
	FireShader_->SendBuffersToShader();

	// Send textures
	FireShader_->SendTextureToShader(0, fireTexture);
	FireShader_->SendTextureToShader(1, noiseTexture);
	FireShader_->SendTextureToShader(2, alphaTexture);

	// Render using shader
	ObjMesh->Render();
	FireShader_ -> Render(indexCount);

	return true;
}

bool ShaderManager::FontRender(Text::SentenceType* sentence, ID3D11ShaderResourceView* texture)
{
	// Model Properties
	int indexCount = sentence->indexCount;

	// Create the pixel buffer
	PixelCBuffer Pixel;
	Pixel.pixelColor.x = sentence->colour.r;
	Pixel.pixelColor.y = sentence->colour.g;
	Pixel.pixelColor.z = sentence->colour.b;
	Pixel.pixelColor.w = sentence->colour.a;

	// Create matrix buffer
	MatrixCBuffer Matrix = MatrixBuffer_;
	TransposeMatrix(Matrix);

	// Update Buffers
	FontShader_->UpdateBuffer(VertexShader, 0, Matrix);
	FontShader_->UpdateBuffer(PixelShader, 0, Pixel);
	FontShader_->SendBuffersToShader();

	// Pass the texture to the shader
	FontShader_->SendTextureToShader(0, texture);

	// Render using shader
	FontShader_->Render(indexCount);
	
	return true;
}

bool ShaderManager::LightRender(GameObject* Obj, float specularPower)
{
	Mesh* ObjMesh;
	Material* ObjMat;

	// Model Properties
	ObjMesh = Obj->GetMesh();
	if (!ObjMesh)
	{
		MessageBox(NULL, L"No Model Attached - Light", L"Error", MB_OK);
		return false;
	}
	ObjMat = ObjMesh->GetMaterial();
	if (!ObjMat)
	{
		MessageBox(NULL, L"No Material Attached - Light", L"Error", MB_OK);
		return false;
	}

	int indexCount = ObjMesh -> GetIndexCount();
	ID3D11ShaderResourceView* texture = ObjMat -> GetTextureArray();

	// Create camera buffer
	CameraCBuffer Camera;
	Camera.cameraPosition = Camera::Instance()->GetTransform()->GetPosition();
	Camera.normalMapTiling = D3DXVECTOR2(0.0f, 0.0f);
	Camera.padding = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	// Create the light buffer
	LightCBuffer Light;
	Light.lightDirection = Light::Instance()->GetDirection();
	Light.ambientColor = Light::Instance()->GetAmbientColor();
	Light.diffuseColor = Light::Instance()->GetDiffuseColor();
	Light.specularColor = Light::Instance()->GetSpecularColor();
	Light.specularPower = specularPower;

	// Create light position buffer
	LightPositionCBuffer LightPosition;
	LightPosition.lightPosition = D3DXVECTOR4(Light::Instance()->GetTransform()->GetPosition(), 1.0f);

	// Create matrix buffer
	MatrixCBuffer Matrix = MatrixBuffer_;
	Obj->GetTransform()->GetWorldMatrix(Matrix.world);
	TransposeMatrix(Matrix);

	// Pass the texture to the shader
	LightShader_->SendTextureToShader(0, texture);

	// Update Buffers
	LightShader_->UpdateBuffer(VertexShader, 0, Matrix);
	LightShader_->UpdateBuffer(VertexShader, 1, Camera);
	LightShader_->UpdateBuffer(VertexShader, 2, LightPosition);
	LightShader_->UpdateBuffer(PixelShader, 0, Light);
	LightShader_->SendBuffersToShader();

	// Render using shader
	ObjMesh->Render();
	LightShader_ -> Render(indexCount);

	return true;
}

bool ShaderManager::WaterRender(Water* Obj, Texture* refraction, Texture* reflection)
{
	Mesh* ObjMesh;
	Material* ObjMat;

	// Model Properties
	ObjMesh = Obj->GetMesh();
	if (!ObjMesh)
	{
		MessageBox(NULL, L"No Model Attached - Water", L"Error", MB_OK);
		return false;
	}
	ObjMat = ObjMesh->GetMaterial();
	if (!ObjMat)
	{
		MessageBox(NULL, L"No Material Attached - Water", L"Error", MB_OK);
		return false;
	}

	// Retrieve variables for rendering
	int indexCount = ObjMesh -> GetIndexCount();
	ID3D11ShaderResourceView* refractionTexture = refraction -> GetTexture();
	ID3D11ShaderResourceView* reflectionTexture = reflection -> GetTexture();
	ID3D11ShaderResourceView* normalTexture = ObjMat->GetNormalTexture();

	// Create camera buffer
	CameraCBuffer Camera;
	Camera.cameraPosition = Camera::Instance()->GetTransform()->GetPosition();
	Camera.normalMapTiling = Obj -> GetNormalMapTiling();
	Camera.padding = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	// Create ocean buffer
	OceanCBuffer Ocean;
	Ocean.waterTranslation = Obj->GetFrame();
	Ocean.reflectRefractScale = Obj->GetReflectRefractScale();
	Ocean.refractionTint = Obj->GetRefractionTint();
	Ocean.lightDirection = Light::Instance() -> GetDirection();
	Ocean.specularShininess = Obj->GetSpecularShininess();
	Ocean.padding = D3DXVECTOR2(0.0f, 0.0f);

	// Create wave buffer
	WaveCBuffer Wave;
	Wave.waveTime = Obj->GetWaterTimer();
	Wave.waveHeight = Obj->GetWaveHeight() / 2;
	Wave.padding = D3DXVECTOR2(0.0f, 0.0f);

	// Create tessellation buffer
	TessellationCBuffer Tessellation;
	Tessellation.tessellationAmount = Obj->GetTessellation();
	Tessellation.padding = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	// Create the world matrix for the model
	MatrixCBuffer Matrix = MatrixBuffer_;
	Obj->GetTransform()->GetWorldMatrix(Matrix.world);
	TransposeMatrix(Matrix);

	// Pass the textures to the shader
	OceanShader_->SendTextureToShader(0, refractionTexture);
	OceanShader_->SendTextureToShader(1, reflectionTexture);
	OceanShader_->SendTextureToShader(2, normalTexture);

	// Update Buffers
	OceanShader_->UpdateBuffer(DomainShader, 0, Matrix);
	OceanShader_->UpdateBuffer(DomainShader, 1, Camera);
	OceanShader_->UpdateBuffer(DomainShader, 2, Wave);
	OceanShader_->UpdateBuffer(PixelShader, 0, Ocean);
	OceanShader_->UpdateBuffer(HullShader, 0, Tessellation);
	OceanShader_->SendBuffersToShader();

	// Render using shader
	ObjMesh->Render(D3D11_PRIMITIVE_TOPOLOGY_4_CONTROL_POINT_PATCHLIST);
	OceanShader_ -> Render(indexCount);
	
	return true;
}

bool ShaderManager::ParticleRender(ParticleSystem* Obj)
{
	Material* ObjMat;
	
	ObjMat = Obj->GetMaterial();
	if (!ObjMat)
	{
		MessageBox(NULL, L"No Material Attached - Particle", L"Error", MB_OK);
		return false;
	}

	// Model Properties
	int indexCount = Obj->GetIndexCount();
	ID3D11ShaderResourceView* texture = ObjMat->GetBaseTexture();

	// Create matrix buffer
	MatrixCBuffer Matrix = MatrixBuffer_;
	Obj->GetTransform()->GetTranslationMatrix(Matrix.world);
	TransposeMatrix(Matrix);

	// Update Buffers
	ParticleShader_->UpdateBuffer(VertexShader, 0, Matrix);
	ParticleShader_->SendBuffersToShader();

	// Send Textures
	ParticleShader_->SendTextureToShader(0, texture);

	// Render using shader
	Obj->Render();
	ParticleShader_ -> Render(indexCount);
	
	return true;
}

bool ShaderManager::SkyRender(SkySphere* Obj)
{
	Mesh* ObjMesh;
	
	ObjMesh = Obj->GetMesh();
	if (!ObjMesh)
	{
		MessageBox(NULL, L"No Model Attached - SkySphere", L"Error", MB_OK);
		return false;
	}

	// Model Properties
	int indexCount = ObjMesh->GetIndexCount();

	// Create Matrix Buffer
	MatrixCBuffer Matrix = MatrixBuffer_;
	Obj->GetTransform()->GetTranslationMatrix(Matrix.world);
	TransposeMatrix(Matrix);

	// Create the sky buffer
	GradientCBuffer Gradient;
	Gradient.topColor = Obj->GetTopColor();
	Gradient.centerColor = Obj->GetCenterColor();

	// Update Buffers
	SkySphereShader_->UpdateBuffer(VertexShader, 0, Matrix);
	SkySphereShader_->UpdateBuffer(PixelShader, 0, Gradient);
	SkySphereShader_->SendBuffersToShader();

	// Render using shader
	ObjMesh->Render();
	SkySphereShader_ -> Render(indexCount);
	
	return true;
}

bool ShaderManager::TerrainRender(Terrain* Obj)
{
	Mesh* ObjMesh;
	Material* ObjMat;

	// Model Properties
	ObjMesh = Obj->GetMesh();
	if (!ObjMesh)
	{
		MessageBox(NULL, L"No Model Attached - Terrain", L"Error", MB_OK);
		return false;
	}
	ObjMat = ObjMesh->GetMaterial();
	if (!ObjMat)
	{
		MessageBox(NULL, L"No Material Attached - Terrain", L"Error", MB_OK);
		return false;
	}

	// Retrieve variables for rendering
	int indexCount = ObjMesh -> GetIndexCount();
	ID3D11ShaderResourceView* colorTexture = ObjMat->GetBaseTexture();
	ID3D11ShaderResourceView* normalTexture = ObjMat->GetNormalTexture();

	// Create the light buffer
	LightCBuffer Light;
	Light.ambientColor = Light::Instance()->GetAmbientColor();
	Light.diffuseColor = Light::Instance()->GetDiffuseColor();
	Light.lightDirection = Light::Instance()->GetDirection();

	// Create the light position buffer
	LightPositionCBuffer LightPosition;
	LightPosition.lightPosition = D3DXVECTOR4(Light::Instance()->GetTransform()->GetPosition(), 1.0f);

	// Create matrix buffer
	MatrixCBuffer Matrix = MatrixBuffer_;
	Obj->GetTransform()->GetWorldMatrix(Matrix.world);
	TransposeMatrix(Matrix);

	// Pass the textures to the shaders
	TerrainShader_->SendTextureToShader(0, colorTexture);
	TerrainShader_->SendTextureToShader(1, normalTexture);

	// Update Buffers
	TerrainShader_->UpdateBuffer(VertexShader, 0, Matrix);
	TerrainShader_->UpdateBuffer(VertexShader, 1, LightPosition);
	TerrainShader_->UpdateBuffer(PixelShader, 0, Light);
	TerrainShader_->SendBuffersToShader();

	// Render using shader
	ObjMesh -> Render();
	TerrainShader_ -> Render(indexCount);
	
	return true;
}

bool ShaderManager::TerrainRender(Terrain* Obj, D3DXVECTOR4 clipPlane)
{
	Mesh* ObjMesh;
	Material* ObjMat;

	// Model Properties
	ObjMesh = Obj->GetMesh();
	if (!ObjMesh)
	{
		MessageBox(NULL, L"No Model Attached - Terrain", L"Error", MB_OK);
		return false;
	}
	ObjMat = ObjMesh->GetMaterial();
	if (!ObjMat)
	{
		MessageBox(NULL, L"No Material Attached - Terrain", L"Error", MB_OK);
		return false;
	}

	// Model Properties
	int indexCount = ObjMesh -> GetIndexCount();
	ID3D11ShaderResourceView* colorTexture = ObjMat->GetBaseTexture();
	ID3D11ShaderResourceView* normalTexture = ObjMat->GetNormalTexture();

	// Create the light buffer
	LightCBuffer Light;
	Light.ambientColor = Light::Instance()->GetAmbientColor();
	Light.diffuseColor = Light::Instance()->GetDiffuseColor();
	Light.lightDirection = Light::Instance()->GetDirection();

	// Create the light position buffer
	LightPositionCBuffer LightPosition;
	LightPosition.lightPosition = D3DXVECTOR4(Light::Instance()->GetTransform()->GetPosition(), 1.0f);

	// Create the clip plane buffer
	ClipPlaneCBuffer ClipPlane;
	ClipPlane.clipPlane = clipPlane;

	// Create matrix buffer
	MatrixCBuffer Matrix = MatrixBuffer_;
	Obj->GetTransform()->GetWorldMatrix(Matrix.world);
	TransposeMatrix(Matrix);

	// Pass the textures to the shader
	TerrainReflectionShader_->SendTextureToShader(0, colorTexture);
	TerrainReflectionShader_->SendTextureToShader(1, normalTexture);

	// Update Buffers
	TerrainReflectionShader_->UpdateBuffer(VertexShader, 0, Matrix);
	TerrainReflectionShader_->UpdateBuffer(VertexShader, 1, ClipPlane);
	TerrainReflectionShader_->UpdateBuffer(PixelShader, 0, Light);
	TerrainReflectionShader_->SendBuffersToShader();

	// Render using shader
	ObjMesh -> Render();
	TerrainReflectionShader_ -> Render(indexCount);
	
	return true;
}

bool ShaderManager::TextureRender(GameObject* Obj)
{
	Mesh* ObjMesh;
	Material* ObjMat;

	// Model Properties
	ObjMesh = Obj->GetMesh();
	if (!ObjMesh)
	{
		MessageBox(NULL, L"No Model Attached - Texture", L"Error", MB_OK);
		return false;
	}
	ObjMat = ObjMesh->GetMaterial();
	if (!ObjMat)
	{
		MessageBox(NULL, L"No Material Attached - Texture", L"Error", MB_OK);
		return false;
	}

	int indexCount = ObjMesh -> GetIndexCount();
	ID3D11ShaderResourceView* texture = ObjMat->GetTextureArray();

	// Create the world matrix for the model
	MatrixCBuffer Matrix = MatrixBuffer_;
	Obj->GetTransform()->GetWorldMatrix(Matrix.world);
	TransposeMatrix(Matrix);

	// Update Buffers
	TextureShader_->UpdateBuffer(VertexShader, 0, Matrix);
	TextureShader_->SendBuffersToShader();

	// Pass the texture into the shader
	TextureShader_->SendTextureToShader(0, texture);

	// Render using shader
	ObjMesh->Render();
	TextureShader_ -> Render(indexCount);
	
	return true;
}

bool ShaderManager::TextureRender(Sprite* Obj, Texture* render)
{
	Mesh* ObjMesh;
	Material* ObjMat;

	// Model Properties
	ObjMesh = Obj->GetMesh();
	if (!ObjMesh)
	{
		MessageBox(NULL, L"No Model Attached - Texture", L"Error", MB_OK);
		return false;
	}

	// Retrieve variables for rendering
	int indexCount = ObjMesh->GetIndexCount();
	ID3D11ShaderResourceView* texture = render -> GetTexture();

	// Create Matrix Buffer
	MatrixCBuffer Matrix = MatrixBuffer_;
	Obj->GetTransform()->GetWorldMatrix(Matrix.world);
	TransposeMatrix(Matrix);

	// Update Buffers
	TextureShader_->UpdateBuffer(VertexShader, 0, Matrix);
	TextureShader_->SendBuffersToShader();

	// Pass the texture into the shader
	TextureShader_->SendTextureToShader(0, texture);

	// Render using shader
	ObjMesh->Render();
	TextureShader_->Render(indexCount);
	
	return true;
}