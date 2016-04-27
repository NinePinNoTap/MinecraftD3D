#include "Config.h"

//==================================================================
// Colour
//==================================================================

D3DXVECTOR4 Config::Colour::Black	= D3DXVECTOR4(0, 0, 0, 1);
D3DXVECTOR4 Config::Colour::White	= D3DXVECTOR4(1, 1, 1, 1);
D3DXVECTOR4 Config::Colour::Red		= D3DXVECTOR4(1, 0, 0, 1);
D3DXVECTOR4 Config::Colour::Green	= D3DXVECTOR4(0, 1, 0, 1);
D3DXVECTOR4 Config::Colour::Blue	= D3DXVECTOR4(0, 0, 1, 1);
D3DXVECTOR4 Config::Colour::Yellow	= D3DXVECTOR4(1, 1, 0, 1);
D3DXVECTOR4 Config::Colour::Water   = D3DXVECTOR4(0, 0.8f, 1, .5f);


//==================================================================
// Directories
//==================================================================

std::string Config::Directory::Audio	= "data/audio/";
std::string Config::Directory::Font		= "data/font/";
std::string Config::Directory::Model	= "data/models/";
std::string Config::Directory::Shader	= "data/shaders/";
std::string Config::Directory::Texture	= "data/textures/";


//==================================================================
// Physics
//==================================================================

float Config::Physics::Gravity = 9.80665f;


//==================================================================
// Rendering
//==================================================================

float Config::Rendering::NearClipPlane = 0.1f;
float Config::Rendering::FarClipPlane = 1000.0f;


//==================================================================
// System
//==================================================================

bool Config::System::FullScreen = false;
bool Config::System::UseVSync = true;
int Config::System::ScreenHeight = 720;
int Config::System::ScreenWidth = 1280;
D3DXVECTOR2 Config::System::CentreScreen = D3DXVECTOR2((FLOAT)GetSystemMetrics(SM_CXSCREEN) / 2, (FLOAT)GetSystemMetrics(SM_CYSCREEN) / 2);


//==================================================================
// Vectors
//==================================================================

D3DXVECTOR3 Config::Vector::Forward = D3DXVECTOR3(0, 0, 1);
D3DXVECTOR3 Config::Vector::Right = D3DXVECTOR3(1, 0, 0);
D3DXVECTOR3 Config::Vector::Up = D3DXVECTOR3(0, 1, 0);
D3DXVECTOR3 Config::Vector::Zero = D3DXVECTOR3(0, 0, 0);


//==================================================================
// World
//==================================================================

int Config::World::ChunkSize = 16;
int Config::World::BlockSize = 1;
int Config::World::LoadRadius = 1;
int Config::World::UnloadRadius = 3;
int Config::World::ColumnHeight = 8;
D3DXVECTOR3 Config::World::ChunkOffset = D3DXVECTOR3(ChunkSize/2, ChunkSize/2, ChunkSize/2);
std::map<string, Block> Config::World::Blocks =
{

	{ "air", Block("air", BlockType::Air, D3DXVECTOR2(0, 0), D3DXVECTOR2(16, 16), false) },
	{ "stone", Block("stone", BlockType::Stone, D3DXVECTOR2(1, 0), D3DXVECTOR2(16, 16), true) },
	{ "dirt", Block("dirt", BlockType::Dirt, D3DXVECTOR2(2, 0), D3DXVECTOR2(16, 16), true) },
	{ "wood", Block("wood", BlockType::Wood, D3DXVECTOR2(3, 0), D3DXVECTOR2(16, 16), true) },
	{ "cobblestone", Block("cobblestone", BlockType::Cobblestone, D3DXVECTOR2(0, 1), D3DXVECTOR2(16, 16), true) },
	{ "bedrock", Block("bedrock", BlockType::Bedrock, D3DXVECTOR2(1, 1), D3DXVECTOR2(16, 16), true) },
	{ "sand", Block("sand", BlockType::Sand, D3DXVECTOR2(2, 1), D3DXVECTOR2(16, 16), true) },
	{ "gravel", Block("gravel", BlockType::Gravel, D3DXVECTOR2(3, 1), D3DXVECTOR2(16, 16), true) },
	{ "water", Block("water", BlockType::Water, D3DXVECTOR2(10, 1), D3DXVECTOR2(16, 16), true) },
	{ "gold", Block("gold", BlockType::Gold, D3DXVECTOR2(0, 2), D3DXVECTOR2(16, 16), true) },
	{ "diamond", Block("diamond", BlockType::Diamond, D3DXVECTOR2(2, 3), D3DXVECTOR2(16, 16), true) },
	{ "lava", Block("lava", BlockType::Lava, D3DXVECTOR2(15, 15), D3DXVECTOR2(16, 16), true) }

};
