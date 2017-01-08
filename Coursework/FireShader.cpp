#include "FireShader.h"
#include "ShaderManager.h"

FireShader::FireShader() : GameShader()
{
}

FireShader::~FireShader()
{

}

bool FireShader::initialise(HWND hwnd)
{
	// Define Shaders
	addShader("fire.vs");
	addShader("fire.ps");
	
	// Define Input Layout
	addLayout("POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0);
	addLayout("TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0);
	addLayout("NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0);
	addLayout("TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0);
	addLayout("BINORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0);
	
	// Define Buffers
	addBuffer<MatrixBuffer>(VertexShader);
	addBuffer<NoiseBuffer>(VertexShader);
	addBuffer<DistortionBuffer>(PixelShader);
	
	// Define Sampler States
	addSamplerState(D3D11_FILTER_MIN_MAG_MIP_LINEAR, D3D11_TEXTURE_ADDRESS_WRAP, 0.0f, 1, D3D11_COMPARISON_ALWAYS, D3DXVECTOR4(0, 0, 0, 0), 0, D3D11_FLOAT32_MAX);
	addSamplerState(D3D11_FILTER_MIN_MAG_MIP_LINEAR, D3D11_TEXTURE_ADDRESS_CLAMP, 0.0f, 1, D3D11_COMPARISON_ALWAYS, D3DXVECTOR4(0, 0, 0, 0), 0, D3D11_FLOAT32_MAX);
	
	// build Shader
	m_result = buildShader(hwnd);
	if (!m_result)
	{
		outputToDebug("Could not initialise fire shader.");
		return false;
	}

	return true;
}

bool FireShader::prepare(Material* objMaterial, Transform* objTransform)
{
	if (!objMaterial)
	{
		MessageBox(NULL, L"No Material Attached - Fire", L"Error", MB_OK);
		return false;
	}

	ID3D11ShaderResourceView* baseTexture = objMaterial->getBaseTexture();
	ID3D11ShaderResourceView* noiseTexture = objMaterial->getNoiseTexture();
	ID3D11ShaderResourceView* alphaTexture = objMaterial->getAlphaTexture();

	// create noise buffer
	NoiseBuffer noiseBuffer;
	noiseBuffer.frameTime = objMaterial->getFloat("update");// gameObject->getFrame();
	noiseBuffer.scrollSpeeds = objMaterial->getVector3("TextureOffsetSpeed"); // gameObject->getSpeed();
	noiseBuffer.scales = objMaterial->getVector3("TextureTiling"); // gameObject->getTiling();
	noiseBuffer.padding = 0.0f;

	// create the distortion buffer
	DistortionBuffer distortionBuffer;
	distortionBuffer.distortion1 = objMaterial->getVector2("DistortionA");
	distortionBuffer.distortion2 = objMaterial->getVector2("DistortionB");
	distortionBuffer.distortion3 = objMaterial->getVector2("DistortionC");
	distortionBuffer.distortionScale = objMaterial->getFloat("DistortionAmount"); // gameObject->getDistortionAmount();
	distortionBuffer.distortionBias = objMaterial->getFloat("DistortionBias"); // gameObject->getDistortionBias();

	// create matrix buffer
	MatrixBuffer matrixBuffer = m_matrixBuffer;
	objTransform->getWorldMatrix(matrixBuffer.world);
	transposeMatrixBuffer(matrixBuffer);

	// update Buffers
	updateBuffer(VertexShader, 0, matrixBuffer);
	updateBuffer(VertexShader, 1, noiseBuffer);
	updateBuffer(PixelShader, 0, distortionBuffer);
	sendBuffersToShader();

	// Send baseTextures
	sendTextureToShader(0, baseTexture);
	sendTextureToShader(1, noiseTexture);
	sendTextureToShader(2, alphaTexture);

	return true;
}