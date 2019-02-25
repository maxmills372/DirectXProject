Texture2D shaderTexture : register(t0);
Texture2D shadowTexture : register(t1);

SamplerState SampleTypeWrap  : register(s0);
SamplerState SampleTypeClamp : register(s1);

cbuffer LightBuffer : register(cb0)
{
	float4 ambientColor;
	float4 diffuseColor;
	//float4 ambientColor2;
	//float4 diffuseColor2;
};


struct InputType
{
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
	float4 lightViewPosition : TEXCOORD1;
	float3 lightPos : TEXCOORD2;
	//float4 lightViewPosition2 : TEXCOORD3;
	//float3 lightPos2 : TEXCOORD4;
};


float4 main(InputType input) : SV_TARGET
{
	float bias;
	float4 color;
	float4 projectTexCoord;
	float depthValue;
	float lightDepthValue;
	float lightIntensity;
	float shadowValue;
	float4 textureColor;
	

	// Set the default output color to the ambient light value for all pixels.
	color = ambientColor;

	// Calculate the amount of light on this pixel.
	lightIntensity = saturate(dot(input.normal, input.lightPos));
	if (lightIntensity > 0.0f)
	{
		// Determine the light color based on the diffuse color and the amount of light intensity.
		color += (diffuseColor * lightIntensity);

		// Saturate the light color.
		color = saturate(color);
	}

	// Sample the pixel color from the texture using the sampler at this texture coordinate location.
	textureColor = shaderTexture.Sample(SampleTypeWrap, input.tex);

	// Combine the light and texture color.
	color = color * textureColor;

	// Calculate the projected texture coordinates to be used with the shadow texture.
	projectTexCoord.x = input.lightViewPosition.x / input.lightViewPosition.w / 2.0f + 0.5f;
	projectTexCoord.y = -input.lightViewPosition.y / input.lightViewPosition.w / 2.0f + 0.5f;

	// Second light.
	//projectTexCoord.x = input.lightViewPosition2.x / 2.0f + 0.5f;
	//projectTexCoord.y = -input.lightViewPosition2.y / 2.0f + 0.5f;

	//if ((saturate(projectTexCoord.x) == projectTexCoord.x) && (saturate(projectTexCoord.y) == projectTexCoord.y))
	//{
	//	depthValue = depthMapTexture2.Sample(SampleTypeClamp, projectTexCoord).r;

	//	lightDepthValue = input.lightViewPosition2.z / input.lightViewPosition2.w;
	//	lightDepthValue = lightDepthValue - bias;

	//	if (lightDepthValue < depthValue)
	//	{
	//		lightIntensity = saturate(dot(input.normal, input.lightPos2));
	//		if (lightIntensity > 0.0f)
	//		{
	//			//color += (diffuseColor2 * lightIntensity);

	//			//colour it white if illuminated
	//			color = float4(1.0f, 1.0f, 1.0f, 1.0f);
	//		}
	//	}
	//}
	//else
	//{
	//	// Calculate the amount of light on this pixel.
	//	lightIntensity = saturate(dot(input.normal, input.lightPos2));

	//	if (lightIntensity > 0.0f)
	//	{
	//		// Determine the final diffuse color based on the diffuse color and the amount of light intensity.
	//		color += (diffuseColor2 * lightIntensity);

	//		// Saturate the final light color.
	//		//color = saturate(color);
	//	}
	//}

	// Saturate the final light color.
	//color = saturate(color);

	// Sample the pixel color from the texture using the sampler at this texture coordinate location.
	shadowValue = shadowTexture.Sample(SampleTypeClamp, projectTexCoord);

	// Combine the light and texture color.
	color = color * shadowValue;

	return color;
}