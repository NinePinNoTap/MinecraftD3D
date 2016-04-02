#include "LightShader.h"
#include "ShaderManager.h"

LightShader::LightShader() : GameShader()
{
}

LightShader::~LightShader()
{

}

bool LightShader::Prepare(Mesh3D* objMesh, Material* objMaterial, Transform* objTransform)
{
	// Model Properties
	if (!objMesh)
	{
		MessageBox(NULL, L"No Model Attached - lightBuffer", L"Error", MB_OK);
		return false;
	}
	if (!objMaterial)
	{
		MessageBox(NULL, L"No Material Attached - lightBuffer", L"Error", MB_OK);
		return false;
	}

	int indexCount = objMesh->GetIndexCount();
	ID3D11ShaderResourceView* texture = objMaterial->GetBaseTexture();

	// Create camera buffer
	CameraBuffer cameraBuffer;
	cameraBuffer.cameraPosition = Camera::Instance()->GetTransform()->GetPosition();
	cameraBuffer.normalMapTiling = D3DXVECTOR2(0.0f, 0.0f);
	cameraBuffer.padding = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	// Create the light buffer
	LightBuffer lightBuffer;
	lightBuffer.lightDirection = Light::Instance()->GetDirection();
	lightBuffer.ambientColor = Light::Instance()->GetAmbientColor();
	lightBuffer.diffuseColor = Light::Instance()->GetDiffuseColor();
	lightBuffer.specularColor = Light::Instance()->GetSpecularColor();
	lightBuffer.specularPower = objMaterial->GetFloat("SpecularPower");

	// Create light position buffer
	LightPositionBuffer lightPositionBuffer;
	lightPositionBuffer.lightPosition = D3DXVECTOR4(Light::Instance()->GetTransform()->GetPosition(), 1.0f);

	// Create matrix buffer
	MatrixBuffer matrixBuffer = ShaderManager::Instance()->GetMatrixBuffer();
	objTransform->GetWorldMatrix(matrixBuffer.world);
	TransposeMatrix(matrixBuffer);

	// Pass the texture to the shader
	SendTextureToShader(0, texture);

	// Update Buffers
	UpdateBuffer(VertexShader, 0, matrixBuffer);
	UpdateBuffer(VertexShader, 1, cameraBuffer);
	UpdateBuffer(VertexShader, 2, lightPositionBuffer);
	UpdateBuffer(PixelShader, 0, lightBuffer);
	SendBuffersToShader();
}