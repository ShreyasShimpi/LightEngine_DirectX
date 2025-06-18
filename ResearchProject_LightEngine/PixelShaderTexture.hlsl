cbuffer lightBuffer : register(b0)
{
	// 16
	float3 ambientLightColor; // 12
	float ambientLightStrength; // 4

	// 16
	float3 pointLightColor; // 12
	float pointLightStrength; // 4

	// 16
	float3 pointLightPosition; // 12
	float padding; // 4

	// 16
	float3 pointLightAttenuation; //12
	float pointLightRadius; //4

	// 12
	float3 directionalLightDir; // 12
}

struct PSInput
{
	float4 pos : SV_Position;
	float2 tex : TexCoord;
	float3 normal : Normal;
	float3 worldPos : WorldPosition;
};

Texture2D objTexture : TEXTURE: register(t0);
SamplerState objSamplerState : SAMPLER: register(s0);

float4 main(PSInput psin) : SV_TARGET
{
	//float3 textureColor = objTexture.Sample(objSamplerState, psin.tex);
	float3 textureColor = {0.5f, 0.5f, 0.5f };

// ambient(material light)
float3 ambientLight = ambientLightColor * ambientLightStrength;

// diffuse light - point light
float3 pointLightPosVector = normalize(pointLightPosition - psin.worldPos);
float3 pointLightDiffuseStrength = max(0, dot(pointLightPosVector, psin.normal));
float pointLightDistance = distance(pointLightPosition, psin.worldPos);
float pointLightAttenuationFactor = 1 / (pointLightAttenuation.x +
pointLightAttenuation.y * pointLightDistance +
pointLightAttenuation.z * pow(pointLightDistance, 2));

float3 pointLightDiffuse = pointLightDiffuseStrength * pointLightColor * pointLightStrength;

// diffuse light -  directional light
float3 directionLightDiffuseStrength = max(0, dot(directionalLightDir, -psin.normal));

// total light
float3 totalLight = ambientLight + directionLightDiffuseStrength
 + pointLightAttenuationFactor * pointLightDiffuse;

// final pixel color
float3 finalPixelColor = textureColor * totalLight;

return float4(finalPixelColor, 1.0f); // alpha 1.0f ( no transperency)
}
