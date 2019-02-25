// Light pixel shader
// Calculate diffuse lighting for a single directional light (also texturing)

Texture2D shaderTexture : register(t0);
SamplerState SampleType : register(s0);

cbuffer LightBuffer : register(cb0)
{
	float4 diffusecolour[2];
	float4 ambientColour[2];
	float4 position[2];
	float4 direction[2];
	//float specularPower[2];
	//float4 specularColour[2];
	float3 padding;
	float rangeValue;
	float3 type[2];
};

struct InputType
{
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD0;
    float3 normal : NORMAL;
	float3 position3D : TEXCOORD1;
	//float3 viewDirection : TEXCOORD2;
};

float4 main(InputType input) : SV_TARGET
{
    float4 texturecolour;
    float4 colour[2];
	float4 finalColour;
	float lightIntensity;

	float3 lightDir;

	//attenuation variables
	float attenuation;
	float distance;
	float constant_factor = 1.0f;
	float linear_factor = 0.125f;
	float quadratic_factor = 0.0f;
	float range = rangeValue;

	//specular variables
	float spec;
	float4 finalSpec;
	float3 halfway;

    // Sample the pixel colour from the texture using the sampler at this texture coordinate location.
    texturecolour = shaderTexture.Sample(SampleType, input.tex);
	
	// Set the defualt output colour to the ambient light value for all pixels
	finalColour = ambientColour[0]; //	WHAT DO I DO HERE ///////////////////////


	//loop through all the lights
	for (int i = 0; i < 2; i++)
	{		
		colour[i] = float4(0.0f, 0.0f, 0.0f, 1.0f);

		if (type[i].x == 1) //POINT
		{
			//calculate light direction
			lightDir = -(input.position3D - position[i]);			
		}
		else if(type[i].x == 0) //DIRECTIONAL
		{
			//set light direction to the set direction variable
			lightDir = -direction[i];
		}
		
		//Calculate distance
		distance = length(lightDir);
		
		if (distance < range)
		{
			// Normalize light direction
			lightDir = normalize(lightDir);

			// Calculate diffuse intesity
			lightIntensity = saturate(dot(input.normal, lightDir));

			if (lightIntensity > 0.0f)
			{
				// Determine the final diffuse colour based on the diffuse colour and the
				// amount of light intensity.
				colour[i] += (diffusecolour[i] * lightIntensity);
				colour[i] = saturate(colour[i]);

				//OR:
				//colour += saturate(diffusecolour * lightIntensity);

				//blinn phong
				//halfway = normalize(lightDir + input.viewDirection);
				//spec = pow(max(dot(input.normal, halfway), 0.0), specularPower);
				//finalSpec = saturate(specularColour * spec);


				// Calculate attenuation value
				attenuation = 1 / (constant_factor + (linear_factor * distance) + (quadratic_factor * (distance * distance)));

				// Combine
				colour[i] = (colour[i] * attenuation);
			}
		}		
	}

	
	// Determine the final amount of diffuse colour based on the diffuse colour combined with the light intensity.
	finalColour = finalColour + (colour[0] + colour[1]);

    // Multiply the texture pixel and the final diffuse colour to get the final pixel colour result.
	//finalColour = finalColour * texturecolour;

	finalColour = saturate(finalColour);// +finalSpec);

    return finalColour;

}