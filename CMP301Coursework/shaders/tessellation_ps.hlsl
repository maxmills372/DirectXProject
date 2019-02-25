// Tessellation pixel shader
// Output colour passed to stage.
Texture2D shaderTexture : register(t0);
SamplerState SampleType : register(s0);

struct InputType
{
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
};

float4 main(InputType input) : SV_TARGET
{
	float4 textureColour;

	textureColour = shaderTexture.SampleLevel(SampleType, input.tex,0);

	return textureColour;
}