#include "TextureShaderNEW.h"
#include "ShaderManager.h"

TextureShaderNEW::TextureShaderNEW() : GameShader()
{
}

TextureShaderNEW::~TextureShaderNEW()
{

}

bool TextureShaderNEW::Prepare(Mesh3D* objMesh, Material* objMaterial, Transform* objTransform)
{
	// Model Properties
	if (!objMesh)
	{
		MessageBox(NULL, L"No Model Attached - Texture", L"Error", MB_OK);
		return false;
	}
	if (!objMaterial)
	{
		MessageBox(NULL, L"No Material Attached - Texture", L"Error", MB_OK);
		return false;
	}

	int indexCount = objMesh->GetIndexCount();
	ID3D11ShaderResourceView* texture = objMaterial->GetBaseTexture();

	// Create the world matrix for the model
	MatrixBuffer matrixBuffer = ShaderManager::Instance()->GetMatrixBuffer();
	objTransform->GetWorldMatrix(matrixBuffer.world);
	TransposeMatrix(matrixBuffer);

	// Update Buffers
	UpdateBuffer(VertexShader, 0, matrixBuffer);
	SendBuffersToShader();

	// Pass the texture into the shader
	SendTextureToShader(0, texture);

	// Render using shader
	Render(indexCount);

	return true;
}