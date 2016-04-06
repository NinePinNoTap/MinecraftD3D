#include "FontShader.h"
#include "ShaderManager.h"

FontShader::FontShader() : GameShader()
{
}

FontShader::~FontShader()
{

}

bool FontShader::Prepare(Mesh3D* objMesh, Material* objMaterial, Transform* objTransform)
{
	// Model Properties
	if (!objMesh)
	{
		MessageBox(NULL, L"No Model Attached - Font", L"Error", MB_OK);
		return false;
	}
	if (!objMaterial)
	{
		MessageBox(NULL, L"No Material Attached - Font", L"Error", MB_OK);
		return false;
	}

	// Model Properties
	int indexCount = objMesh->GetIndexCount();
	ID3D11ShaderResourceView* texture = objMaterial->GetBaseTexture();

	// Create the pixel buffer
	PixelBuffer pixelBuffer;
	pixelBuffer.pixelColor = objMaterial->GetVector4("BaseColour");

	// Create matrix buffer
	MatrixBuffer matrixBuffer = ShaderManager::Instance()->GetMatrixBuffer();
	TransposeMatrix(matrixBuffer);

	// Update Buffers
	UpdateBuffer(VertexShader, 0, matrixBuffer);
	UpdateBuffer(PixelShader, 0, pixelBuffer);
	SendBuffersToShader();

	// Pass the texture to the shader
	SendTextureToShader(0, texture);

	return true;
}