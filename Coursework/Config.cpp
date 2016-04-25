#include "Config.h"

//==================================================================
// Config::Globals
//==================================================================

bool Config::System::FullScreen		= false;
bool Config::System::UseVSync		= true;
int Config::System::ScreenHeight	= 720;
int Config::System::ScreenWidth		= 1280;
D3DXVECTOR2 Config::System::CentreScreen = D3DXVECTOR2((FLOAT)GetSystemMetrics(SM_CXSCREEN) / 2, (FLOAT)GetSystemMetrics(SM_CYSCREEN) / 2);

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
int Config::World::LoadRadius	= 1;
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
// Physics
//==================================================================

float Config::Physics::Gravity = 9.80665f;

//==================================================================
// Vectors
//==================================================================

D3DXVECTOR3 Config::Vector::Forward = D3DXVECTOR3(0, 0, 1);
D3DXVECTOR3 Config::Vector::Right	= D3DXVECTOR3(1, 0, 0);
D3DXVECTOR3 Config::Vector::Up		= D3DXVECTOR3(0, 1, 0);
D3DXVECTOR3 Config::Vector::Zero	= D3DXVECTOR3(0, 0, 0);
