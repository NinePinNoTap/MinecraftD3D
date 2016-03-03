#include "Sprite.h"
#include "DirectXManager.h"

Sprite::Sprite() : Primitive()
{
}

Sprite::Sprite(const Sprite& other)
{
}

Sprite::~Sprite()
{
}

bool Sprite::Initialise(Rect3D Dimensions)
{
	// Create the mesh
	Mesh_ = new PrimitiveMesh;
	if (!Mesh_)
	{
		return false;
	}

	Result_ = Mesh_->Create2DBox(Dimensions);
	if (!Result_)
	{
		return false;
	}

	Transform_ = new Transform;
	if (!Transform_)
	{
		return false;
	}

	return true;
}

bool Sprite::SetTexture(WCHAR* filename)
{
	// Create a texture
	Result_ = Mesh_->CreateMaterial(vector<wstring>(1, filename));
	if (!Result_)
	{
		return false;
	}

	return true;
}