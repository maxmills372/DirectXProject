// Light vertex shader
// Standard issue vertex shader, apply matrices, pass info to pixel shader

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

cbuffer TimeBuffer : register(cb2)
{
	float time;
	//float3 padding2;
	float height;
	float length;
	float padding2;
};


struct InputType
{
    float4 position : POSITION;
    float2 tex : TEXCOORD0;
    float3 normal : NORMAL;
};

struct OutputType
{
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD0;
    float3 normal : NORMAL;
	float3 position3D : TEXCOORD1;
	float3 viewDirection : TEXCOORD2;
};

OutputType main(InputType input)
{
	OutputType output;
	float3 worldPosition;

	input.position.w = 1.0f;
	//worldPosition.w = 1.0f;

	// Calculate the position of the vertex in the world
	worldPosition = mul(input.position, worldMatrix);

	// Calculate the position of the vertex against the world, view, and projection matrices.
	output.position = mul(input.position, worldMatrix);
	output.position = mul(output.position, viewMatrix);
	output.position = mul(output.position, projectionMatrix);

	// Determine the viewing direction based on the position of the camera and the
	//position of the vertex in the world.
	output.viewDirection = cameraPosition.xyz - worldPosition.xyz;
	//output.viewDirection = cameraPosition.y - worldPosition.y;
	//output.viewDirection = cameraPosition.z - worldPosition.z;

	// Normalize the viewing direction vector.
	output.viewDirection = normalize(output.viewDirection);

	// Store the texture coordinates for the pixel shader.
	output.tex = input.tex;

	// Calculate the normal vector against the world matrix only.
	output.normal = mul(input.normal, (float3x3)worldMatrix);

	// Normalize the normal vector.
	output.normal = normalize(output.normal);


    return output;
}