#pragma once

#include <d3dx11.h>
#include <d3dx10math.h>
#include <string>

//==========
// Graphics
//==========

const bool FULL_SCREEN = false;
const bool VSYNC_ENABLED = true;
const int SCREEN_HEIGHT = 720;
const int SCREEN_WIDTH = 1280;

//=============
// Directories
//=============

const std::string AUDIO_DIR = "data/audio/";
const std::string FONT_DIR = "data/font/";
const std::string MODEL_DIR = "data/models/";
const std::string SHADER_DIR = "data/shaders/";
const std::string TEXTURE_DIR = "data/textures/";

//===================
// Camera Properties
//===================

const float SCREEN_NEAR  = 0.1f;
const float SCREEN_DEPTH = 1000.0f;
const float CAMERA_SENSITIVITY  = 10.0f;

//===============
// Voxel Terrain
//===============

// How big the block model is
const int BLOCK_SIZE = 1;

// How many blocks in a chunk
const int NO_OF_BLOCKS_WIDTH = 16;
const int NO_OF_BLOCKS_HEIGHT = 16;
const int NO_OF_BLOCKS_DEPTH = 16;

// How many chunks in a terrain
const int NO_OF_CHUNKS_WIDTH = 8;
const int NO_OF_CHUNKS_HEIGHT = 4;
const int NO_OF_CHUNKS_DEPTH = 8;

// Total size of a chunk dimension
const int CHUNK_WIDTH = NO_OF_BLOCKS_WIDTH * BLOCK_SIZE;
const int CHUNK_HEIGHT = NO_OF_BLOCKS_HEIGHT * BLOCK_SIZE;
const int CHUNK_DEPTH = NO_OF_BLOCKS_DEPTH * BLOCK_SIZE;

// Total size of a terrain dimension
const int TERRAIN_WIDTH = CHUNK_WIDTH * NO_OF_CHUNKS_WIDTH;
const int TERRAIN_HEIGHT = CHUNK_HEIGHT * NO_OF_CHUNKS_HEIGHT;
const int TERRAIN_DEPTH = CHUNK_DEPTH * NO_OF_CHUNKS_DEPTH;

// Global size of the terrain
const int TERRAIN_VOLUME = NO_OF_BLOCKS_WIDTH * NO_OF_BLOCKS_HEIGHT * NO_OF_BLOCKS_DEPTH;

//=========
// Colours
//=========

const D3DXVECTOR4 BLACK = D3DXVECTOR4(0, 0, 0, 1);
const D3DXVECTOR4 WHITE = D3DXVECTOR4(1, 1, 1, 1);
const D3DXVECTOR4 RED = D3DXVECTOR4(1, 0, 0, 1);
const D3DXVECTOR4 GREEN = D3DXVECTOR4(0, 1, 0, 1);
const D3DXVECTOR4 BLUE = D3DXVECTOR4(0, 0, 1, 1);
const D3DXVECTOR4 YELLOW = D3DXVECTOR4(1, 1, 0, 1);

//============
// Directions
//============

const D3DXVECTOR3 FORWARD_VECTOR = D3DXVECTOR3(0, 0, 1);
const D3DXVECTOR3 BACK_VECTOR = -FORWARD_VECTOR;

const D3DXVECTOR3 RIGHT_VECTOR = D3DXVECTOR3(1, 0, 0);
const D3DXVECTOR3 LEFT_VECTOR = -RIGHT_VECTOR;

const D3DXVECTOR3 UP_VECTOR = D3DXVECTOR3(0, 1, 0);
const D3DXVECTOR3 DOWN_VECTOR = -UP_VECTOR;

const enum Direction
{
	Up, Down,
	Left, Right,
	Forward, Backward
};

//==============
// Virtual Keys
//==============

#define VK_0               0x30
#define VK_1               0x31
#define VK_2               0x32
#define VK_3               0x33
#define VK_4               0x34
#define VK_5               0x35
#define VK_6               0x36
#define VK_7               0x37
#define VK_8               0x38
#define VK_9               0x39
#define VK_A               0x41
#define VK_B               0x42
#define VK_C               0x43
#define VK_D               0x44
#define VK_E               0x45
#define VK_F               0x46
#define VK_G               0x47
#define VK_H               0x48
#define VK_I               0x49
#define VK_J               0x4A
#define VK_K               0x4B
#define VK_L               0x4C
#define VK_M               0x4D
#define VK_N               0x4E
#define VK_O               0x4F
#define VK_P               0x50
#define VK_Q               0x51
#define VK_R               0x52
#define VK_S               0x53
#define VK_T               0x54
#define VK_U               0x55
#define VK_V               0x56
#define VK_W               0x57
#define VK_X               0x58
#define VK_Y               0x59
#define VK_Z               0x5A
#define VK_SPACE          0x20
#define VK_LEFT           0x25
#define VK_UP             0x26
#define VK_RIGHT          0x27
#define VK_DOWN           0x28


