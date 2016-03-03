#include "Terrain.h"
#include "DirectXManager.h"

Terrain::Terrain() : GameObject()
{
}

Terrain::Terrain(const Terrain& other)
{
}

Terrain::~Terrain()
{
}

bool Terrain::Initialise(ObjSize Size, WCHAR* textureFilename, WCHAR* normalTextureFilename, Vector2 TextureRepeat, float Scale)
{
	// Create the mesh
	Mesh_ = new TerrainMesh;
	Result_ = Mesh_->CreateTerrain(Size, TextureRepeat, Scale);
	if (!Result_)
	{
		return false;
	}

	// Create material
	Result_ = Mesh_->CreateMaterial(textureFilename, normalTextureFilename);
	if (!Result_)
	{
		return false;
	}

	// Create the transform
	Transform_ = new Transform;
	if (!Transform_)
	{
		return false;
	}

	return true;
}

void Terrain::Shutdown()
{
	return;
}

TerrainMesh* Terrain::GetMesh()
{
	if (!Mesh_)
	{
		Mesh_ = new TerrainMesh;
	}

	return Mesh_;
}