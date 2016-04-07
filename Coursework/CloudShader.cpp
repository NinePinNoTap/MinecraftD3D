#include "CloudShader.h"
#include "ShaderManager.h"

CloudShader::CloudShader() : GameShader()
{
}

CloudShader::~CloudShader()
{

}

bool CloudShader::Initialise(HWND hwnd)
{
	// Define Shaders
	AddShader("cloud.vs");
	AddShader("cloud.ps");

	// Define Input Layout
	AddLayout("POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0);
	AddLayout("TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0);
	AddLayout("NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0);
	AddLayout("TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0);
	AddLayout("BINORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0);

	// Define Buffers
	AddBuffer<MatrixBuffer>(VertexShader);
	AddBuffer<SkyBuffer>(PixelShader);

	// Define Sampler State
	AddSamplerState(D3D11_FILTER_MIN_MAG_MIP_LINEAR, D3D11_TEXTURE_ADDRESS_WRAP, 0.0f, 1, D3D11_COMPARISON_ALWAYS, D3DXVECTOR4(0, 0, 0, 0), 0, D3D11_FLOAT32_MAX);

	// Build Shader
	Result_ = BuildShader(hwnd);
	if (!Result_)
	{
		OutputToDebug("Could not initialise cloud shader.");
		return false;
	}

	return true;
}

bool CloudShader::Prepare(Mesh3D* objMesh, Material* objMaterial, Transform* objTransform)
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

	// Model Properties
	int indexCount = objMesh->GetIndexCount();
	ID3D11ShaderResourceView* cloudTexture = objMaterial->GetTexture("BaseTexture")->GetTexture();
	ID3D11ShaderResourceView* perturbTexture = objMaterial->GetTexture("PerturbTexture")->GetTexture();

	// Create matrix buffer
	MatrixBuffer matrixBuffer = ShaderManager::Instance()->GetMatrixBuffer();
	objTransform->GetTranslationMatrix(matrixBuffer.world);
	TransposeMatrix(matrixBuffer);

	// Create the sky buffer
	SkyBuffer skyBuffer;
	skyBuffer.brightness = objMaterial->GetFloat("TextureBrightness");
	skyBuffer.scale = objMaterial->GetFloat("TextureScale");
	skyBuffer.translation = objMaterial->GetFloat("Frame");
	skyBuffer.padding = 0.0f;

	// Update Buffers
	UpdateBuffer(VertexShader, 0, matrixBuffer);
	UpdateBuffer(PixelShader, 0, skyBuffer);
	SendBuffersToShader();

	// Send Textures
	SendTextureToShader(0, cloudTexture);
	SendTextureToShader(1, perturbTexture);

	return true;
}