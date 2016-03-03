#pragma once

#include <d3d11.h>
#include <d3dx10math.h>

// 2D Position Data Struct
struct Vector2
{
	Vector2()
	{
		x = y = 0;
	}

	Vector2(float _x, float _y)
	{
		x = _x;
		y = _y;
	}

	float x;
	float y;
};

// 3D Position Data Struct
struct Vector3
{
	Vector3()
	{
		x = y = z = 0; 
	}

	Vector3(float _x, float _y, float _z)
	{
		x = _x;
		y = _y;
		z = _z;
	}

	float x;
	float y;
	float z;
};

// RGBA Colour Data Struct
struct Colour
{
	Colour()
	{
		r = g = b = a = 1.0f;
	}

	Colour(float _r, float _g, float _b)
	{
		r = _r;
		g = _g;
		b = _b;
	}

	Colour(float _r, float _g, float _b, float _a)
	{
		r = _r;
		g = _g;
		b = _b;
		a = _a;
	}

	float r;
	float g;
	float b;
	float a;
};

// Data Struct for Representing a Screen Resolution
struct ScreenResolution
{
	ScreenResolution()
	{
		width = 0;
		height = 0;
	}

	ScreenResolution(int w, int h)
	{
		width = w;
		height = h;
	}

	int width;
	int height;
};



// Data Struct for Representing an objects size
struct ObjSize
{
	// Default
	ObjSize()
	{
		width = 0;
		height = 0;
		depth = 0;
	}

	ObjSize(float w, float h, float d = 0.0f)
	{
		width = w;
		height = h;
		depth = d;
	}

	float width;
	float height;
	float depth;
};

// Data Struct for Representing Vertex Data
struct VertexData
{
	VertexData()
	{
		position = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		texture = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		tangent = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		binormal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	}
	D3DXVECTOR3 position;
	D3DXVECTOR3 texture;
	D3DXVECTOR3 normal;
	D3DXVECTOR3 tangent;
	D3DXVECTOR3 binormal;
};

// Data Struct for Representing a Bounding Box
struct BoundingBox
{
	BoundingBox()
	{
		left = 0.0f;
		right = 0.0f;
		top = 0.0f;
		bottom = 0.0f;
		front = 0.0f;
		back = 0.0f;
	}

	BoundingBox(ObjSize size, D3DXVECTOR3 position)
	{
		left = position.x - (size.width / 2.0f);
		right = position.x + (size.width / 2.0f);
		top = position.y + (size.height / 2.0f);
		bottom = position.y - (size.height / 2.0f);
		front = position.z - (size.depth / 2.0f);
		back = position.z + (size.depth / 2.0f);
	}

	float left;
	float right;
	float top;
	float bottom;
	float front;
	float back;
};

// Data struct for Representing Sound Files
struct WaveHeaderType
{
	char chunkId[4];
	unsigned long chunkSize;
	char format[4];
	char subChunkId[4];
	unsigned long subChunkSize;
	unsigned short audioFormat;
	unsigned short numChannels;
	unsigned long sampleRate;
	unsigned long bytesPerSecond;
	unsigned short blockAlign;
	unsigned short bitsPerSample;
	char dataChunkId[4];
	unsigned long dataSize;
};


