#include "InstancedTextureShader.h"
#include "ShaderManager.h"

InstancedTextureShader::InstancedTextureShader() : GameShader()
{
}

InstancedTextureShader::~InstancedTextureShader()
{

}

bool InstancedTextureShader::initialise(HWND hwnd)
{
	// Define Shaders
	addShader("instancedtexture.vs");
	addShader("instancedtexture.ps");

	// Define Input Layout
	addLayout("POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0);
	addLayout("TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0);

	addLayout("TEXCOORD", 1, DXGI_FORMAT_R32G32B32_FLOAT, 1, 0, D3D11_INPUT_PER_INSTANCE_DATA, 1);
	addLayout("TEXCOORD", 2, DXGI_FORMAT_R32G32_FLOAT, 1, 12, D3D11_INPUT_PER_INSTANCE_DATA, 1);
	addLayout("TEXCOORD", 3, DXGI_FORMAT_R32G32_FLOAT, 1, 20, D3D11_INPUT_PER_INSTANCE_DATA, 1);

	// Define Buffers
	addBuffer<MatrixBuffer>(VertexShader);

	// Define Sampler State
	addSamplerState(D3D11_FILTER_MIN_MAG_MIP_POINT, D3D11_TEXTURE_ADDRESS_WRAP, 0.0f, 1, D3D11_COMPARISON_ALWAYS, D3DXVECTOR4(0, 0, 0, 0), 0, D3D11_FLOAT32_MAX);

	// build Shader
	m_result = buildShader(hwnd);
	if (!m_result)
	{
		outputToDebug("Could not initialise instanced texture shader.");
		return false;
	}

	return true;
}

bool InstancedTextureShader::prepare(Material* objMaterial, Transform* objTransform)
{
	if (!objMaterial)
	{
		MessageBox(NULL, L"No Material Attached -  InstancedTexture", L"Error", MB_OK);
		return false;
	}

	ID3D11ShaderResourceView* baseTexture = objMaterial->getBaseTexture();

	// create matrix buffer
	MatrixBuffer matrixBuffer = m_matrixBuffer;
	objTransform->getWorldMatrix(matrixBuffer.world);
	transposeMatrixBuffer(matrixBuffer);

	// update Buffers
	updateBuffer(VertexShader, 0, matrixBuffer);
	sendBuffersToShader();

	// Pass the texture to the shader
	sendTextureToShader(0, baseTexture);

	return true;
}