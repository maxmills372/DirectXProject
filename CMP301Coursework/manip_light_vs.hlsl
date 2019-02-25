// Light vertex shader
// Standard issue vertex shader, apply matrices, pass info to pixel shader
Texture2D shaderTexture : register(t0);
SamplerState SampleType : register(s0);

cbuffer MatrixBuffer : register(cb0)
{
	matrix worldMatrix;
	matrix viewMatrix;
	matrix projectionMatrix;
};

cbuffer CameraBuffer : register(cb1)
{
	float3 cameraPosition;
	float heightMapSize;
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
	float4 worldPosition;
	float4 textureColour;
	//float height = 1.0f;

	input.position.w = 1.0f;
	//worldPosition.w = 1.0f;

	//SCALING
	input.position.x *= 10.0f;
	input.position.y *= 10.0f;
	input.position.z *= 10.0f;

	

	// Vertex manipulation
	// offset position based on sine wave
	input.position.x += height * sin((1 / length)*(input.position.x + time));
	input.position.y += height * sin((1 / length)*(input.position.x + time));
	input.position.z += height * sin((1/length)*(input.position.x + time));

	// Ripples using normals - dont know if its correct
	//input.position.x += (height * sin((1 / length)*((input.position.x * input.position.y * input.position.z) + time))) * input.normal.x;
	//input.position.y += (height * sin((1 / length)*((input.position.x * input.position.y * input.position.z) + time))) * input.normal.y;
	//input.position.z += (height * sin((1 / length)*((input.position.x * input.position.y * input.position.z) + time))) * input.normal.z;

	textureColour = shaderTexture.SampleLevel(SampleType, input.tex, 0);
	input.position.x += textureColour.xyz * heightMapSize * input.normal.x;
	input.position.y += textureColour.xyz * heightMapSize * input.normal.y;
	input.position.z += textureColour.xyz * heightMapSize * input.normal.z;

	//modify normals
	//input.normal.x = 1 - cos(input.position.x + time);
	//input.normal.y = abs(cos(input.position.x + time));


	// Calculate the position of the vertex in the world
	worldPosition = mul(input.position, worldMatrix);

	// Calculate the position of the vertex against the world, view, and projection matrices.
	output.position = mul(input.position, worldMatrix);
	output.position = mul(output.position, viewMatrix);
	output.position = mul(output.position, projectionMatrix);

	// Determine the viewing direction based on the position of the camera and the
	//position of the vertex in the world.
	output.viewDirection = cameraPosition.xyz - worldPosition.xyz;

	// Normalize the viewing direction vector.
	output.viewDirection = normalize(output.viewDirection);


	// Store the texture coordinates for the pixel shader.
	output.tex = input.tex;

	// Calculate the normal vector against the world matrix only.
	output.normal = mul(input.normal, (float3x3)worldMatrix);

	// Normalize the normal vector.
	output.normal = normalize(output.normal);


	// world position of vertex
	output.position3D = mul(input.position, worldMatrix);


	return output;
}