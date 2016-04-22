#include "Config.h"

//==================================================================
// Config::Globals
//==================================================================

bool Config::Global::FullScreen		= false;
bool Config::Global::UseVSync		= true;
int Config::Global::ScreenHeight	= 720;
int Config::Global::ScreenWidth		= 1280;

//==================================================================
// Directories
//==================================================================

std::string Config::Directory::Audio	= "data/audio/";
std::string Config::Directory::Font		= "data/font/";
std::string Config::Directory::Model	= "data/models/";
std::string Config::Directory::Shader	= "data/shaders/";
std::string Config::Directory::Texture	= "data/textures/";

//==================================================================
// Camera
//==================================================================

float Config::Rendering::NearClipPlane = 0.1f;
float Config::Rendering::FarClipPlane = 1000.0f;

//==================================================================
// World
//==================================================================

int Config::World::ChunkSize	= 16;
int Config::World::BlockSize	= 1;
int Config::World::LoadRadius	= 4;
int Config::World::ColumnHeight = 8;

//==================================================================
// Colour
//==================================================================

D3DXVECTOR4 Config::Colour::Black	= D3DXVECTOR4(0, 0, 0, 1);
D3DXVECTOR4 Config::Colour::White	= D3DXVECTOR4(1, 1, 1, 1);
D3DXVECTOR4 Config::Colour::Red		= D3DXVECTOR4(1, 0, 0, 1);
D3DXVECTOR4 Config::Colour::Green	= D3DXVECTOR4(0, 1, 0, 1);
D3DXVECTOR4 Config::Colour::Blue	= D3DXVECTOR4(0, 0, 1, 1);
D3DXVECTOR4 Config::Colour::Yellow	= D3DXVECTOR4(1, 1, 0, 1);
D3DXVECTOR4 Config::Colour::Water	= D3DXVECTOR4(0, 0.8f, 1, .5f);

//==================================================================
// Vectors
//==================================================================

D3DXVECTOR3 Config::Vector::Forward = D3DXVECTOR3(0, 0, 1);
D3DXVECTOR3 Config::Vector::Right	= D3DXVECTOR3(1, 0, 0);
D3DXVECTOR3 Config::Vector::Up		= D3DXVECTOR3(0, 1, 0);
