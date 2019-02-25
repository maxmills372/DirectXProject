// Light pixel shader
// Calculate diffuse lighting for a single directional light (also texturing)

Texture2D shaderTexture : register(t0);
SamplerState SampleType : register(s0);

cbuffer LightBuffer : register(cb0)
{
    float4 diffuseColor;
	float4 ambientColour;
    float3 lightDirection;
	float specularPower;
	float4 specularColour;
    //float padding;
};

struct InputType
{
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD0;
    float3 normal : NORMAL;
	float3 viewDirection : TEXCOORD1;
};

float4 main(InputType input) : SV_TARGET
{
    float4 textureColor;
    float3 lightDir;
    float lightIntensity;
    float4 color;
	float spec;
	float4 finalSpec;
	float3 halfway;

    // Sample the pixel color from the texture using the sampler at this texture coordinate location.
    textureColor = shaderTexture.Sample(SampleType, input.tex);
	
    // Set the defualt output colour to the ambient light value for all pixels
    color = ambientColour;

	// Invert the light direction for calculations.
    lightDir = -lightDirection;

    // Calculate the amount of light on this pixel.
    lightIntensity = max(saturate(dot(input.normal, lightDir)),0.0);

	// Determine the final amount of diffuse color based on the diffuse color combined with the light intensity.
  
	//if (lightIntensity > 0.0f)
	//{
		color += saturate(diffuseColor * lightIntensity);

		//blinn phong
		halfway = normalize(lightDir + input.viewDirection);
		spec = pow(max(dot(input.normal, halfway), 0.0), specularPower);
		finalSpec = saturate(specularColour * spec);

	//}

    // Multiply the texture pixel and the final diffuse color to get the final pixel color result.
    color = color * textureColor;

	color = saturate(color + finalSpec);

    return color;
}