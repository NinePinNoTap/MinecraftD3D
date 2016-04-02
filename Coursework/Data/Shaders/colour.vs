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
};

struct PixelInputType
{
    float4 position : SV_POSITION;
};

// Vertex Shader
PixelInputType VertexShaderMain(VertexInputType input)
{
    PixelInputType output;

	// Add homogeneous coordinates
    input.position.w = 1.0f;

	// Calculate the position of the vertex against the matrices
    output.position = mul(input.position, worldMatrix);
    output.position = mul(output.position, viewMatrix);
    output.position = mul(output.position, projectionMatrix);
    
    return output;
}