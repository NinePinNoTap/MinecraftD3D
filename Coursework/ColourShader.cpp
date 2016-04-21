#include "ColourShader.h"
#include "ShaderManager.h"

ColourShader::ColourShader() : GameShader()
{
}

ColourShader::~ColourShader()
{

}

bool ColourShader::Initialise(HWND hwnd)
{
	// Define Shaders
	AddShader("colour.vs");
	AddShader("colour.ps");

	// Define Input Layout
	AddLayout("POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0);
	AddLayout("TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0);
	AddLayout("NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0);
	AddLayout("TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0);
	AddLayout("BINORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0);
	
	// Define Buffers
	AddBuffer<MatrixBuffer>(VertexShader);
	AddBuffer<PixelBuffer>(PixelShader);
	
	// Build Shader
	Result_ = BuildShader(hwnd);
	if (!Result_)
	{
		OutputToDebug("Could not initialise colour shader.");
		return false;
	}

	return true;
}

bool ColourShader::Prepare(Material* objMaterial, Transform* objTransform)
{
	if (!objMaterial)
	{
		MessageBox(NULL, L"No Material Attached - Colour", L"Error", MB_OK);
		return false;
	}

	// Create the world matrix for the model
	MatrixBuffer matrixBuffer = MatrixBuffer_;
	objTransform->GetWorldMatrix(matrixBuffer.world);
	TransposeMatrix(matrixBuffer);

	// Create pixel buffer
	PixelBuffer pixelBuffer;
	pixelBuffer.pixelColor = objMaterial->GetVector4("BaseColour");

	// Update Buffers
	UpdateBuffer(VertexShader, 0, matrixBuffer);
	UpdateBuffer(PixelShader, 0, pixelBuffer);
	SendBuffersToShader();

	return true;
}