#include "FireShader.h"
#include "ShaderManager.h"

FireShader::FireShader() : GameShader()
{
}

FireShader::~FireShader()
{

}

bool FireShader::Initialise(HWND hwnd)
{
	// Define Shaders
	AddShader("fire.vs");
	AddShader("fire.ps");
	
	// Define Input Layout
	AddLayout("POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0);
	AddLayout("TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0);
	AddLayout("NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0);
	AddLayout("TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0);
	AddLayout("BINORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0);
	
	// Define Buffers
	AddBuffer<MatrixBuffer>(VertexShader);
	AddBuffer<NoiseBuffer>(VertexShader);
	AddBuffer<DistortionBuffer>(PixelShader);
	
	// Define Sampler States
	AddSamplerState(D3D11_FILTER_MIN_MAG_MIP_LINEAR, D3D11_TEXTURE_ADDRESS_WRAP, 0.0f, 1, D3D11_COMPARISON_ALWAYS, D3DXVECTOR4(0, 0, 0, 0), 0, D3D11_FLOAT32_MAX);
	AddSamplerState(D3D11_FILTER_MIN_MAG_MIP_LINEAR, D3D11_TEXTURE_ADDRESS_CLAMP, 0.0f, 1, D3D11_COMPARISON_ALWAYS, D3DXVECTOR4(0, 0, 0, 0), 0, D3D11_FLOAT32_MAX);
	
	// Build Shader
	Result_ = BuildShader(hwnd);
	if (!Result_)
	{
		OutputToDebug("Could not initialise fire shader.");
		return false;
	}

	return true;
}

bool FireShader::Prepare(Material* objMaterial, Transform* objTransform)
{
	if (!objMaterial)
	{
		MessageBox(NULL, L"No Material Attached - Fire", L"Error", MB_OK);
		return false;
	}

	ID3D11ShaderResourceView* baseTexture = objMaterial->GetBaseTexture();
	ID3D11ShaderResourceView* noiseTexture = objMaterial->GetNoiseTexture();
	ID3D11ShaderResourceView* alphaTexture = objMaterial->GetAlphaTexture();

	// Create noise buffer
	NoiseBuffer noiseBuffer;
	noiseBuffer.frameTime = objMaterial->GetFloat("Frame");// gameObject->GetFrame();
	noiseBuffer.scrollSpeeds = objMaterial->GetVector3("TextureOffsetSpeed"); // gameObject->GetSpeed();
	noiseBuffer.scales = objMaterial->GetVector3("TextureTiling"); // gameObject->GetTiling();
	noiseBuffer.padding = 0.0f;

	// Create the distortion buffer
	DistortionBuffer distortionBuffer;
	distortionBuffer.distortion1 = objMaterial->GetVector2("DistortionA");
	distortionBuffer.distortion2 = objMaterial->GetVector2("DistortionB");
	distortionBuffer.distortion3 = objMaterial->GetVector2("DistortionC");
	distortionBuffer.distortionScale = objMaterial->GetFloat("DistortionAmount"); // gameObject->GetDistortionAmount();
	distortionBuffer.distortionBias = objMaterial->GetFloat("DistortionBias"); // gameObject->GetDistortionBias();

	// Create matrix buffer
	MatrixBuffer matrixBuffer = ShaderManager::Instance()->GetMatrixBuffer();
	objTransform->GetWorldMatrix(matrixBuffer.world);
	TransposeMatrix(matrixBuffer);

	// Update Buffers
	UpdateBuffer(VertexShader, 0, matrixBuffer);
	UpdateBuffer(VertexShader, 1, noiseBuffer);
	UpdateBuffer(PixelShader, 0, distortionBuffer);
	SendBuffersToShader();

	// Send baseTextures
	SendTextureToShader(0, baseTexture);
	SendTextureToShader(1, noiseTexture);
	SendTextureToShader(2, alphaTexture);

	return true;
}