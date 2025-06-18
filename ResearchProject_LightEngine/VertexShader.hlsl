// color
//struct VSOut
//{
//	float3 color : Color;
//	float4 pos : SV_Position;
//};
//
//VSOut main(float2 pos : Position, float3 color : Color)
//{
//	VSOut vso;
//	vso.pos = float4(pos.x, pos.y, 0.0f, 1.0f);
//	vso.color = color;
//	return vso;
//}

// texture
struct VSOut
{
	float2 tex : TexCoord;
	float4 pos : SV_Position;
};

cbuffer CBuf
{
	matrix transform;
};

VSOut main(float3 pos : Position, float2 tex : TexCoord)
{
	VSOut vso;
	vso.pos = mul(float4(pos.x, pos.y, pos.z, 1.0f), transform);
	vso.tex = tex;
	return vso;
}
//
//cbuffer CBuf
//{
//	matrix transform;
//};
//
//float4 main(float3 pos : Position) : SV_Position
//{
//	return mul(float4(pos,1.0f),transform);
//}