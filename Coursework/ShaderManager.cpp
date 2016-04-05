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
	CloudShader_->AddShader("cloud.vs");
	CloudShader_->AddShader("cloud.ps");
	CloudShader_->AddLayout("POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0);
	CloudShader_->AddLayout("TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0);
	CloudShader_->AddLayout("NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0);
	CloudShader_->AddLayout("TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0);
	CloudShader_->AddLayout("BINORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0);
	CloudShader_->AddBuffer<MatrixBuffer>(VertexShader);
	CloudShader_->AddBuffer<SkyBuffer>(PixelShader);
	CloudShader_->AddSamplerState(D3D11_FILTER_MIN_MAG_MIP_LINEAR, D3D11_TEXTURE_ADDRESS_WRAP, 0.0f, 1, D3D11_COMPARISON_ALWAYS, Colour(0, 0, 0, 0), 0, D3D11_FLOAT32_MAX);
	Result_ = CloudShader_->BuildShader(hwnd);
	if (!Result_)
	{
		OutputToDebug("Could not initialise cloud shader.");
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
	ColourShader_->AddShader("colour.vs");
	ColourShader_->AddShader("colour.ps");
	ColourShader_->AddLayout("POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0);
	ColourShader_->AddLayout("TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0);
	ColourShader_->AddLayout("NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0);
	ColourShader_->AddLayout("TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0);
	ColourShader_->AddLayout("BINORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0);
	ColourShader_->AddBuffer<MatrixBuffer>(VertexShader);
	ColourShader_->AddBuffer<PixelBuffer>(PixelShader);
	Result_ = ColourShader_->BuildShader(hwnd);
	if (!Result_)
	{
		OutputToDebug("Could not initialise colour shader.");
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
	FireShader_->AddShader("fire.vs");
	FireShader_->AddShader("fire.ps");
	FireShader_->AddLayout("POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0);
	FireShader_->AddLayout("TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0);
	FireShader_->AddLayout("NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0);
	FireShader_->AddLayout("TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0);
	FireShader_->AddLayout("BINORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0);
	FireShader_->AddBuffer<MatrixBuffer>(VertexShader);
	FireShader_->AddBuffer<NoiseBuffer>(VertexShader);
	FireShader_->AddBuffer<DistortionBuffer>(PixelShader);
	FireShader_->AddSamplerState(D3D11_FILTER_MIN_MAG_MIP_LINEAR, D3D11_TEXTURE_ADDRESS_WRAP, 0.0f, 1, D3D11_COMPARISON_ALWAYS, Colour(0, 0, 0, 0), 0, D3D11_FLOAT32_MAX);
	FireShader_->AddSamplerState(D3D11_FILTER_MIN_MAG_MIP_LINEAR, D3D11_TEXTURE_ADDRESS_CLAMP, 0.0f, 1, D3D11_COMPARISON_ALWAYS, Colour(0, 0, 0, 0), 0, D3D11_FLOAT32_MAX);
	Result_ = FireShader_->BuildShader(hwnd);
	if (!Result_)
	{
		OutputToDebug("Could not initialise fire shader.");
		return false;
	}

	//============================
	// Initialise the Font Shader
	//============================

	FontShader_ = new GameShader;
	if (!FontShader_)
	{
		return false;
	}
	FontShader_->AddShader("font.vs");
	FontShader_->AddShader("font.ps");
	FontShader_->AddLayout("POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0);
	FontShader_->AddLayout("TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0);
	FontShader_->AddLayout("NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0);
	FontShader_->AddLayout("TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0);
	FontShader_->AddLayout("BINORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0);
	FontShader_->AddBuffer<MatrixBuffer>(VertexShader);
	FontShader_->AddBuffer<PixelBuffer>(PixelShader);
	FontShader_->AddSamplerState(D3D11_FILTER_MIN_MAG_MIP_LINEAR, D3D11_TEXTURE_ADDRESS_WRAP, 0.0f, 1, D3D11_COMPARISON_ALWAYS, Colour(0, 0, 0, 0), 0, D3D11_FLOAT32_MAX);
	Result_ = FontShader_->BuildShader(hwnd);
	if (!Result_)
	{
		OutputToDebug("Could not initialise font shader.");
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
	LightShader_->AddShader("light.vs");
	LightShader_->AddShader("light.ps");
	LightShader_->AddLayout("POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0);
	LightShader_->AddLayout("TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0);
	LightShader_->AddLayout("NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0);
	LightShader_->AddLayout("TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0);
	LightShader_->AddLayout("BINORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0);
	LightShader_->AddBuffer<MatrixBuffer>(VertexShader);
	LightShader_->AddBuffer<CameraBuffer>(VertexShader);
	LightShader_->AddBuffer<LightPositionBuffer>(VertexShader);
	LightShader_->AddBuffer<LightBuffer>(PixelShader);
	LightShader_->AddSamplerState(D3D11_FILTER_MIN_MAG_MIP_LINEAR, D3D11_TEXTURE_ADDRESS_WRAP, 0.0f, 1, D3D11_COMPARISON_ALWAYS, Colour(0, 0, 0, 0), 0, D3D11_FLOAT32_MAX);
	Result_ = LightShader_->BuildShader(hwnd);
	if (!Result_)
	{
		OutputToDebug("Could not initialise light shader.");
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
	OceanShader_->AddSamplerState(D3D11_FILTER_MIN_MAG_MIP_LINEAR, D3D11_TEXTURE_ADDRESS_WRAP, 0.0f, 1, D3D11_COMPARISON_ALWAYS, Colour(0, 0, 0, 0), 0, D3D11_FLOAT32_MAX);
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
	SkySphereShader_->AddShader("skysphere.vs");
	SkySphereShader_->AddShader("skysphere.ps");
	SkySphereShader_->AddLayout("POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0);
	SkySphereShader_->AddLayout("TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0);
	SkySphereShader_->AddLayout("NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0);
	SkySphereShader_->AddLayout("TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0);
	SkySphereShader_->AddLayout("BINORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0);
	SkySphereShader_->AddBuffer<MatrixBuffer>(VertexShader);
	SkySphereShader_->AddBuffer<GradientBuffer>(PixelShader);
	SkySphereShader_->AddSamplerState(D3D11_FILTER_MIN_MAG_MIP_LINEAR, D3D11_TEXTURE_ADDRESS_WRAP, 0.0f, 1, D3D11_COMPARISON_ALWAYS, Colour(0, 0, 0, 0), 0, D3D11_FLOAT32_MAX);
	Result_ = SkySphereShader_->BuildShader(hwnd);
	if (!Result_)
	{
		OutputToDebug("Could not initialise sky sphere shader.");
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
	TerrainShader_->AddShader("terrain.vs");
	TerrainShader_->AddShader("terrain.ps");
	TerrainShader_->AddLayout("POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0);
	TerrainShader_->AddLayout("TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0);
	TerrainShader_->AddLayout("NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0);
	TerrainShader_->AddLayout("TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0);
	TerrainShader_->AddLayout("BINORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0);
	TerrainShader_->AddBuffer<MatrixBuffer>(VertexShader);
	TerrainShader_->AddBuffer<LightPositionBuffer>(VertexShader);
	TerrainShader_->AddBuffer<LightBuffer>(PixelShader);
	TerrainShader_->AddSamplerState(D3D11_FILTER_MIN_MAG_MIP_LINEAR, D3D11_TEXTURE_ADDRESS_WRAP, 0.0f, 1, D3D11_COMPARISON_ALWAYS, Colour(0, 0, 0, 0), 0, D3D11_FLOAT32_MAX);
	Result_ = TerrainShader_->BuildShader(hwnd);
	if (!Result_)
	{
		OutputToDebug("Could not initialise terrain shader.");
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
	TerrainReflectionShader_->AddShader("terrainreflection.vs");
	TerrainReflectionShader_->AddShader("terrainreflection.ps");
	TerrainReflectionShader_->AddLayout("POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0);
	TerrainReflectionShader_->AddLayout("TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0);
	TerrainReflectionShader_->AddLayout("NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0);
	TerrainReflectionShader_->AddLayout("TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0);
	TerrainReflectionShader_->AddLayout("BINORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0);
	TerrainReflectionShader_->AddBuffer<MatrixBuffer>(VertexShader);
	TerrainReflectionShader_->AddBuffer<ClipPlaneBuffer>(VertexShader);
	TerrainReflectionShader_->AddBuffer<LightBuffer>(PixelShader);
	TerrainReflectionShader_->AddSamplerState(D3D11_FILTER_MIN_MAG_MIP_LINEAR, D3D11_TEXTURE_ADDRESS_WRAP, 0.0f, 1, D3D11_COMPARISON_ALWAYS, Colour(0, 0, 0, 0), 0, D3D11_FLOAT32_MAX);
	Result_ = TerrainReflectionShader_->BuildShader(hwnd);
	if (!Result_)
	{
		OutputToDebug("Could not initialise terrain reflection shader.");
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
	TextureShader_->AddShader("texture.vs");
	TextureShader_->AddShader("texture.ps");
	TextureShader_->AddLayout("POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0);
	TextureShader_->AddLayout("TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0);
	TextureShader_->AddLayout("NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0);
	TextureShader_->AddLayout("TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0);
	TextureShader_->AddLayout("BINORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0);
	TextureShader_->AddBuffer<MatrixBuffer>(VertexShader);
	TextureShader_->AddSamplerState(D3D11_FILTER_MIN_MAG_MIP_LINEAR, D3D11_TEXTURE_ADDRESS_WRAP, 0.0f, 1, D3D11_COMPARISON_ALWAYS, Colour(0,0,0,0), 0, D3D11_FLOAT32_MAX);
	Result_ = TextureShader_->BuildShader(hwnd);
	if (!Result_)
	{
		OutputToDebug("Could not initialise texture shader.");
		return false;
	}

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
