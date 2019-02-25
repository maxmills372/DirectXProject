// Texture pixel/fragment shader
// Basic fragment shader for rendering textured geometry


Texture2D texture0 : register(t0);
Texture2D texture1 : register(t1);
Texture2D shaderTexture : register(t2);

SamplerState Sampler0: register(s0);
SamplerState SampleType : register(s1);


cbuffer BlendTexBuffer : register(cb0)
{

	float divisionAmount;
	float3 padding;

};

struct InputType
{
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
	float3 position3D : TEXCOORD1;
	float3 viewDirection : TEXCOORD2;
};





float4 main(InputType input) : SV_TARGET
{

	float4 texturecolour;
	
	
	// Sample the pixel color from the texture using the sampler at this texture coordinate location.
	texturecolour = ((texture0.Sample(Sampler0, input.tex) * divisionAmount) + (texture1.Sample(Sampler0, input.tex) * (1 - divisionAmount)) / 2);
	//textureColor = (texture0.Sample(Sampler0, input.tex));
		

	return texturecolour;
}