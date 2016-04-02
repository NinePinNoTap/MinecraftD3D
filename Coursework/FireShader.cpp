#include "FireShader.h"
#include "ShaderManager.h"

FireShader::FireShader() : GameShader()
{
}

FireShader::~FireShader()
{

}

bool FireShader::Prepare(Mesh3D* objMesh, Material* objMaterial, Transform* objTransform)
{
	// Model Properties
	if (!objMesh)
	{
		MessageBox(NULL, L"No Model Attached - Fire", L"Error", MB_OK);
		return false;
	}
	if (!objMaterial)
	{
		MessageBox(NULL, L"No Material Attached - Fire", L"Error", MB_OK);
		return false;
	}

	int indexCount = objMesh->GetIndexCount();
	ID3D11ShaderResourceView* fireTexture = objMaterial->GetBaseTexture();
	ID3D11ShaderResourceView* noiseTexture = objMaterial->GetNoiseTexture();
	ID3D11ShaderResourceView* alphaTexture = objMaterial->GetAlphaTexture();

	// Create noise buffer
	NoiseBuffer noiseBuffer;
	noiseBuffer.frameTime = objMaterial->GetFloat("Frame");// gameObject->GetFrame();
	noiseBuffer.scrollSpeeds = objMaterial->GetVector3("TextureOffsetSpeed"); // gameObject->GetSpeed();
	noiseBuffer.scales = objMaterial->GetVector3("TextureTiling"); // gameObject->GetTiling();
	noiseBuffer.padding = 0.0f;

	// Create the distortion buffer
	DistortionBuffer distortionBuffer;
	distortionBuffer.distortion1 = objMaterial->GetVector2("DistortionA");
	distortionBuffer.distortion2 = objMaterial->GetVector2("DistortionB");
	distortionBuffer.distortion3 = objMaterial->GetVector2("DistortionC");
	distortionBuffer.distortionScale = objMaterial->GetFloat("DistortionAmount"); // gameObject->GetDistortionAmount();
	distortionBuffer.distortionBias = objMaterial->GetFloat("DistortionBias"); // gameObject->GetDistortionBias();

	// Create matrix buffer
	MatrixBuffer matrixBuffer = ShaderManager::Instance()->GetMatrixBuffer();
	objTransform->GetWorldMatrix(matrixBuffer.world);
	TransposeMatrix(matrixBuffer);

	// Update Buffers
	UpdateBuffer(VertexShader, 0, matrixBuffer);
	UpdateBuffer(VertexShader, 1, noiseBuffer);
	UpdateBuffer(PixelShader, 0, distortionBuffer);
	SendBuffersToShader();

	// Send textures
	SendTextureToShader(0, fireTexture);
	SendTextureToShader(1, noiseTexture);
	SendTextureToShader(2, alphaTexture);

	// Render using shader
	Render(indexCount);
}