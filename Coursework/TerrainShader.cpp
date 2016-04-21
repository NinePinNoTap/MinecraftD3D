#include "TerrainShader.h"
#include "ShaderManager.h"

TerrainShader::TerrainShader() : GameShader()
{
}

TerrainShader::~TerrainShader()
{

}

bool TerrainShader::Initialise(HWND hwnd)
{
	// Define Shaders
	AddShader("terrain.vs");
	AddShader("terrain.ps");

	// Define Input Layout
	AddLayout("POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0);
	AddLayout("TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0);
	AddLayout("NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0);
	AddLayout("TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0);
	AddLayout("BINORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0);

	// Define Buffers
	AddBuffer<MatrixBuffer>(VertexShader);
	AddBuffer<LightPositionBuffer>(VertexShader);
	AddBuffer<LightBuffer>(PixelShader);

	// Define Sampler State
	AddSamplerState(D3D11_FILTER_MIN_MAG_MIP_LINEAR, D3D11_TEXTURE_ADDRESS_WRAP, 0.0f, 1, D3D11_COMPARISON_ALWAYS, D3DXVECTOR4(0, 0, 0, 0), 0, D3D11_FLOAT32_MAX);

	// Build Shader
	Result_ = BuildShader(hwnd);
	if (!Result_)
	{
		OutputToDebug("Could not initialise terrain shader.");
		return false;
	}

	return true;
}

bool TerrainShader::Prepare(Material* objMaterial, Transform* objTransform)
{
	if (!objMaterial)
	{
		MessageBox(NULL, L"No Material Attached - Terrain", L"Error", MB_OK);
		return false;
	}

	// Retrieve variables for rendering
	ID3D11ShaderResourceView* baseTexture = objMaterial->GetBaseTexture();
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
	objTransform->GetWorldMatrix(matrixBuffer.world);
	TransposeMatrix(matrixBuffer);

	// Pass the baseTextures to the shaders
	SendTextureToShader(0, baseTexture);
	SendTextureToShader(1, normalTexture);

	// Update Buffers
	UpdateBuffer(VertexShader, 0, matrixBuffer);
	UpdateBuffer(VertexShader, 1, lightPositionBuffer);
	UpdateBuffer(PixelShader, 0, lightBuffer);
	SendBuffersToShader();

	return true;
}