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

float3 explode(float3 position, float3 normal)
{
	float magnitude = 2.0;
	float3 direction = normal *  ((sin(time) + 1.0) / 2.0) *  magnitude;
	return position + direction;
}
// gs function
[maxvertexcount(12)]//NUMBER OF VERTICES (for the one shape being created)
void main(triangle InputType input[3], inout TriangleStream<OutputType> triStream)
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


	//float planeEqn = 3x + 5y + 4z - 4;
	//if (centerPos > planeEqn) 
	//{
    
	
	// Calculate the face normal	
	float3 faceEdgeA = input[0].position - input[1].position;
	float3 faceEdgeB = input[2].position - input[1].position;
	float3 faceNormal = normalize(cross(faceEdgeA, faceEdgeB));

	// Displacement vector moves along face normal
	float displacementFactor = (sin(time) + 1.f);
    float3 displacement = faceNormal * displacementFactor;
   
    // Calculate the face center    
    float3 centerPos = (input[0].position.xyz + input[1].position.xyz + input[2].position.xyz) / 3.0f;
    float2 centerTex = (input[0].tex + input[1].tex + input[2].tex) / 3.0f;
    centerPos = faceNormal * 1.2f;

	// Generate a pyramid from a new point out from the face centre
	// Extrude by displacemnet vector
    for (int i = 0; i < 3; i++)
    {
        output.position = input[i].position + float4(displacement, 0);
        output.position = mul(output.position, viewMatrix);
        output.position = mul(output.position, projectionMatrix);
        output.normal = input[i].normal;
        output.tex = input[i].tex;

        triStream.Append(output);
        
        int j = (i + 1) % 3;
        output.position = input[j].position + float4(displacement, 0);
        output.position = mul(output.position, viewMatrix);
        output.position = mul(output.position, projectionMatrix);
        output.normal = input[j].normal;
        output.tex = input[j].tex;

        triStream.Append(output);
        
        output.position = float4(centerPos, 1) + float4(displacement, 0);
        output.position = mul(output.position, viewMatrix);
        output.position = mul(output.position, projectionMatrix);
        output.normal = faceNormal;
        output.tex = centerTex;

        triStream.Append(output);
        
        triStream.RestartStrip();
    }

    // Create face for base of pyramid
	for (int k = 2; k >= 0; k--)
    {
        output.position = input[k].position + float4(displacement, 0);
        output.position = mul(output.position, viewMatrix);
        output.position = mul(output.position, projectionMatrix);
        output.normal = input[k].normal;
        output.tex = input[k].tex;
        triStream.Append(output);
    }
       
    //}
	//
	// Rendering quads
	//
    /*
	for (int i = 0; i < 4; i++)
	{
        float4 vposition = float4(g_positions[i], 1.0f) + float4(displacement, 0.f);
    
		output.position = mul(vposition, worldMatrix) ;
		output.position = mul(output.position, viewMatrix);
		output.position = mul(output.position, projectionMatrix);

		output.tex = texCoord[i];
		output.normal = input[0].normal; 

		triStream.Append(output);
	}*/
    triStream.RestartStrip();
	
	/*output.position = explode(input[0].position , faceNormal);

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
    */
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
	
	//triStream.RestartStrip();	
}
