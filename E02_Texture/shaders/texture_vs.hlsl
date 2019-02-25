// texture vertex shader
// Basic shader for rendering textured geometry

cbuffer MatrixBuffer : register(cb0)
{
    matrix worldMatrix;
    matrix viewMatrix;
    matrix projectionMatrix;
};

struct InputType
{
    float4 position : POSITION;
    float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
};

struct OutputType
{
    float4 position : SV_POSITION; // SHOLD BE POSITION FOR BILLBOARD STUFF
    float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
};

OutputType main(InputType input, unsigned int id : SV_VertexID)
{
	OutputType output;
    

    // Change the position vector to be 4 units for proper matrix calculations.
	input.position.w = 1.0f;
	
	// Shift texture position by 0.5
	//input.tex.x -= 0.5f;
	//input.tex.y -= 0.5f;

    // Calculate the position of the vertex against the world, view, and projection matrices.
    output.position = mul(input.position, worldMatrix);
    output.position = mul(output.position, viewMatrix);
    output.position = mul(output.position, projectionMatrix);
	//output.position = input.position;// float4(input.position.xyz, 1.f);

	// Store the texture coordinates for the pixel shader.
    output.tex = input.tex;

	// Calculate the normal vector against the world matrix only.
    output.normal = mul(input.normal, (float3x3)worldMatrix);
	
    // Normalize the normal vector.
    output.normal = normalize(output.normal);
    
    return output;
}