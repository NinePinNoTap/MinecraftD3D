#include "ColourShader.h"
#include "ShaderManager.h"

ColourShader::ColourShader() : GameShader()
{
}

ColourShader::~ColourShader()
{

}

bool ColourShader::Prepare(Mesh3D* objMesh, Material* objMaterial, Transform* objTransform)
{
	// Model Properties
	if (!objMesh)
	{
		MessageBox(NULL, L"No Model Attached - Colour", L"Error", MB_OK);
		return false;
	}
	if (!objMaterial)
	{
		MessageBox(NULL, L"No Material Attached - Colour", L"Error", MB_OK);
		return false;
	}

	int indexCount = objMesh->GetIndexCount();

	// Create the world matrix for the model
	MatrixBuffer matrixBuffer = ShaderManager::Instance()->GetMatrixBuffer();
	objTransform->GetWorldMatrix(matrixBuffer.world);
	TransposeMatrix(matrixBuffer);

	// Create pixel buffer
	PixelBuffer pixelBuffer;
	pixelBuffer.pixelColor = objMaterial->GetVector4("BaseColour");

	// Update Buffers
	UpdateBuffer(VertexShader, 0, matrixBuffer);
	UpdateBuffer(PixelShader, 0, pixelBuffer);
	SendBuffersToShader();

	Render(indexCount);
}