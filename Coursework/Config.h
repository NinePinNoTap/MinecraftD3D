#pragma once

#include <d3dx11.h>
#include <d3dx10math.h>
#include <string>

using namespace std;

namespace Config
{
	class Global
	{
		public:
			static bool FullScreen;
			static bool UseVSync;
			static int ScreenHeight;
			static int ScreenWidth;
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

	class Rendering
	{
		public:
			static float NearClipPlane;
			static float FarClipPlane;
	};

	class World
	{
		public:
			static int ChunkSize;
			static int BlockSize;
			static int LoadRadius;
			static int ColumnHeight;
	};

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

	class Vector
	{
		public:
			static D3DXVECTOR3 Forward;
			static D3DXVECTOR3 Right;
			static D3DXVECTOR3 Up;
	};
}