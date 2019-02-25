// Light pixel shader
// Calculate diffuse lighting for a single directional light (also texturing)

Texture2D shaderTexture : register(t0);
SamplerState SampleType : register(s0);

cbuffer LightBuffer : register(cb0)
{
    float4 diffuseColor[4];
	float4 ambientColour[4];
    float4 position[4];
 
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
    float4 textureColor;
    float3 lightDir[4];
    float lightIntensity;
    float4 color[4];
	float4 finalColour;

	finalColour = float4(0.0f, 0.0f, 0.0f, 1.0f);
	color[0] = float4(0.0f, 0.0f, 0.0f, 1.0f);
	color[1] = float4(0.0f, 0.0f, 0.0f, 1.0f);
	color[2] = float4(0.0f, 0.0f, 0.0f, 1.0f);
	color[3] = float4(0.0f, 0.0f, 0.0f, 1.0f);

    // Sample the pixel color from the texture using the sampler at this texture coordinate location.
    textureColor = shaderTexture.Sample(SampleType, input.tex);
	
	for (int i = 0; i < 4; i++)
	{
		// Set the defualt output colour to the ambient light value for all pixels
		//color[i] = ambientColour[i];

		// Invert the light direction for calculations.
		lightDir[i] = normalize(input.position3D - position[i]);

		// Calculate the amount of light on this pixel.
		lightIntensity = saturate(dot(input.normal, -lightDir[i]));

		if (lightIntensity > 0.0f)
		{
			// Determine the final diffuse colour based on the diffuse colour and the
			//amount of light intensity.
			color[i] += (diffuseColor[i] * lightIntensity); 
			color[i] = saturate(color[i]);

			//OR:
			//color += saturate(diffuseColor * lightIntensity);

		}

	}

	
	// Determine the final amount of diffuse color based on the diffuse color combined with the light intensity.
	finalColour = finalColour + (color[0] + color[1] + color[2] + color[3]);

	

    // Multiply the texture pixel and the final diffuse color to get the final pixel color result.
    //color = color * textureColor;

    return finalColour;
}