cbuffer CompositeParams : register(b0)
{
    float coefficient;
}

struct PSInput
{
    float4 pos : SV_Position;
    float2 tex : TexCoord;
};

Texture2D tex0 : register(t0);
Texture2D tex1 : register(t1);
SamplerState texSampler : register(s0);

float4 main(PSInput psin) : SV_TARGET
{
    return mad(coefficient, tex1.Sample(texSampler, psin.tex), tex0.Sample(texSampler, psin.tex));
}
