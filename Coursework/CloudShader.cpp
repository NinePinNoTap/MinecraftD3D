#include "CloudShader.h"
#include "ShaderManager.h"

CloudShader::CloudShader() : GameShader()
{
}

CloudShader::~CloudShader()
{

}

bool CloudShader::initialise(HWND hwnd)
{
	// Define Shaders
	addShader("cloud.vs");
	addShader("cloud.ps");

	// Define Input Layout
	addLayout("POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0);
	addLayout("TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0);
	addLayout("NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0);
	addLayout("TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0);
	addLayout("BINORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0);

	// Define Buffers
	addBuffer<MatrixBuffer>(VertexShader);
	addBuffer<SkyBuffer>(PixelShader);

	// Define Sampler State
	addSamplerState(D3D11_FILTER_MIN_MAG_MIP_LINEAR, D3D11_TEXTURE_ADDRESS_WRAP, 0.0f, 1, D3D11_COMPARISON_ALWAYS, D3DXVECTOR4(0, 0, 0, 0), 0, D3D11_FLOAT32_MAX);

	// build Shader
	m_result = buildShader(hwnd);
	if (!m_result)
	{
		outputToDebug("Could not initialise cloud shader.");
		return false;
	}

	return true;
}

bool CloudShader::prepare(Material* objMaterial, Transform* objTransform)
{
	if (!objMaterial)
	{
		MessageBox(NULL, L"No Material Attached - Clouds", L"Error", MB_OK);
		return false;
	}

	// Model Properties
	ID3D11ShaderResourceView* baseTexture = objMaterial->getTexture("BaseTexture")->getTexture();
	ID3D11ShaderResourceView* perturbTexture = objMaterial->getTexture("PerturbTexture")->getTexture();

	// create matrix buffer
	MatrixBuffer matrixBuffer = m_matrixBuffer;
	objTransform->getTranslationMatrix(matrixBuffer.world);
	transposeMatrixBuffer(matrixBuffer);

	// create the sky buffer
	SkyBuffer skyBuffer;
	skyBuffer.brightness = objMaterial->getFloat("TextureBrightness");
	skyBuffer.scale = objMaterial->getFloat("TextureScale");
	skyBuffer.translation = objMaterial->getFloat("update");
	skyBuffer.padding = 0.0f;

	// update Buffers
	updateBuffer(VertexShader, 0, matrixBuffer);
	updateBuffer(PixelShader, 0, skyBuffer);
	sendBuffersToShader();

	// Send Textures
	sendTextureToShader(0, baseTexture);
	sendTextureToShader(1, perturbTexture);

	return true;
}