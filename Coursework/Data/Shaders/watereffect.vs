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
};

struct PixelInputType
{
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD0;
    float4 refractionPosition : TEXCOORD1;
};

// Vertex Shader
PixelInputType VertexShaderMain(VertexInputType input)
{
    PixelInputType output;
	matrix viewProjectWorld;

	// Add the homogeneous coordinate
    input.position.w = 1.0f;

	// Calculate the position of the vertex against the matrices
    output.position = mul(input.position, worldMatrix);
    output.position = mul(output.position, viewMatrix);
    output.position = mul(output.position, projectionMatrix);
    
	// Pass the tex coords to the pixel shader
	output.tex = input.tex;
    
	// Create the refraction view projection matrix
    viewProjectWorld = mul(viewMatrix, projectionMatrix);
    viewProjectWorld = mul(worldMatrix, viewProjectWorld);
   
	// Calculate the refraction position using the above view matrix
    output.refractionPosition = mul(input.position, viewProjectWorld);

    return output;
}