#include "TintShader.h"
#include "Camera.h"

TintShader::TintShader()
{
}

TintShader::~TintShader()
{
}


bool TintShader::Initialise(HWND hwnd)
{
	// Define Shaders
	AddShader("tint.vs");
	AddShader("tint.ps");

	// Define Input Layout
	AddLayout("POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0);
	AddLayout("TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0);

	// Define Buffers
	AddBuffer<MatrixBuffer>(VertexShader);
	AddBuffer<PixelBuffer>(PixelShader);

	// Define Sampler State
	AddSamplerState(D3D11_FILTER_MIN_MAG_MIP_LINEAR, D3D11_TEXTURE_ADDRESS_WRAP, 0.0f, 1, D3D11_COMPARISON_ALWAYS, D3DXVECTOR4(0, 0, 0, 0), 0, D3D11_FLOAT32_MAX);

	// Build Shader
	Result_ = BuildShader(hwnd);
	if (!Result_)
	{
		OutputToDebug("Could not initialise tint shader.");
		return false;
	}

	return true;
}

bool TintShader::Prepare(Material* objMaterial, Transform* objTransform)
{
	if (!objMaterial)
	{
		MessageBox(NULL, L"No Material Attached - Font", L"Error", MB_OK);
		return false;
	}

	// Model Properties
	ID3D11ShaderResourceView* baseTexture = objMaterial->GetBaseTexture();

	// Create the pixel buffer
	PixelBuffer pixelBuffer;
	pixelBuffer.pixelColor = objMaterial->GetVector4("BaseColour");

	// Create matrix buffer
	MatrixBuffer matrixBuffer = MatrixBuffer_;
	objTransform->GetWorldMatrix(matrixBuffer.world);
	TransposeMatrix(matrixBuffer);

	// Update Buffers
	UpdateBuffer(VertexShader, 0, matrixBuffer);
	UpdateBuffer(PixelShader, 0, pixelBuffer);
	SendBuffersToShader();

	// Pass the baseTexture to the shader
	SendTextureToShader(0, baseTexture);

	return true;
}