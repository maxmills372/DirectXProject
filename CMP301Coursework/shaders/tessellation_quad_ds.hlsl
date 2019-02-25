// Tessellation domain shader
// After tessellation the domain shader processes the all the vertices
Texture2D shaderTexture : register(t0);
SamplerState SampleType : register(s0);

cbuffer MatrixBuffer : register(cb0)
{
    matrix worldMatrix;
    matrix viewMatrix;
    matrix projectionMatrix;
};

cbuffer HeightBuffer : register(cb1)
{
	float heightMapSize;
	float3 padding;
}

struct ConstantOutputType
{
    float edges[4] : SV_TessFactor;
    float insides[2] : SV_InsideTessFactor;
};

struct InputType
{
    float3 position : POSITION;
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
};

struct OutputType
{
    float4 position : SV_POSITION;
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
};

[domain("quad")]

OutputType main(ConstantOutputType input, float2 uvwCoord : SV_DomainLocation, const OutputPatch<InputType, 4> patch)
{
    float3 vertexPosition, vertexNormal;
	float2 texCoords;
	float4 textureColour;
    OutputType output;
 
    // Determine the position of the new vertex.
	// Invert the y and Z components of uvwCoord as these coords are generated in UV space and therefore y is positive downward.
	// Alternatively you can set the output topology of the hull shader to cw instead of ccw (or vice versa).
	//vertexPosition = uvwCoord.x * patch[0].position + uvwCoord.y * patch[1].position + uvwCoord.z * patch[2].position;
   
	float3 v1 = lerp(patch[0].position, patch[1].position, 1 - uvwCoord.y);
	float3 v2 = lerp(patch[2].position, patch[3].position, 1 - uvwCoord.y);
	vertexPosition = lerp(v1, v2, uvwCoord.x);


	float3 n1 = lerp(patch[0].normal, patch[1].normal, 1 - uvwCoord.y);
	float3 n2 = lerp(patch[2].normal, patch[3].normal, 1 - uvwCoord.y);
	vertexNormal = lerp(n1, n2, uvwCoord.x);


	float2 t1 = lerp(patch[0].tex, patch[1].tex, 1 - uvwCoord.y);
	float2 t2 = lerp(patch[2].tex, patch[3].tex, 1 - uvwCoord.y);
	texCoords = lerp(t1, t2, uvwCoord.x);

	textureColour = shaderTexture.SampleLevel(SampleType, texCoords, 0);
	vertexPosition.y += textureColour.x * heightMapSize;	//pass in number

	

    // Calculate the position of the new vertex against the world, view, and projection matrices.
    output.position = mul(float4(vertexPosition, 1.0f), worldMatrix);
    output.position = mul(output.position, viewMatrix);
    output.position = mul(output.position, projectionMatrix);


    // Send the input color into the pixel shader.
    output.tex = texCoords;

	// Calculate the normal vector against the world matrix only.
	output.normal = mul(vertexNormal, (float3x3)worldMatrix);

	// Normalize the normal vector.
	output.normal = normalize(output.normal);

    return output;
}

