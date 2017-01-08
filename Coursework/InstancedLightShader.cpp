#include "InstancedLightShader.h"
#include "ShaderManager.h"

InstancedLightShader::InstancedLightShader() : GameShader()
{
}

InstancedLightShader::~InstancedLightShader()
{

}

bool InstancedLightShader::initialise(HWND hwnd)
{
	// Define Shaders
	addShader("instancedlight.vs");
	addShader("instancedlight.ps");

	// Define Input Layout
	addLayout("POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0);
	addLayout("TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0);
	addLayout("NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0);

	addLayout("TEXCOORD", 1, DXGI_FORMAT_R32G32B32_FLOAT, 1, 0, D3D11_INPUT_PER_INSTANCE_DATA, 1);
	addLayout("TEXCOORD", 2, DXGI_FORMAT_R32G32_FLOAT, 1, 12, D3D11_INPUT_PER_INSTANCE_DATA, 1);
	addLayout("TEXCOORD", 3, DXGI_FORMAT_R32G32_FLOAT, 1, 20, D3D11_INPUT_PER_INSTANCE_DATA, 1);

	// Define Buffers
	addBuffer<MatrixBuffer>(VertexShader);
	addBuffer<CameraBuffer>(VertexShader);
	addBuffer<LightPositionBuffer>(VertexShader);
	addBuffer<LightBuffer>(PixelShader);

	// Define Sampler State
	addSamplerState(D3D11_FILTER_MIN_MAG_MIP_POINT, D3D11_TEXTURE_ADDRESS_WRAP, 0.0f, 1, D3D11_COMPARISON_ALWAYS, D3DXVECTOR4(0, 0, 0, 0), 0, D3D11_FLOAT32_MAX);

	// build Shader
	m_result = buildShader(hwnd);
	if (!m_result)
	{
		outputToDebug("Could not initialise instanced light shader.");
		return false;
	}

	return true;
}

bool InstancedLightShader::prepare(Material* objMaterial, Transform* objTransform)
{
	if (!objMaterial)
	{
		MessageBox(NULL, L"No Material Attached -  InstancedLight", L"Error", MB_OK);
		return false;
	}

	ID3D11ShaderResourceView* baseTexture = objMaterial->getBaseTexture();

	// create camera buffer
	CameraBuffer cameraBuffer;
	cameraBuffer.cameraPosition = Camera::getInstance()->getTransform()->getPosition();
	cameraBuffer.normalMapTiling = D3DXVECTOR2(0.0f, 0.0f);
	cameraBuffer.padding = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	// create the light buffer
	LightBuffer lightBuffer;
	lightBuffer.lightDirection = Light::getInstance()->getDirection();
	lightBuffer.ambientColor = Light::getInstance()->getAmbientColor();
	lightBuffer.diffuseColor = Light::getInstance()->getDiffuseColor();
	lightBuffer.specularColor = Light::getInstance()->getSpecularColor();
	lightBuffer.specularPower = Light::getInstance()->getSpecularPower();

	// create light position buffer
	LightPositionBuffer lightPositionBuffer;
	lightPositionBuffer.lightPosition = D3DXVECTOR4(Light::getInstance()->getTransform()->getPosition(), 1.0f);

	// create matrix buffer
	MatrixBuffer matrixBuffer = m_matrixBuffer;
	objTransform->getWorldMatrix(matrixBuffer.world);
	transposeMatrixBuffer(matrixBuffer);

	// Pass the baseTexture to the shader
	sendTextureToShader(0, baseTexture);

	// update Buffers
	updateBuffer(VertexShader, 0, matrixBuffer);
	updateBuffer(VertexShader, 1, cameraBuffer);
	updateBuffer(VertexShader, 2, lightPositionBuffer);
	updateBuffer(PixelShader, 0, lightBuffer);

	sendBuffersToShader();

	return true;
}