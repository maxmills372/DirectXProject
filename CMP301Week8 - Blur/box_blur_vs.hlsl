// box blur vertex shader
// Standard issue vertex shader, apply matrices, pass info to pixel shader

cbuffer MatrixBuffer : register(cb0)
{
	matrix worldMatrix;
	matrix viewMatrix;
	matrix projectionMatrix;
};
cbuffer ScreenSizeBuffer : register(cb1)
{
	float screenWidth;
	float screenHeight;
	float2 padding;
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
	float2 texCoord1 : TEXCOORD1;
	float2 texCoord2 : TEXCOORD2;
	float2 texCoord3 : TEXCOORD3;
	float2 texCoord4 : TEXCOORD4;
	float2 texCoord5 : TEXCOORD5;
	float2 texCoord6 : TEXCOORD6;
	float2 texCoord7 : TEXCOORD7;
	float2 texCoord8 : TEXCOORD8;
	float2 texCoord9 : TEXCOORD9;
	float2 texCoord10 : TEXCOORD10;
	float2 texCoord11 : TEXCOORD11;
	float2 texCoord12 : TEXCOORD12;
	float2 texCoord13 : TEXCOORD13;
	float2 texCoord14 : TEXCOORD14;
	float2 texCoord15 : TEXCOORD15;
	float2 texCoord16 : TEXCOORD16;
	float2 texCoord17 : TEXCOORD17;
	float2 texCoord18 : TEXCOORD18;
	float2 texCoord19 : TEXCOORD19;
	float2 texCoord20 : TEXCOORD20;
	float2 texCoord21 : TEXCOORD21;
	float2 texCoord22 : TEXCOORD22;
	float2 texCoord23 : TEXCOORD23;
	float2 texCoord24 : TEXCOORD24;
};

OutputType main(InputType input)
{
	OutputType output;
	float texelWidth, texelHeight;

	input.position.w = 1.0f;
	output.position = mul(input.position, worldMatrix);
	output.position = mul(output.position, viewMatrix);
	output.position = mul(output.position, projectionMatrix);

	output.tex = input.tex;

	// Determine the floating point size of a texel for a screen with this specific width.
	texelWidth = 1.0f / screenWidth;
	texelHeight = 1.0f / screenHeight;


	// Create UV coordinates for the pixel and its four horizontal neighbors on either side.
	output.texCoord1 = input.tex + float2(-texelWidth, -texelHeight);
	output.texCoord2 = input.tex + float2(0.0f, -texelHeight);
	output.texCoord3 = input.tex + float2(texelWidth, -texelHeight);
	output.texCoord4 = input.tex + float2(-texelWidth, 0.0f);
	output.texCoord5 = input.tex + float2(texelWidth, 0.0f);
	output.texCoord6 = input.tex + float2(-texelWidth, texelHeight);
	output.texCoord7 = input.tex + float2(0.0f, texelHeight);
	output.texCoord8 = input.tex + float2(texelWidth, texelHeight);
	
	output.texCoord9 = input.tex + float2(-texelWidth * 2, -texelHeight * 2);
	output.texCoord10 = input.tex + float2(-texelWidth, -texelHeight * 2);
	output.texCoord11 = input.tex + float2(0.0f, -texelHeight * 2);
	output.texCoord12 = input.tex + float2(texelWidth, -texelHeight * 2);
	output.texCoord13 = input.tex + float2(texelWidth * 2, -texelHeight * 2);
	output.texCoord14 = input.tex + float2(-texelWidth * 2, -texelHeight);
	output.texCoord15 = input.tex + float2(texelWidth * 2, -texelHeight);
	output.texCoord16 = input.tex + float2(-texelWidth * 2, 0.0f);
	output.texCoord17 = input.tex + float2(texelWidth * 2, 0.0f);
	output.texCoord18 = input.tex + float2(-texelWidth * 2, texelHeight);
	output.texCoord19 = input.tex + float2(texelWidth * 2, texelHeight);
	output.texCoord20 = input.tex + float2(-texelWidth * 2, texelHeight * 2);
	output.texCoord21 = input.tex + float2(-texelWidth, texelHeight * 2);
	output.texCoord22 = input.tex + float2(0.0f, texelHeight * 2);
	output.texCoord23 = input.tex + float2(texelWidth, texelHeight * 2);
	output.texCoord24 = input.tex + float2(texelWidth * 2, texelHeight * 2);


	return output;

}