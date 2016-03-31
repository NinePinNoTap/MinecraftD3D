#include "TextureArray.h"
#include "DirectXManager.h"

TextureArray::TextureArray()
{
	// Initialise the pointer to null
	Textures_ = 0;
}


TextureArray::TextureArray(const TextureArray& other)
{
}

TextureArray::~TextureArray()
{
}

bool TextureArray::Initialise(vector<wstring> TextureFilenames)
{
	HRESULT Result_;
	UINT TextureCount;
	
	// Count how many textures there are
	TextureCount = TextureFilenames.size();

	// Create a vector to store the textures
	std::vector<ID3D11Texture2D*> srcTex(TextureCount);

	// Loop through each texture
	for(UINT i = 0; i < TextureCount; ++i)
	{
		D3DX11_IMAGE_LOAD_INFO loadInfo;

        loadInfo.Width  = D3DX11_FROM_FILE;
        loadInfo.Height = D3DX11_FROM_FILE;
        loadInfo.Depth  = D3DX11_FROM_FILE;
        loadInfo.FirstMipLevel = 0;
        loadInfo.MipLevels = D3DX11_FROM_FILE;
        loadInfo.Usage = D3D11_USAGE_STAGING;
        loadInfo.BindFlags = 0;
        loadInfo.CpuAccessFlags = D3D11_CPU_ACCESS_WRITE | D3D11_CPU_ACCESS_READ;
        loadInfo.MiscFlags = 0;
		loadInfo.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		loadInfo.Filter = D3DX11_FILTER_NONE;
		loadInfo.MipFilter = D3DX11_FILTER_NONE;
		loadInfo.pSrcInfo  = 0;

		// Create a texture using the file name using the above description
		Result_ = D3DX11CreateTextureFromFile(DirectXManager::Instance()->GetDevice(), TextureFilenames[i].c_str(), &loadInfo, 0, (ID3D11Resource**)&srcTex[i], 0);
		if(FAILED(Result_)) { return false; }
	}

	// Get a description of the first image in the array
	D3D11_TEXTURE2D_DESC texElementDesc;
	srcTex[0] -> GetDesc(&texElementDesc);

	// Create a description for the texture array
	D3D11_TEXTURE2D_DESC texArrayDesc;
	texArrayDesc.Width              = texElementDesc.Width;
	texArrayDesc.Height             = texElementDesc.Height;
	texArrayDesc.MipLevels          = texElementDesc.MipLevels;
	texArrayDesc.ArraySize          = TextureCount;
	texArrayDesc.Format             = texElementDesc.Format;
	texArrayDesc.SampleDesc.Count   = 1;
	texArrayDesc.SampleDesc.Quality = 0;
	texArrayDesc.Usage              = D3D11_USAGE_DEFAULT;
	texArrayDesc.BindFlags          = D3D11_BIND_SHADER_RESOURCE;
	texArrayDesc.CPUAccessFlags     = 0;
	texArrayDesc.MiscFlags          = 0;

	// Create a texture using tha above description
	ID3D11Texture2D* texArray = 0;
	Result_ = DirectXManager::Instance()->GetDevice()->CreateTexture2D(&texArrayDesc, 0, &texArray);
	if(FAILED(Result_))
		return false;

	// Insert textures into the texture array
	for(UINT texElement = 0; texElement < TextureCount; texElement++)
	{
		for(UINT mipLevel = 0; mipLevel < texElementDesc.MipLevels; ++mipLevel)
		{
			D3D11_MAPPED_SUBRESOURCE mappedTex2D;
			Result_ = DirectXManager::Instance()->GetDeviceContext()->Map(srcTex[texElement], mipLevel, D3D11_MAP_READ, 0, &mappedTex2D);

			DirectXManager::Instance()->GetDeviceContext()->UpdateSubresource(texArray, D3D11CalcSubresource(mipLevel, texElement, texElementDesc.MipLevels),
											0, mappedTex2D.pData, mappedTex2D.RowPitch, mappedTex2D.DepthPitch);

			DirectXManager::Instance()->GetDeviceContext()->Unmap(srcTex[texElement], mipLevel);
		}
	}	

	// Describe the shader resource view output we want
	D3D11_SHADER_RESOURCE_VIEW_DESC viewDesc;
	viewDesc.Format = texArrayDesc.Format;
	viewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DARRAY;
	viewDesc.Texture2DArray.MostDetailedMip = 0;
	viewDesc.Texture2DArray.MipLevels = texArrayDesc.MipLevels;
	viewDesc.Texture2DArray.FirstArraySlice = 0;
	viewDesc.Texture2DArray.ArraySize = TextureCount;

	// Create the shader resource view for the texture array
	Result_ = DirectXManager::Instance()->GetDevice()->CreateShaderResourceView(texArray, &viewDesc, &Textures_);
	if(FAILED(Result_)) { return false; }

	// Cleanup
	texArray -> Release();
	for(UINT i = 0; i < TextureCount; i++)
	{
		srcTex[i] -> Release();
	}

	srcTex.clear();

	return true;
}

void TextureArray::Shutdown()
{
	// Release the texture resource
	if(Textures_)
	{
		Textures_ -> Release();
		Textures_ = 0;
	}
	
	return;
}

// Getter
ID3D11ShaderResourceView* TextureArray::GetTextures()
{
	return Textures_;
}