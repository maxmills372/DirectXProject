// Light pixel shader
// Calculate diffuse lighting for a single directional light (also texturing)

Texture2D shaderTexture : register(t0);
SamplerState SampleType : register(s0);

cbuffer LightBuffer : register(cb0)
{
    float4 diffuseColor;
	float4 ambientColour;
    float3 position;
	float padding;
 
};

struct InputType
{
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD0;
    float3 normal : NORMAL;
	float3 position3D : TEXCOORD1;
};

float4 main(InputType input) : SV_TARGET
{
    float4 textureColour;
    float3 lightDir;
    float lightIntensity;
    float4 color;
	//float4 finalColour;

	// Sample the pixel color from the texture using the sampler at this texture
	//coordinate location.
	textureColour = shaderTexture.Sample(SampleType, input.tex);
	// Set the default output colour to the ambient light value for all pixels.
	color = ambientColour;
	// Invert the light direction for calculations.
	lightDir = normalize(input.position3D - position);
	// Calculate the amount of light on this pixel.
	lightIntensity = saturate(dot(input.normal, -lightDir));


	if (lightIntensity > 0.0f)
	{
		// Determine the final diffuse colour based on the diffuse colour and the
		//amount of light intensity.
		color += (diffuseColor * lightIntensity);
		// Saturate the ambient and diffuse colour.
		color = saturate(color);
	}
	// Multiply the texture pixel and the final diffuse colour
	//color = color * textureColour;


    return color;
}