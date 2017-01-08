// Globals
cbuffer TessellationBuffer
{
    float tessellationAmount;
    float3 padding;
};

// Typedefs
struct HullInputType
{
    float3 position : POSITION;
    float2 tex : TEXCOORD0;
};

struct ConstantOutputType
{
    float edges[4] : SV_TessFactor;
    float inside[2] : SV_InsideTessFactor;
};

struct HullOutputType
{
    float3 position : POSITION;
    float2 tex : TEXCOORD0;
};

// Patch Constant Function
ConstantOutputType OceanPatchConstantFunction(InputPatch<HullInputType, 4> inputPatch, uint patchId : SV_PrimitiveID)
{    
    ConstantOutputType output;

    // set the tessellation factors for the three edges of the triangle.
    output.edges[0] = tessellationAmount;
    output.edges[1] = tessellationAmount;
    output.edges[2] = tessellationAmount;
    output.edges[3] = tessellationAmount;

    // set the tessellation factor for tessallating inside the triangle.
    output.inside[0] = tessellationAmount;
    output.inside[1] = tessellationAmount;

    return output;
}

[domain("quad")]
[partitioning("integer")]
[outputtopology("triangle_cw")]
[outputcontrolpoints(4)]
[patchconstantfunc("OceanPatchConstantFunction")]

// Hull Shader
HullOutputType HullShaderMain(InputPatch<HullInputType, 4> patch, uint pointId : SV_OutputControlPointID, uint patchId : SV_PrimitiveID)
{
    HullOutputType output;

    // set the position and texture for this control point
    output.position = patch[pointId].position;
    output.tex = patch[pointId].tex;

	// Send to the domain shader
    return output;
}