// Globals
cbuffer MatrixBuffer
{
    matrix worldMatrix;
    matrix viewMatrix;
    matrix projectionMatrix;
	matrix reflectionMatrix;
};

// Typedefs
struct VertexInputType
{
    float4 position : POSITION;
    float2 tex : TEXCOORD0;
	float3 instancePosition : TEXCOORD1;
	float2 instanceTexture : TEXCOORD2;
	float2 totalTextures : TEXCOORD3;
};

struct PixelInputType
{
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD0;
};

// Vertex Shader
PixelInputType VertexShaderMain(VertexInputType input)
{
    PixelInputType output;
    float4 worldPosition;

    // Add homogeneous coordinate
    input.position.w = 1.0f;
	
	// Apply instance position offset
	input.position.x += input.instancePosition.x;
	input.position.y += input.instancePosition.y;
	input.position.z += input.instancePosition.z;

    // Calculate the position of the vertex against the matrices
    output.position = mul(input.position, worldMatrix);
    output.position = mul(output.position, viewMatrix);
    output.position = mul(output.position, projectionMatrix);
    
    // Pass the texture coord to the pixel shader
    output.tex.x = (input.tex.x / input.totalTextures.x) + (input.instanceTexture.x / input.totalTextures.x);
	output.tex.y = (input.tex.y / input.totalTextures.y) + (input.instanceTexture.y / input.totalTextures.y);

    return output;
}