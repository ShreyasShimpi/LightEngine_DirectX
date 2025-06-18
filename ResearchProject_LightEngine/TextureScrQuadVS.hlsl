struct VSInput
{
    float4 pos : Position;
    float2 tex : TexCoord;
};

struct VSOut
{
    float4 pos : SV_Position;
    float2 tex : TexCoord;
};

// vertex shader
VSOut main(VSInput vsin)
{
    VSOut vsout;
    vsout.pos = vsin.pos;
    vsout.tex = vsin.tex;
    return vsout;
}
