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

bool Terrain::initialise(Rect3D terrainSize, TerrainType type, string textureFilename, string normalTextureFilename, Vector2 textureRepeat, float terrainScale, float terrainSmoothing)
{
	TerrainFactory TerrainFactory;
	int terrainSeed;

	//==============
	// create Model
	//==============

	m_model = new Model;
	if (!m_model)
	{
		return false;
	}

	// create Terrain
	if (type == TerrainType::FlatTerrain)
	{
		TerrainFactory.createTerrain(terrainSize, textureRepeat, terrainScale, *m_model);
	}
	else if (type == TerrainType::PerlinTerrain)
	{
		terrainSeed = rand() % 100000;

		TerrainFactory.createTerrainWithPerlinNoise(terrainSize, textureRepeat, terrainScale, terrainSmoothing * 15, *m_model, terrainSeed);
	}

	//=================
	// create Material
	//=================

	Material* newMaterial = new Material;
	m_result = newMaterial->setBaseTexture(textureFilename);
	if (!m_result)
	{
		return false;
	}

	m_result = newMaterial->setNormalTexture(normalTextureFilename);
	if (!m_result)
	{
		return false;
	}

	m_model->addMaterial(newMaterial);

	//==================
	// create Transform
	//==================

	m_transform = new Transform;
	if (!m_transform)
	{
		return false;
	}

	//=================
	// initialise Vars
	//=================

	m_frame = 0;

	m_reflective = renderMode::Off;
	m_culled = renderMode::Off;
	m_depth = renderMode::On;
	m_blendMode = BlendMode::NoBlending;

	m_isActive = true;

	return true;
}