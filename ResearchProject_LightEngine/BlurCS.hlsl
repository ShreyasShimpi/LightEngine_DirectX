#define GAUSSIAN_RADIUS 7

cbuffer BlurParams : register(b0)
{
	// 32
    float4 coefficients[(GAUSSIAN_RADIUS + 1) / 4];

	// 8
    int radius;  // radius <= GAUSSIAN_RADIUS
    int direction;  //  direction 0 = horizontal, 1 = vertical
}

Texture2D<float4> texIn : register(t0);
RWTexture2D<float4> texOut : register(u0);

[numthreads(8, 8, 1)]
void Blur(uint3 groupID : SV_GroupID, uint3 groupThreadID : SV_GroupThreadID, uint groupIndex : SV_GroupIndex, uint3 dispatchID : SV_DispatchThreadID)
{
    int2 pixel = int2(dispatchID.x, dispatchID.y);

    int2 dir = int2(1 - direction, direction);

	float4 finalPixelValue = { 0.0f, 0.0f, 0.0f, 0.0f };

    for (int i = -radius; i <= radius; ++i)
    {
        uint absIndex = (uint)abs(i);
		finalPixelValue += coefficients[absIndex / 4][absIndex % 4] * texIn[mad(i, dir, pixel)];
    }

	texOut[pixel] = finalPixelValue;
}