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

struct CameraCBuffer
{
	D3DXVECTOR3 cameraPosition;
	D3DXVECTOR2 normalMapTiling;
	D3DXVECTOR3 padding;
};

struct ClipPlaneCBuffer
{
	D3DXVECTOR4 clipPlane;
};

struct DistortionCBuffer
{
	D3DXVECTOR2 distortion1;
	D3DXVECTOR2 distortion2;
	D3DXVECTOR2 distortion3;
	float distortionScale;
	float distortionBias;
};

struct GradientCBuffer
{
	D3DXVECTOR4 topColor;
	D3DXVECTOR4 centerColor;
};

struct LightCBuffer
{
	D3DXVECTOR4 ambientColor;
	D3DXVECTOR4 diffuseColor;
	D3DXVECTOR3 lightDirection;
	float specularPower;
	D3DXVECTOR4 specularColor;
};

struct LightPositionCBuffer
{
	D3DXVECTOR4 lightPosition;
};

struct MatrixCBuffer
{
	D3DXMATRIX world;
	D3DXMATRIX view;
	D3DXMATRIX projection;
	D3DXMATRIX reflection;
};

struct NoiseCBuffer
{
	float frameTime;
	D3DXVECTOR3 scrollSpeeds;
	D3DXVECTOR3 scales;
	float padding;
};

struct OceanCBuffer
{
	D3DXVECTOR4 refractionTint;
	D3DXVECTOR3 lightDirection;
	float waterTranslation;
	float reflectRefractScale;
	float specularShininess;
	D3DXVECTOR2 padding;
};

struct PixelCBuffer
{
	D3DXVECTOR4 pixelColor;
};

struct SkyCBuffer
{
	float translation;
	float scale;
	float brightness;
	float padding;
};

struct TessellationCBuffer
{
	float tessellationAmount;
	D3DXVECTOR3 padding;
};

struct WaveCBuffer
{
	float waveTime;
	float waveHeight;
	D3DXVECTOR2 padding;
};

struct WaterEffectCBuffer
{
	float refractionScale;
	float effectTime;
	D3DXVECTOR2 padding;
};


