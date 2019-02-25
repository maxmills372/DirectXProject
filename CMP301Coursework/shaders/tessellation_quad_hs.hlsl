// Tessellation Hull Shader
// Prepares control points for tessellation
Texture2D shaderTexture : register(t0);
SamplerState SampleType : register(s0);

cbuffer TessBuffer : register(cb0)
{
	float4 edges;
	float2 insides;
	float2 padding;
};

cbuffer CameraBuffer : register(cb1)
{
	float3 cameraPosition;
	float padding2;
};

struct InputType
{
    float3 position : POSITION;
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
};

struct ConstantOutputType
{
    float edges[4] : SV_TessFactor;
    float insides[2] : SV_InsideTessFactor;
};

struct OutputType
{
    float3 position : POSITION;
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
};

float calcDistance(float3 vector_1, float3 vector_2, float2 texCoords)
{
	//float4 textureColour = shaderTexture.SampleLevel(SampleType, input.tex, 0);
	vector_2.y = shaderTexture.SampleLevel(SampleType, texCoords, 0) * 5;
	

	//Distance formula
	float dist = sqrt(pow((vector_1.x - vector_2.x), 2) + pow((vector_1.y - vector_2.y), 2) + pow((vector_1.z - vector_2.z), 2));

	dist = 64 - dist * 10;

	if (dist > 64 )
	{
		dist = 64;
	}
	if (dist < 2)
	{
		dist = 2;
	}
	return dist;
}

ConstantOutputType PatchConstantFunction(InputPatch<InputType, 4> inputPatch, uint patchId : SV_PrimitiveID)
{    
    ConstantOutputType output;
	
	float heightMapSize = 5.0f; //PASS INNNNNNNNNNNN


	float3 middlePoint;
	float3 edge0, edge1, edge2, edge3;

	edge0 = (inputPatch[0].position + inputPatch[1].position) / 2.0f;
	edge1 = (inputPatch[1].position + inputPatch[3].position) / 2.0f;
	edge2 = (inputPatch[3].position + inputPatch[2].position) / 2.0f;
	edge3 = (inputPatch[2].position + inputPatch[0].position) / 2.0f;

	// Find the middle point of the inputPatch
	middlePoint = edge0 + edge2;
	middlePoint /= 2.0f;

	// Set the tessellation factors for the three edges of the triangle.
	output.edges[0] = 1.0f;
	output.edges[1] = 1.0f;
	output.edges[2] = 1.0f;
	output.edges[3] = 1.0f;

	// Set the tessellation factor for tessallating inside the triangle.
	output.insides[0] = 1.0f;
	output.insides[1] = 1.0f;


	float2 texCoord1, texCoord2, texCoord3, texCoord4;
	float2 middleTex;

	texCoord1 = (inputPatch[0].tex + inputPatch[1].tex) / 2.0f;
	texCoord2 = (inputPatch[1].tex + inputPatch[3].tex) / 2.0f;
	texCoord3 = (inputPatch[3].tex + inputPatch[2].tex) / 2.0f;
	texCoord4 = (inputPatch[2].tex + inputPatch[0].tex) / 2.0f;

	middleTex = texCoord1 + texCoord3;
	middleTex /= 2.0f;

/*
	float2 t1 = lerp(inputPatch[0].tex, inputPatch[1].tex, 1 - texCoords.y);
	float2 t2 = lerp(inputPatch[2].tex, inputPatch[3].tex, 1 - texCoords.y);
	texCoords = lerp(t1, t2, texCoords.x);
	*/

	output.edges[0] = calcDistance(cameraPosition, edge0, texCoord1);
	output.edges[1] = calcDistance(cameraPosition, edge1, texCoord2);
	output.edges[2] = calcDistance(cameraPosition, edge2, texCoord3);
	output.edges[3] = calcDistance(cameraPosition, edge3, texCoord4);

	output.insides[0] = calcDistance(cameraPosition, middlePoint, middleTex);
	output.insides[1] = calcDistance(cameraPosition, middlePoint, middleTex);

	//float num = 200;

	/*output.edges[0] = 1 / calcDistance(cameraPosition, edge0) * num;
	output.edges[1] = 1 / calcDistance(cameraPosition, edge1)* num;
	output.edges[2] = 1 / calcDistance(cameraPosition, edge2)* num;
	output.edges[3] = 1 / calcDistance(cameraPosition, edge3)* num;

	output.insides[0] = 1 / calcDistance(cameraPosition, middlePoint)* num;
	output.insides[1] = 1 / calcDistance(cameraPosition, middlePoint)* num;*/


    return output;
}


[domain("quad")]
[partitioning("fractional_even")]
//integer, fractional_odd, fractional_evem
[outputtopology("triangle_cw")]
[outputcontrolpoints(4)]
[patchconstantfunc("PatchConstantFunction")]
OutputType main(InputPatch<InputType, 4> patch, uint pointId : SV_OutputControlPointID, uint patchId : SV_PrimitiveID)
{
    OutputType output;

    // Set the position for this control point as the output position.
    output.position = patch[pointId].position;

	// Set the input color as the output color.
	output.normal = patch[pointId].normal;


    // Set the input tex as the output tex.
    output.tex = patch[pointId].tex;

    return output;
}