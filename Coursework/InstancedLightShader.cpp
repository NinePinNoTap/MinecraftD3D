#include "InstancedLightShader.h"
#include "ShaderManager.h"

InstancedLightShader::InstancedLightShader() : GameShader()
{
}

InstancedLightShader::~InstancedLightShader()
{

}

bool InstancedLightShader::Initialise(HWND hwnd)
{
	// Define Shaders
	AddShader("instancedlight.vs");
	AddShader("instancedlight.ps");

	// Define Input Layout
	AddLayout("POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0);
	AddLayout("TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0);
	AddLayout("NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0);

	AddLayout("TEXCOORD", 1, DXGI_FORMAT_R32G32B32_FLOAT, 1, 0, D3D11_INPUT_PER_INSTANCE_DATA, 1);
	AddLayout("TEXCOORD", 2, DXGI_FORMAT_R32G32_FLOAT, 1, 12, D3D11_INPUT_PER_INSTANCE_DATA, 1);
	AddLayout("TEXCOORD", 3, DXGI_FORMAT_R32G32_FLOAT, 1, 20, D3D11_INPUT_PER_INSTANCE_DATA, 1);

	// Define Buffers
	AddBuffer<MatrixBuffer>(VertexShader);
	AddBuffer<CameraBuffer>(VertexShader);
	AddBuffer<LightPositionBuffer>(VertexShader);
	AddBuffer<LightBuffer>(PixelShader);

	// Define Sampler State
	AddSamplerState(D3D11_FILTER_MIN_MAG_MIP_LINEAR, D3D11_TEXTURE_ADDRESS_WRAP, 0.0f, 1, D3D11_COMPARISON_ALWAYS, D3DXVECTOR4(0, 0, 0, 0), 0, D3D11_FLOAT32_MAX);

	// Build Shader
	Result_ = BuildShader(hwnd);
	if (!Result_)
	{
		OutputToDebug("Could not initialise instanced light shader.");
		return false;
	}

	return true;
}

bool InstancedLightShader::Prepare(Mesh3D* objMesh, Material* objMaterial, Transform* objTransform)
{
	// Model Properties
	if (!objMesh)
	{
		MessageBox(NULL, L"No Model Attached - InstancedLight", L"Error", MB_OK);
		return false;
	}
	if (!objMaterial)
	{
		MessageBox(NULL, L"No Material Attached -  InstancedLight", L"Error", MB_OK);
		return false;
	}

	int indexCount = objMesh->GetIndexCount();
	ID3D11ShaderResourceView* texture = objMaterial->GetBaseTexture();

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
	lightBuffer.specularPower = Light::Instance()->GetSpecularPower();

	// Create light position buffer
	LightPositionBuffer lightPositionBuffer;
	lightPositionBuffer.lightPosition = D3DXVECTOR4(Light::Instance()->GetTransform()->GetPosition(), 1.0f);

	// Create matrix buffer
	MatrixBuffer matrixBuffer = ShaderManager::Instance()->GetMatrixBuffer();
	objTransform->GetWorldMatrix(matrixBuffer.world);
	TransposeMatrix(matrixBuffer);

	// Pass the texture to the shader
	SendTextureToShader(0, texture);

	// Update Buffers
	UpdateBuffer(VertexShader, 0, matrixBuffer);
	UpdateBuffer(VertexShader, 1, cameraBuffer);
	UpdateBuffer(VertexShader, 2, lightPositionBuffer);
	UpdateBuffer(PixelShader, 0, lightBuffer);
	SendBuffersToShader();

	return true;
}