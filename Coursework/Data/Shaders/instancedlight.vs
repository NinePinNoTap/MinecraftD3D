// Globals
cbuffer MatrixBuffer
{
    matrix worldMatrix;
    matrix viewMatrix;
    matrix projectionMatrix;
	matrix reflectionMatrix;
};

cbuffer CameraBuffer
{
    float3 cameraPosition;
	float2 normalMapTiling;
    float3 padding;
};

cbuffer LightBuffer
{
	float4 lightPosition;
};

// Typedefs
struct VertexInputType
{
    float4 position : POSITION;
    float2 tex : TEXCOORD0;
    float3 normal : NORMAL;
	float3 instancePosition : TEXCOORD1;
	float2 instanceTexture : TEXCOORD2;
	float2 totalTextures : TEXCOORD3;
};

struct PixelInputType
{
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD0;
    float3 normal : NORMAL;
    float3 viewDirection : TEXCOORD1;
    float3 lightPos : TEXCOORD2;
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
    
    // Calculate the normal vector and normalise
    output.normal = mul(input.normal, (float3x3)worldMatrix);
    output.normal = normalize(output.normal);

    // Calculate the position of the vertex in the world
    worldPosition = mul(input.position, worldMatrix);

    // Calculate the viewing direction and normalise
    output.viewDirection = cameraPosition.xyz - worldPosition.xyz;
    output.viewDirection = normalize(output.viewDirection);

	// Calculate the light position and normalise
    output.lightPos.xyz = lightPosition.xyz - worldPosition.xyz;
    output.lightPos = normalize(output.lightPos);

    return output;
}