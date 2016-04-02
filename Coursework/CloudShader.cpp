#include "CloudShader.h"
#include "ShaderManager.h"

CloudShader::CloudShader() : GameShader()
{
}

CloudShader::~CloudShader()
{

}

bool CloudShader::Prepare(Mesh3D* objMesh, Material* objMaterial, Transform* objTransform)
{
	// Model Properties
	if (!objMesh)
	{
		MessageBox(NULL, L"No Model Attached - Clouds", L"Error", MB_OK);
		return false;
	}
	if (!objMaterial)
	{
		MessageBox(NULL, L"No Material Attached - Clouds", L"Error", MB_OK);
		return false;
	}

	// Model Properties
	int indexCount = objMesh->GetIndexCount();
	ID3D11ShaderResourceView* cloudTexture = objMaterial->GetTexture("BaseTexture")->GetTexture();
	ID3D11ShaderResourceView* perturbTexture = objMaterial->GetTexture("PerturbTexture")->GetTexture();

	// Create matrix buffer
	MatrixBuffer matrixBuffer = ShaderManager::Instance()->GetMatrixBuffer();
	objTransform->GetTranslationMatrix(matrixBuffer.world);
	TransposeMatrix(matrixBuffer);

	// Create the sky buffer
	SkyBuffer skyBuffer;
	skyBuffer.brightness = objMaterial->GetFloat("TextureBrightness");
	skyBuffer.scale = objMaterial->GetFloat("TextureScale");
	skyBuffer.translation = objMaterial->GetFloat("Frame");// gameObject->GetFrame();
	skyBuffer.padding = 0.0f;

	// Update Buffers
	UpdateBuffer(VertexShader, 0, matrixBuffer);
	UpdateBuffer(PixelShader, 0, skyBuffer);
	SendBuffersToShader();

	// Send Textures
	SendTextureToShader(0, cloudTexture);
	SendTextureToShader(1, perturbTexture);

	// Render using shader
	Render(indexCount);
}