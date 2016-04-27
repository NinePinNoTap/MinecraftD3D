#pragma once

#include <d3dx11.h>
#include <d3dx10math.h>
#include <map>
#include <string>

#include "Block.h"

using namespace std;

namespace Config
{
	class Colour
	{
	public:
		static D3DXVECTOR4 Black;
		static D3DXVECTOR4 White;
		static D3DXVECTOR4 Red;
		static D3DXVECTOR4 Green;
		static D3DXVECTOR4 Blue;
		static D3DXVECTOR4 Yellow;
		static D3DXVECTOR4 Water;
	};

	class Directory
	{
	public:
		static std::string Audio;
		static std::string Font;
		static std::string Model;
		static std::string Shader;
		static std::string Texture;
	};

	class Physics
	{
		public:
			static float Gravity;
	};

	class Rendering
	{
	public:
		static float NearClipPlane;
		static float FarClipPlane;
	};

	class System
	{
	public:
		static bool FullScreen;
		static bool UseVSync;
		static int ScreenHeight;
		static int ScreenWidth;
		static D3DXVECTOR2 CentreScreen;
	};

	class Vector
	{
		public:
			static D3DXVECTOR3 Forward;
			static D3DXVECTOR3 Right;
			static D3DXVECTOR3 Up;
			static D3DXVECTOR3 Zero;
	};
	
	class World
	{
	public:
		static int ChunkSize;
		static int BlockSize;
		static int LoadRadius;
		static int UnloadRadius;
		static int ColumnHeight;
		static int WaterHeight;
		static D3DXVECTOR3 ChunkOffset;
		static map<string, Block> Blocks;
	};
}