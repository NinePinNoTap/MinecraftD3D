#include "OceanShader.h"
#include "ShaderManager.h"

OceanShader::OceanShader() : GameShader()
{
}

OceanShader::~OceanShader()
{

}

bool OceanShader::initialise(HWND hwnd)
{
	// Define Shaders
	addShader("ocean.vs");
	addShader("ocean.ps");
	addShader("ocean.hs");
	addShader("ocean.ds");

	// Define Input Layout
	addLayout("POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0);
	addLayout("TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0);
	addLayout("NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0);
	addLayout("TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0);
	addLayout("BINORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0);

	// Define Buffers
	addBuffer<MatrixBuffer>(DomainShader);
	addBuffer<CameraBuffer>(DomainShader);
	addBuffer<WaveBuffer>(DomainShader);
	addBuffer<OceanBuffer>(PixelShader);
	addBuffer<TessellationBuffer>(HullShader);

	// Define Sample State
	addSamplerState(D3D11_FILTER_MIN_MAG_MIP_LINEAR, D3D11_TEXTURE_ADDRESS_WRAP, 0.0f, 1, D3D11_COMPARISON_ALWAYS, D3DXVECTOR4(0, 0, 0, 0), 0, D3D11_FLOAT32_MAX);

	// build Shader
	m_result = buildShader(hwnd);
	if (!m_result)
	{
		outputToDebug("Could not initialise ocean shader.");
		return false;
	}

	return true;
}

bool OceanShader::prepare(Material* objMaterial, Transform* objTransform)
{
	if (!objMaterial)
	{
		MessageBox(NULL, L"No Material Attached - Ocean", L"Error", MB_OK);
		return false;
	}

	// Retrieve variables for rendering
	ID3D11ShaderResourceView* refractionTexture = objMaterial->getTexture("RefractionTexture")->getTexture();
	ID3D11ShaderResourceView* reflectionTexture = objMaterial->getTexture("ReflectionTexture")->getTexture();
	ID3D11ShaderResourceView* normalTexture = objMaterial->getNormalTexture();

	// create camera buffer
	CameraBuffer cameraBuffer;
	cameraBuffer.cameraPosition = Camera::getInstance()->getTransform()->getPosition();
	cameraBuffer.normalMapTiling = objMaterial->getVector2("NormalMapTiling");// D3DXVECTOR2(0.01f, 0.02f);
	cameraBuffer.padding = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	// create ocean buffer
	OceanBuffer oceanBuffer;
	oceanBuffer.waterTranslation = objMaterial->getFloat("WaterTranslation");// 0.0f;
	oceanBuffer.reflectRefractScale = objMaterial->getFloat("ReflectRefractScale");// 0.03f;
	oceanBuffer.refractionTint = objMaterial->getVector4("RefractionTint");// D3DXVECTOR4(0.0f, 0.8f, 1.0f, 1.0f);
	oceanBuffer.lightDirection = Light::getInstance()->getDirection();
	oceanBuffer.specularShininess = Light::getInstance()->getSpecularPower();
	oceanBuffer.padding = D3DXVECTOR2(0.0f, 0.0f);

	// create wave buffer
	WaveBuffer waveBuffer;
	waveBuffer.waveTime = objMaterial->getFloat("update");
	waveBuffer.waveHeight = objMaterial->getFloat("WaveHeight") / 2;
	waveBuffer.padding = D3DXVECTOR2(0.0f, 0.0f);

	// create tessellation buffer
	TessellationBuffer tessellationBuffer;
	tessellationBuffer.tessellationAmount = objMaterial->getFloat("TessellationAmount");// 58;// gameObject->getTessellation();
	tessellationBuffer.padding = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	// create the world matrix for the model
	MatrixBuffer matrixBuffer = m_matrixBuffer;
	objTransform->getWorldMatrix(matrixBuffer.world);
	transposeMatrixBuffer(matrixBuffer);

	// Pass the baseTextures to the shader
	sendTextureToShader(0, refractionTexture);
	sendTextureToShader(1, reflectionTexture);
	sendTextureToShader(2, normalTexture);

	// update Buffers
	updateBuffer(DomainShader, 0, matrixBuffer);
	updateBuffer(DomainShader, 1, cameraBuffer);
	updateBuffer(DomainShader, 2, waveBuffer);
	updateBuffer(PixelShader, 0, oceanBuffer);
	updateBuffer(HullShader, 0, tessellationBuffer);
	sendBuffersToShader();

	return true;
}