#include "OceanShader.h"
#include "ShaderManager.h"

OceanShader::OceanShader() : GameShader()
{
}

OceanShader::~OceanShader()
{

}

bool OceanShader::Initialise(HWND hwnd)
{
	// Define Shaders
	AddShader("ocean.vs");
	AddShader("ocean.ps");
	AddShader("ocean.hs");
	AddShader("ocean.ds");

	// Define Input Layout
	AddLayout("POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0);
	AddLayout("TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0);
	AddLayout("NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0);
	AddLayout("TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0);
	AddLayout("BINORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0);

	// Define Buffers
	AddBuffer<MatrixBuffer>(DomainShader);
	AddBuffer<CameraBuffer>(DomainShader);
	AddBuffer<WaveBuffer>(DomainShader);
	AddBuffer<OceanBuffer>(PixelShader);
	AddBuffer<TessellationBuffer>(HullShader);

	// Define Sample State
	AddSamplerState(D3D11_FILTER_MIN_MAG_MIP_LINEAR, D3D11_TEXTURE_ADDRESS_WRAP, 0.0f, 1, D3D11_COMPARISON_ALWAYS, D3DXVECTOR4(0, 0, 0, 0), 0, D3D11_FLOAT32_MAX);

	// Build Shader
	Result_ = BuildShader(hwnd);
	if (!Result_)
	{
		OutputToDebug("Could not initialise ocean shader.");
		return false;
	}

	return true;
}

bool OceanShader::Prepare(Mesh3D* objMesh, Material* objMaterial, Transform* objTransform)
{
	// Model Properties
	if (!objMesh)
	{
		MessageBox(NULL, L"No Model Attached - Clouds", L"Error", MB_OK);
		return false;
	}
	if (!objMaterial)
	{
		MessageBox(NULL, L"No Material Attached - Clouds", L"Error", MB_OK);
		return false;
	}

	// Retrieve variables for rendering
	int indexCount = objMesh->GetIndexCount();
	ID3D11ShaderResourceView* refractionTexture = objMaterial->GetTexture("RefractionTexture")->GetTexture();
	ID3D11ShaderResourceView* reflectionTexture = objMaterial->GetTexture("ReflectionTexture")->GetTexture();
	ID3D11ShaderResourceView* normalTexture = objMaterial->GetNormalTexture();

	/*WaterHeight_ = waterResolution.depth;
	NormalMapTiling_ = D3DXVECTOR2(0.01f, 0.02f);
	Frame_ = 0.0f;
	WaterTranslation_ = 0.0f;
	ReflectRefractScale_ = 0.03f;
	RefractionTint_ = D3DXVECTOR4(0.0f, 0.8f, 1.0f, 1.0f);
	WaterShininess_ = 50.0f;
	WaveHeight_ = 1.5f;
	WaveSpeed_ = 0.025f;
	Tessellation_ = 58.0f;*/

	// Create camera buffer
	CameraBuffer cameraBuffer;
	cameraBuffer.cameraPosition = Camera::Instance()->GetTransform()->GetPosition();
	cameraBuffer.normalMapTiling = D3DXVECTOR2(0.01f, 0.02f);
	cameraBuffer.padding = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	// Create ocean buffer
	OceanBuffer oceanBuffer;
	oceanBuffer.waterTranslation = 0.0f;
	oceanBuffer.reflectRefractScale = 0.03f;
	oceanBuffer.refractionTint = D3DXVECTOR4(0.0f, 0.8f, 1.0f, 1.0f);
	oceanBuffer.lightDirection = Light::Instance()->GetDirection();
	oceanBuffer.specularShininess = Light::Instance()->GetSpecularPower();
	oceanBuffer.padding = D3DXVECTOR2(0.0f, 0.0f);

	// Create wave buffer
	WaveBuffer waveBuffer;
	waveBuffer.waveTime = 0.0f;
	waveBuffer.waveHeight = 1.5f / 2;
	waveBuffer.padding = D3DXVECTOR2(0.0f, 0.0f);

	// Create tessellation buffer
	TessellationBuffer tessellationBuffer;
	tessellationBuffer.tessellationAmount = 58;// gameObject->GetTessellation();
	tessellationBuffer.padding = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	// Create the world matrix for the model
	MatrixBuffer matrixBuffer = ShaderManager::Instance()->GetMatrixBuffer();
	objTransform->GetWorldMatrix(matrixBuffer.world);
	TransposeMatrix(matrixBuffer);

	// Pass the textures to the shader
	SendTextureToShader(0, refractionTexture);
	SendTextureToShader(1, reflectionTexture);
	SendTextureToShader(2, normalTexture);

	// Update Buffers
	UpdateBuffer(DomainShader, 0, matrixBuffer);
	UpdateBuffer(DomainShader, 1, cameraBuffer);
	UpdateBuffer(DomainShader, 2, waveBuffer);
	UpdateBuffer(PixelShader, 0, oceanBuffer);
	UpdateBuffer(HullShader, 0, tessellationBuffer);
	SendBuffersToShader();

	return true;
}