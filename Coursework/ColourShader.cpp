#include "ColourShader.h"
#include "ShaderManager.h"

ColourShader::ColourShader() : GameShader()
{
}

ColourShader::~ColourShader()
{

}

bool ColourShader::initialise(HWND hwnd)
{
	// Define Shaders
	addShader("colour.vs");
	addShader("colour.ps");

	// Define Input Layout
	addLayout("POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0);
	addLayout("TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0);
	addLayout("NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0);
	addLayout("TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0);
	addLayout("BINORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0);
	
	// Define Buffers
	addBuffer<MatrixBuffer>(VertexShader);
	addBuffer<PixelBuffer>(PixelShader);
	
	// build Shader
	m_result = buildShader(hwnd);
	if (!m_result)
	{
		outputToDebug("Could not initialise colour shader.");
		return false;
	}

	return true;
}

bool ColourShader::prepare(Material* objMaterial, Transform* objTransform)
{
	if (!objMaterial)
	{
		MessageBox(NULL, L"No Material Attached - Colour", L"Error", MB_OK);
		return false;
	}

	// create the world matrix for the model
	MatrixBuffer matrixBuffer = m_matrixBuffer;
	objTransform->getWorldMatrix(matrixBuffer.world);
	transposeMatrixBuffer(matrixBuffer);

	// create pixel buffer
	PixelBuffer pixelBuffer;
	pixelBuffer.pixelColor = objMaterial->getVector4("BaseColour");

	// update Buffers
	updateBuffer(VertexShader, 0, matrixBuffer);
	updateBuffer(PixelShader, 0, pixelBuffer);
	sendBuffersToShader();

	return true;
}