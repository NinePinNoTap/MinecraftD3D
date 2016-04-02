#pragma once

#include <d3dx11.h>
#include <d3dx10math.h>
#include <string>

#include "Colour.h"

//==========
// Graphics
//==========

const bool FULL_SCREEN = false;
const bool VSYNC_ENABLED = true;
const int SCREEN_HEIGHT = 720;
const int SCREEN_WIDTH = 1280;

//===================
// Camera Properties
//===================

const float SCREEN_NEAR  = 0.1f;
const float SCREEN_DEPTH = 10000.0f;
const float CAMERA_SENSITIVITY  = 10.0f;

//===============
// Voxel Terrain
//===============

const int BLOCK_SIZE = 2;

const int CHUNK_COUNT_WIDTH = 1;
const int CHUNK_COUNT_HEIGHT = 1;
const int CHUNK_COUNT_DEPTH = 1;

const int BLOCK_COUNT_WIDTH = 16;
const int BLOCK_COUNT_HEIGHT = 16;
const int BLOCK_COUNT_DEPTH = 16;

const int CHUNK_TOTAL_WIDTH = BLOCK_COUNT_WIDTH * BLOCK_SIZE;
const int CHUNK_TOTAL_HEIGHT = BLOCK_COUNT_HEIGHT * BLOCK_SIZE;
const int CHUNK_TOTAL_DEPTH = BLOCK_COUNT_DEPTH * BLOCK_SIZE;

const int TOTAL_BLOCKS_IN_CHUNK = BLOCK_COUNT_WIDTH * BLOCK_COUNT_HEIGHT * BLOCK_COUNT_DEPTH;

//=========
// Colours
//=========

const Colour BLACK = Colour(0, 0, 0, 1);
const Colour WHITE = Colour(1, 1, 1, 1);
const Colour RED = Colour(1, 0, 0, 1);
const Colour GREEN = Colour(0, 1, 0, 1);
const Colour BLUE = Colour(0, 0, 1, 1);
const Colour YELLOW = Colour(1, 1, 0, 1);

//===================
// Direction Vectors
//===================

const D3DXVECTOR3 FORWARD_VECTOR = D3DXVECTOR3(0, 0, 1);
const D3DXVECTOR3 RIGHT_VECTOR = D3DXVECTOR3(1, 0, 0);
const D3DXVECTOR3 UP_VECTOR = D3DXVECTOR3(0, 1, 0);

//=============
// Directories
//=============

const std::string AUDIO_DIR = "data/audio/";
const std::string FONT_DIR = "data/font/";
const std::string MODEL_DIR = "data/models/";
const std::string SHADER_DIR = "data/shaders/";
const std::string TEXTURE_DIR = "data/textures/";

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


