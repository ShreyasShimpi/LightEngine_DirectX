cbuffer ThresholdParameters : register(b0)
{
    float threshold;
}

Texture2D<float4> texIn : register(t0);
RWTexture2D<float4> texOut : register(u0);

[numthreads(8, 8, 1)]
void ThresholdAndDownsample(uint3 groupID : SV_GroupID, uint3 groupThreadID : SV_GroupThreadID, uint groupIndex : SV_GroupIndex, uint3 dispatchID : SV_DispatchThreadID)
{
    // output pixel in half resolution
    uint2 pixel = uint2(dispatchID.x, dispatchID.y);

    // bilinear interpolation for downsampling
    uint2 inputPixel = pixel * 2;

    float4 intensity0 = lerp(texIn[inputPixel], texIn[inputPixel + uint2(1, 0)], 0.5);
    float4 intensity1 = lerp(texIn[inputPixel + uint2(0, 1)], texIn[inputPixel + uint2(1, 1)], 0.5);
    float4 totalIntensity = lerp(intensity0, intensity1, 0.5);

    // thresholding on downsampled value
    float intensityTest = (float) (length(totalIntensity.rgb) > threshold);

	texOut[pixel] = float4(intensityTest * totalIntensity.rgb, 1.0);
}