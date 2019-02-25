// Example geometry shader
// Receives a point and outputs a triangle

cbuffer MatrixBuffer : register(cb0)
{
    matrix worldMatrix;
    matrix viewMatrix;
    matrix projectionMatrix;
};
cbuffer CameraBuffer : register(cb1)
{
	float3 cameraPosition;
	float padding;
};
cbuffer PositionBuffer
{
	//array of positions (VERTICES?)
	/*static float3 g_positions[4] =
	{
		float3(-1, 1, 0),
		float3(-1, -1, 0),
		float3(1, 1, 0),
		float3(1, -1, 0) 
		
	};*/
	static float3 g_positions_2[8] =
	{
		float3(-1, 1, 0),
		float3(-1, -1, 0),
		float3(1, 1, 0),
		float3(1, -1, 0),
		float3(-1, 5, 0),
		float3(-1, 3, 0),
		float3(1, 5, 0),
		float3(1, 3, 0)
	};
	
};

struct InputType
{
	float4 position : POSITION;
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
};

// pixel input type
struct OutputType
{
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
};

// gs function
[maxvertexcount(4)]//NUMBER OF VERTICES (for the one shape being created)
void main(point InputType input[1], inout TriangleStream<OutputType> triStream)
{
	OutputType output;

	

	// Change the position vector to be 4 units for proper matrix calculations.
    input[0].position.w = 1.0f;
	//input[1].position.w = 1.0f;

	float3 g_positions[4] =
	{
		float3(-1, 1, 0) + input[0].position,
		float3(-1, -1, 0) + input[0].position,
		float3(1, 1, 0) + input[0].position,
		float3(1, -1, 0) + input[0].position
	};

	float2 texCoord[4];
	texCoord[0] = float2(0, 1);
	texCoord[1] = float2(1, 1);
	texCoord[2] = float2(0, 0);
	texCoord[3] = float2(1, 0);

	

	float3 up, forward, right, global_up;
	global_up = float3(0.0f, 1.0f, 0.0f);

	forward = input[0].position - cameraPosition;
	forward = forward / length(forward);
	forward.y = 0.0f;

	right = cross(forward, global_up);
	right = right / length(right);
	
	up = cross(forward, right);
	up = up / length(up);


	g_positions[0] = (up + right) + input[0].position.xyz;
	g_positions[1] = (up - right) + input[0].position.xyz;
	g_positions[2] = (-up + right) + input[0].position.xyz;
	g_positions[3] = (-up - right)  + input[0].position.xyz;

	// For quads
	for (int i = 0; i < 4; i++)
	{
		float3 vposition = g_positions[i] ;

		output.position = mul(float4(vposition, 1.0),  worldMatrix);
		output.position = mul(output.position, viewMatrix);
		output.position = mul(output.position, projectionMatrix);

		output.tex = texCoord[i];
		output.normal = input[0].normal; 

		triStream.Append(output);
	}

	

	//input[0].rotation = rotationAroundY;
	//output.position = mul(input[0].position, rotationAroundY);
	

	// For triangles
	/*
	// Move the vertex away from the point position
	output.position = input[0].position + float4(0.0, 3.0, 0.0, 1.0);
	
	output.position = mul(output.position, worldMatrix);
    output.position = mul(output.position, viewMatrix);
    output.position = mul(output.position, projectionMatrix);

	output.tex = texCoord[0];

    output.normal = mul(input[0].normal, (float3x3)worldMatrix);
    output.normal = normalize(output.normal);

	triStream.Append(output);

	// Move the vertex away from the point position
	output.position = input[1].position + float4(-3.0, 0.0, 0.0, 1.0);
	
	output.position = mul(output.position, worldMatrix);
    output.position = mul(output.position, viewMatrix);
    output.position = mul(output.position, projectionMatrix);

	output.tex = texCoord[1];

    output.normal = mul(input[1].normal, (float3x3)worldMatrix);
    output.normal = normalize(output.normal);

	triStream.Append(output);

	// Move the vertex away from the point position
	output.position = input[2].position + float4(3.0, 0.0, 0.0, 1.0);
	
	output.position = mul(output.position, worldMatrix);
    output.position = mul(output.position, viewMatrix);
    output.position = mul(output.position, projectionMatrix);

	output.tex = texCoord[2];//input[2].tex;

    output.normal = mul(input[2].normal, (float3x3)worldMatrix);
    output.normal = normalize(output.normal);

	triStream.Append(output);


	triStream.RestartStrip();
	*/
	
}