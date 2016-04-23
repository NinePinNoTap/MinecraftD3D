#include "InstancedTextureShader.h"
#include "ShaderManager.h"

InstancedTextureShader::InstancedTextureShader() : GameShader()
{
}

InstancedTextureShader::~InstancedTextureShader()
{

}

bool InstancedTextureShader::Initialise(HWND hwnd)
{
	// Define Shaders
	AddShader("instancedtexture.vs");
	AddShader("instancedtexture.ps");

	// Define Input Layout
	AddLayout("POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0);
	AddLayout("TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0);

	AddLayout("TEXCOORD", 1, DXGI_FORMAT_R32G32B32_FLOAT, 1, 0, D3D11_INPUT_PER_INSTANCE_DATA, 1);
	AddLayout("TEXCOORD", 2, DXGI_FORMAT_R32G32_FLOAT, 1, 12, D3D11_INPUT_PER_INSTANCE_DATA, 1);
	AddLayout("TEXCOORD", 3, DXGI_FORMAT_R32G32_FLOAT, 1, 20, D3D11_INPUT_PER_INSTANCE_DATA, 1);

	// Define Buffers
	AddBuffer<MatrixBuffer>(VertexShader);

	// Define Sampler State
	AddSamplerState(D3D11_FILTER_MIN_MAG_MIP_POINT, D3D11_TEXTURE_ADDRESS_WRAP, 0.0f, 1, D3D11_COMPARISON_ALWAYS, D3DXVECTOR4(0, 0, 0, 0), 0, D3D11_FLOAT32_MAX);

	// Build Shader
	Result_ = BuildShader(hwnd);
	if (!Result_)
	{
		OutputToDebug("Could not initialise instanced texture shader.");
		return false;
	}

	return true;
}

bool InstancedTextureShader::Prepare(Material* objMaterial, Transform* objTransform)
{
	if (!objMaterial)
	{
		MessageBox(NULL, L"No Material Attached -  InstancedTexture", L"Error", MB_OK);
		return false;
	}

	ID3D11ShaderResourceView* baseTexture = objMaterial->GetBaseTexture();

	// Create matrix buffer
	MatrixBuffer matrixBuffer = MatrixBuffer_;
	objTransform->GetWorldMatrix(matrixBuffer.world);
	TransposeMatrix(matrixBuffer);

	// Update Buffers
	UpdateBuffer(VertexShader, 0, matrixBuffer);
	SendBuffersToShader();

	// Pass the texture to the shader
	SendTextureToShader(0, baseTexture);

	return true;
}