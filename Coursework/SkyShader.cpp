#include "SkyShader.h"
#include "ShaderManager.h"

SkyShader::SkyShader() : GameShader()
{
}

SkyShader::~SkyShader()
{

}

bool SkyShader::Prepare(Mesh3D* objMesh, Material* objMaterial, Transform* objTransform)
{
	if (!objMesh)
	{
		MessageBox(NULL, L"No Model Attached - SkySphere", L"Error", MB_OK);
		return false;
	}
	if (!objMaterial)
	{
		MessageBox(NULL, L"No Material Attached - SkySphere", L"Error", MB_OK);
		return false;
	}

	// Model Properties
	int indexCount = objMesh->GetIndexCount();

	// Create matrixBuffer Buffer
	MatrixBuffer matrixBuffer = ShaderManager::Instance()->GetMatrixBuffer();
	objTransform->GetTranslationMatrix(matrixBuffer.world);
	TransposeMatrix(matrixBuffer);

	// Create the sky buffer
	GradientBuffer gradientBuffer;
	gradientBuffer.topColor = objMaterial->GetVector4("TopColour");
	gradientBuffer.centerColor = objMaterial->GetVector4("CenterColour");

	// Update Buffers
	UpdateBuffer(VertexShader, 0, matrixBuffer);
	UpdateBuffer(PixelShader, 0, gradientBuffer);
	SendBuffersToShader();

	// Render using shader
	Render(indexCount);
}