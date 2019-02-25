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
	float time;
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
	float3 rotation : TEXCOORD1;
};

// pixel input type
struct OutputType
{
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
};

float4 explode(float4 position, float3 normal)
{
	float magnitude = 2.0;
	float3 direction = normal *  ((sin(time) + 1.0) / 2.0) *  magnitude;
	return position + float4(direction, 0.0);
}
// gs function
[maxvertexcount(6)]//NUMBER OF VERTICES (for the one shape being created)
void main(triangleadj InputType input[6], inout TriangleStream<OutputType> triStream)
{
	OutputType output;	

	// Change the position vector to be 4 units for proper matrix calculations.
    input[0].position.w = 1.0f;
	//input[1].position.w = 1.0f;

	float3 g_positions[4] =
	{
		float3(-0.5, 0.5, 0) + input[0].position,
		float3(-0.5, -0.5, 0) + input[0].position,
		float3(0.5, 0.5, 0) + input[0].position,
		float3(0.5, -0.5, 0) + input[0].position
	};

	float2 texCoord[8];
	texCoord[0] = float2(0, 1);
	texCoord[1] = float2(1, 1);
	texCoord[2] = float2(0, 0);
	texCoord[3] = float2(1, 0);
	texCoord[4] = float2(1, 0);
	texCoord[5] = float2(1, 1);
	texCoord[6] = float2(0, 0);
	texCoord[7] = float2(0, 1);

	//
	// Billboarding 
	//
	/*
	
	float3 up, forward, right, global_up;
	global_up = float3(0.0f, 1.0f, 0.0f);

	forward = input[0].position - cameraPosition;
	forward = forward / length(forward);


	right = cross(forward, global_up);
	right = right / length(right);
	
	up = cross(forward, right);
	up = up / length(up);


	g_positions[0] = (up + right) + input[0].position.xyz;
	g_positions[1] = (up - right) +input[0].position.xyz;
	g_positions[2] = (-up + right) + input[0].position.xyz;
	g_positions[3] = (-up - right)  +input[0].position.xyz;
	*/

	//
	// Calculate the face normal
	//
    /*
	float3 faceEdgeA = input[0].position - input[1].position;
	float3 faceEdgeB = input[2].position - input[1].position;
	float3 faceNormal = normalize(cross(faceEdgeA, faceEdgeB));*/

    float3 faceEdgeA = input[0].position - input[4].position;
    float3 faceEdgeB = input[2].position - input[4].position;
    float3 faceNormal = normalize(cross(faceEdgeA, faceEdgeB));

	//Steps
	//1. pass in plane eqn
	//2. check which side triangle is on
	//3. move up/down from plane normal
	//

	//
	// Rendering quads
	//
	/*for (int i = 0; i < 4; i++)
	{
		float4 vposition = input[i].position;
        output.position = explode(vposition, faceNormal);

		output.position = mul(vposition, worldMatrix) ;
		output.position = mul(output.position, viewMatrix);
		output.position = mul(output.position, projectionMatrix);

		output.tex = texCoord[i];
		output.normal = input[0].normal; 

		triStream.Append(output);
	}*/

	
	output.position = explode(input[0].position , faceNormal);

	output.position = mul(output.position, worldMatrix);
	output.position = mul(output.position, viewMatrix);
	output.position = mul(output.position, projectionMatrix);

	output.tex = texCoord[0];

	output.normal = mul(input[0].normal, (float3x3)worldMatrix);
	output.normal = normalize(output.normal);

	triStream.Append(output);

	// Move the vertex away from the point position
	output.position = explode(input[1].position, faceNormal);

	output.position = mul(output.position, worldMatrix);
	output.position = mul(output.position, viewMatrix);
	output.position = mul(output.position, projectionMatrix);

	output.tex = texCoord[1];

	output.normal = mul(input[1].normal, (float3x3)worldMatrix);
	output.normal = normalize(output.normal);

	triStream.Append(output);

	// Move the vertex away from the point position
	output.position = explode(input[2].position, faceNormal);

	output.position = mul(output.position, worldMatrix);
	output.position = mul(output.position, viewMatrix);
	output.position = mul(output.position, projectionMatrix);

	output.tex = texCoord[2];//input[2].tex;

	output.normal = mul(input[2].normal, (float3x3)worldMatrix);
	output.normal = normalize(output.normal);

	triStream.Append(output);
    

	
	// Move the vertex away from the point position
	output.position = explode(input[3].position, faceNormal);

	output.position = mul(output.position, worldMatrix);
	output.position = mul(output.position, viewMatrix);
	output.position = mul(output.position, projectionMatrix);

	output.tex = texCoord[3];

	output.normal = mul(input[3].normal, (float3x3)worldMatrix);
	output.normal = normalize(output.normal);
	triStream.Append(output);

    // Move the vertex away from the point position
    output.position = explode(input[4].position, faceNormal);

    output.position = mul(output.position, worldMatrix);
    output.position = mul(output.position, viewMatrix);
    output.position = mul(output.position, projectionMatrix);

    output.tex = texCoord[4];

    output.normal = mul(input[4].normal, (float3x3) worldMatrix);
    output.normal = normalize(output.normal);
    triStream.Append(output);

    // Move the vertex away from the point position
    output.position = explode(input[5].position, faceNormal);

    output.position = mul(output.position, worldMatrix);
    output.position = mul(output.position, viewMatrix);
    output.position = mul(output.position, projectionMatrix);

    output.tex = texCoord[5]; 

    output.normal = mul(input[5].normal, (float3x3) worldMatrix);
    output.normal = normalize(output.normal);
    triStream.Append(output);
    
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
	*/
	
	triStream.RestartStrip();	
}
