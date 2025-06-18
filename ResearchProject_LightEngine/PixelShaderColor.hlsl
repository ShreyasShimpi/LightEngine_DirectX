cbuffer lightBuffer : register(b0)
{
    // 16
    float3 ambientLightColor; // 12
    float ambientLightStrength; // 4
    
    // 16
    float3 dynamicLightColor; // 12
    float dynamicLightStrength; // 4
    
    // 12
    float3 dynamicLightPosition; // 12
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
    //float3 textureColor = objTexture.Sample(objSamplerState, psin.tex);
    
    // ambient(material light)
    float3 pixelColor = dynamicLightColor * dynamicLightStrength;
    
    return float4(pixelColor, 1.0f); // alpha 1.0f ( no transperency)
}
