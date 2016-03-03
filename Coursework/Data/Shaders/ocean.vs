// Typedefs
struct VertexInputType
{
    float3 position : POSITION;
    float2 tex : TEXCOORD0;
};

struct HullInputType
{
    float3 position : POSITION;
    float2 tex : TEXCOORD0;
};

// Vertex Shader
HullInputType VertexShaderMain(VertexInputType input)
{
    HullInputType output;
    
	// Pass the position and texture coord into the hull shader
    output.position = input.position;
    output.tex = input.tex;
    
    return output;
}