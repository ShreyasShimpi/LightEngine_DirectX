cbuffer lightParameters : register(b0)
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

Texture2D objTexture : TEXTURE : register(t0);
SamplerState objSamplerState : SAMPLER : register(s0);

float4 main(PSInput psin) : SV_TARGET
{
    // ambient(material light)
    float3 pixelColor = pointLightColor * pointLightStrength;
    return float4(pixelColor, 1.0f); // alpha 1.0f ( no transperency)
}
