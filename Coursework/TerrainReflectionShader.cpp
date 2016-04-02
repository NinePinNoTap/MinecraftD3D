#include "TerrainReflectionShader.h"
#include "ShaderManager.h"

TerrainReflectionShader::TerrainReflectionShader() : GameShader()
{
}

TerrainReflectionShader::~TerrainReflectionShader()
{

}

bool TerrainReflectionShader::Prepare(Mesh3D* objMesh, Material* objMaterial, Transform* objTransform)
{
	// Model Properties
	if (!objMesh)
	{
		MessageBox(NULL, L"No Model Attached - Terrain Reflection", L"Error", MB_OK);
		return false;
	}
	if (!objMaterial)
	{
		MessageBox(NULL, L"No Material Attached - Terrain Reflection", L"Error", MB_OK);
		return false;
	}

	// Model Properties
	int indexCount = objMesh->GetIndexCount();
	ID3D11ShaderResourceView* colorTexture = objMaterial->GetBaseTexture();
	ID3D11ShaderResourceView* normalTexture = objMaterial->GetNormalTexture();

	// Create the light buffer
	LightBuffer lightBuffer;
	lightBuffer.ambientColor = Light::Instance()->GetAmbientColor();
	lightBuffer.diffuseColor = Light::Instance()->GetDiffuseColor();
	lightBuffer.lightDirection = Light::Instance()->GetDirection();

	// Create the light position buffer
	LightPositionBuffer lightPositionBuffer;
	lightPositionBuffer.lightPosition = D3DXVECTOR4(Light::Instance()->GetTransform()->GetPosition(), 1.0f);

	// Create the clip plane buffer
	ClipPlaneBuffer clipPlaneBuffer;
	clipPlaneBuffer.clipPlane = objMaterial->GetVector4("ClipPlane");

	// Create matrix buffer
	MatrixBuffer matrixBuffer = ShaderManager::Instance()->GetMatrixBuffer();
	objTransform->GetWorldMatrix(matrixBuffer.world);
	TransposeMatrix(matrixBuffer);

	// Pass the textures to the shader
	SendTextureToShader(0, colorTexture);
	SendTextureToShader(1, normalTexture);

	// Update Buffers
	UpdateBuffer(VertexShader, 0, matrixBuffer);
	UpdateBuffer(VertexShader, 1, clipPlaneBuffer);
	UpdateBuffer(PixelShader, 0, lightBuffer);
	SendBuffersToShader();
}