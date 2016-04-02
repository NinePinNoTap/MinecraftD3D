#pragma once

#include <d3d11.h>
#include <d3dx10math.h>

enum ShaderType
{
	VertexShader,
	PixelShader,
	HullShader,
	DomainShader
};

struct CameraBuffer
{
	D3DXVECTOR3 cameraPosition;
	D3DXVECTOR2 normalMapTiling;
	D3DXVECTOR3 padding;
};

struct ClipPlaneBuffer
{
	D3DXVECTOR4 clipPlane;
};

struct DistortionBuffer
{
	D3DXVECTOR2 distortion1;
	D3DXVECTOR2 distortion2;
	D3DXVECTOR2 distortion3;
	float distortionScale;
	float distortionBias;
};

struct GradientBuffer
{
	D3DXVECTOR4 topColor;
	D3DXVECTOR4 centerColor;
};

struct LightBuffer
{
	D3DXVECTOR4 ambientColor;
	D3DXVECTOR4 diffuseColor;
	D3DXVECTOR3 lightDirection;
	float specularPower;
	D3DXVECTOR4 specularColor;
};

struct LightPositionBuffer
{
	D3DXVECTOR4 lightPosition;
};

struct MatrixBuffer
{
	D3DXMATRIX world;
	D3DXMATRIX view;
	D3DXMATRIX projection;
	D3DXMATRIX reflection;
};

struct NoiseBuffer
{
	float frameTime;
	D3DXVECTOR3 scrollSpeeds;
	D3DXVECTOR3 scales;
	float padding;
};

struct OceanBuffer
{
	D3DXVECTOR4 refractionTint;
	D3DXVECTOR3 lightDirection;
	float waterTranslation;
	float reflectRefractScale;
	float specularShininess;
	D3DXVECTOR2 padding;
};

struct PixelBuffer
{
	D3DXVECTOR4 pixelColor;
};

struct SkyBuffer
{
	float translation;
	float scale;
	float brightness;
	float padding;
};

struct TessellationBuffer
{
	float tessellationAmount;
	D3DXVECTOR3 padding;
};

struct WaveBuffer
{
	float waveTime;
	float waveHeight;
	D3DXVECTOR2 padding;
};

struct WaterEffectBuffer
{
	float refractionScale;
	float effectTime;
	D3DXVECTOR2 padding;
};


