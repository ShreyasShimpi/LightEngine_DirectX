struct VSInput
{
    float3 pos : Position;
    float3 normal : Normal;
};

struct VSOutput
{
    float4 pos : SV_Position;
    float3 normal : Normal;
    float3 worldPos : WorldPosition;
};

cbuffer CBuf
{
	// 16 byte
    matrix worldViewProjectionTransform; // 16
	
	// 16 byte
    matrix worldTransform; // 16
};

VSOutput main(VSInput vsin)
{
    VSOutput vsout;
    vsout.pos = mul(float4(vsin.pos, 1.0f), worldViewProjectionTransform);
    
	// make w component of vector 0 to ignore translation component of world matrix
    vsout.normal = normalize(mul(float4(vsin.normal, 0.0f), worldTransform));
    vsout.worldPos = mul(float4(vsin.pos, 1.0f), worldTransform);
    return vsout;
}