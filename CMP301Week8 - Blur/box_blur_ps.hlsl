// Light pixel shader
// Calculate diffuse lighting for a single directional light (also texturing)

Texture2D shaderTexture : register(t0);
SamplerState SampleType : register(s0);

struct InputType
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

float4 main(InputType input) : SV_TARGET
{
	float4 colour;

	// Initialize the colour to black.
	colour = float4(0.0f, 0.0f, 0.0f, 0.0f);

	// Add the nine surrounding pixels to the colour.
	colour += shaderTexture.Sample(SampleType, input.tex);

	colour += shaderTexture.Sample(SampleType, input.texCoord1);
	colour += shaderTexture.Sample(SampleType, input.texCoord2);
	colour += shaderTexture.Sample(SampleType, input.texCoord3);
	colour += shaderTexture.Sample(SampleType, input.texCoord4);
	colour += shaderTexture.Sample(SampleType, input.texCoord5);
	colour += shaderTexture.Sample(SampleType, input.texCoord6);
	colour += shaderTexture.Sample(SampleType, input.texCoord7);
	colour += shaderTexture.Sample(SampleType, input.texCoord8);

	//additional pixels, total = 25
	colour += shaderTexture.Sample(SampleType, input.texCoord9);
	colour += shaderTexture.Sample(SampleType, input.texCoord10);
	colour += shaderTexture.Sample(SampleType, input.texCoord11);
	colour += shaderTexture.Sample(SampleType, input.texCoord12);
	colour += shaderTexture.Sample(SampleType, input.texCoord13);
	colour += shaderTexture.Sample(SampleType, input.texCoord14);
	colour += shaderTexture.Sample(SampleType, input.texCoord15);
	colour += shaderTexture.Sample(SampleType, input.texCoord16);
	colour += shaderTexture.Sample(SampleType, input.texCoord17);
	colour += shaderTexture.Sample(SampleType, input.texCoord18);
	colour += shaderTexture.Sample(SampleType, input.texCoord19);
	colour += shaderTexture.Sample(SampleType, input.texCoord20);
	colour += shaderTexture.Sample(SampleType, input.texCoord21);
	colour += shaderTexture.Sample(SampleType, input.texCoord22);
	colour += shaderTexture.Sample(SampleType, input.texCoord23);
	colour += shaderTexture.Sample(SampleType, input.texCoord24);

	//divide by total 
	colour /= 25.0f;

	// Set the alpha channel to one.
	colour.a = 1.0f;


	return colour;

}