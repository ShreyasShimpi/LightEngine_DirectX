#pragma once
#include <DirectXMath.h>

// vertex shader constant buffer
// should be alligned 16 bytes
struct STransformsCB
{
	DirectX::XMMATRIX m_worldViewProjectionTransform; // 16

	DirectX::XMMATRIX m_worldTransform; // 16
};


// pixel shader constant buffer
// should be alligned 16 bytes
struct SConstantBufferPS
{
	// 16
	DirectX::XMFLOAT3 m_ambientLightColor; // 12
	float m_ambientLightStrength; // 4

	//16
	DirectX::XMFLOAT3 m_pointLightColor; // 12
	float m_pointLightStrength; // 4

	// 16
	DirectX::XMFLOAT3 m_pointLightPosition; // 12
	float padding1 = 0.0f; // 4

	// 16
	DirectX::XMFLOAT3 m_pointLightAttenuation; // 12
	float m_pointLightRadius; // 4

	// 12
	DirectX::XMFLOAT3 m_directionalLightDir;
};

// light parameters
struct SLightParametersCB
{
	// 16
	DirectX::XMFLOAT3 m_ambientLightColor; // 12
	float m_ambientLightStrength; // 4

	//16
	DirectX::XMFLOAT3 m_pointLightColor; // 12
	float m_pointLightStrength; // 4

	// 16
	DirectX::XMFLOAT3 m_pointLightPosition; // 12
	float padding1 = 0.0f; // 4

	// 16
	DirectX::XMFLOAT3 m_pointLightAttenuation; // 12
	float m_pointLightRadius; // 4

	// 12
	DirectX::XMFLOAT3 m_directionalLightDir;
};

// material parameters
struct SMaterialParametersCB
{
	alignas(16) DirectX::XMFLOAT3 m_materialColor;
};

// for compute shaders
struct SThresholdParameter
{
	alignas(16) float m_threshold;
};

struct SCompositeParameter
{
	alignas(16) float m_coefficient;
};

// (GAUSSIAN_RADIUS + 1) must be multiple of 4 because of the way we set up the shader
#define GAUSSIAN_RADIUS 7

struct SBlurringParameters
{
	alignas(16) float m_coefficients[GAUSSIAN_RADIUS + 1]; // 32

	int m_radius; // 4

	 // 0 = horizontal, 1 = vertical
	int m_direction;//4
};