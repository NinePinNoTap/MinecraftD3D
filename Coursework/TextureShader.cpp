#include "TextureShader.h"
#include "ShaderManager.h"

TextureShader::TextureShader() : GameShader()
{
}

TextureShader::~TextureShader()
{

}

bool TextureShader::initialise(HWND hwnd)
{
	// Define Shaders
	addShader("texture.vs");
	addShader("texture.ps");

	// Define Input Layout
	addLayout("POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0);
	addLayout("TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0);
	addLayout("NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0);
	addLayout("TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0);
	addLayout("BINORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0);

	// Define Buffers
	addBuffer<MatrixBuffer>(VertexShader);

	// Define Sampler State
	addSamplerState(D3D11_FILTER_MIN_MAG_MIP_POINT, D3D11_TEXTURE_ADDRESS_WRAP, 0.0f, 1, D3D11_COMPARISON_ALWAYS, D3DXVECTOR4(0, 0, 0, 0), 0, D3D11_FLOAT32_MAX);

	// build Shader
	m_result = buildShader(hwnd);
	if (!m_result)
	{
		outputToDebug("Could not initialise Texture shader.");
		return false;
	}

	return true;
}

bool TextureShader::prepare(Material* objMaterial, Transform* objTransform)
{
	if (!objMaterial)
	{
		MessageBox(NULL, L"No Material Attached - Texture", L"Error", MB_OK);
		return false;
	}

	ID3D11ShaderResourceView* baseTexture = objMaterial->getBaseTexture();

	// create the world matrix for the model
	MatrixBuffer matrixBuffer = m_matrixBuffer;
	objTransform->getWorldMatrix(matrixBuffer.world);
	transposeMatrixBuffer(matrixBuffer);

	// update Buffers
	updateBuffer(VertexShader, 0, matrixBuffer);
	sendBuffersToShader();

	// Pass the baseTexture into the shader
	sendTextureToShader(0, baseTexture);

	return true;
}