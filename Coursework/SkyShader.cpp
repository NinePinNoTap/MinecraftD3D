#include "SkyShader.h"
#include "ShaderManager.h"

SkyShader::SkyShader() : GameShader()
{
}

SkyShader::~SkyShader()
{

}

bool SkyShader::initialise(HWND hwnd)
{
	// Define Shaders
	addShader("skysphere.vs");
	addShader("skysphere.ps");

	// Define Input Layout
	addLayout("POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0);
	addLayout("TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0);
	addLayout("NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0);
	addLayout("TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0);
	addLayout("BINORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0);
	
	// Define Buffers
	addBuffer<MatrixBuffer>(VertexShader);
	addBuffer<GradientBuffer>(PixelShader);
	
	// Define Sampler State
	addSamplerState(D3D11_FILTER_MIN_MAG_MIP_LINEAR, D3D11_TEXTURE_ADDRESS_WRAP, 0.0f, 1, D3D11_COMPARISON_ALWAYS, D3DXVECTOR4(0, 0, 0, 0), 0, D3D11_FLOAT32_MAX);
	
	// build Shader
	m_result = buildShader(hwnd);
	if (!m_result)
	{
		outputToDebug("Could not initialise sky sphere shader.");
		return false;
	}

	return true;
}

bool SkyShader::prepare(Material* objMaterial, Transform* objTransform)
{
	if (!objMaterial)
	{
		MessageBox(NULL, L"No Material Attached - SkySphere", L"Error", MB_OK);
		return false;
	}

	// create matrixBuffer Buffer
	MatrixBuffer matrixBuffer = m_matrixBuffer;
	objTransform->getTranslationMatrix(matrixBuffer.world);
	transposeMatrixBuffer(matrixBuffer);

	// create the sky buffer
	GradientBuffer gradientBuffer;
	gradientBuffer.topColor = objMaterial->getVector4("TopColour");
	gradientBuffer.centerColor = objMaterial->getVector4("CenterColour");

	// update Buffers
	updateBuffer(VertexShader, 0, matrixBuffer);
	updateBuffer(PixelShader, 0, gradientBuffer);
	sendBuffersToShader();

	return true;
}