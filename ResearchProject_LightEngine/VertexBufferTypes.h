#pragma once
#include <DirectXMath.h>

// for textures
struct SVertexDataTexture
{
	// variables
	DirectX::XMFLOAT3 m_position;
	DirectX::XMFLOAT2 m_texureCoord;

	// constructor
	SVertexDataTexture()
		: m_position(0.0f, 0.0f, 0.0f), m_texureCoord(0.0f, 0.0f)
	{
	}
	SVertexDataTexture(float x, float y, float z, float u, float v)
		: m_position(x, y, z), m_texureCoord(u, v)
	{
	}
	SVertexDataTexture(DirectX::XMFLOAT3 position, DirectX::XMFLOAT2 texureCoord)
		: m_position(position), m_texureCoord(texureCoord)
	{
	}
};

// for textures WITH NORMALS
struct SVertexDataTextureNormal
{
	// variables
	DirectX::XMFLOAT3 m_position;
	DirectX::XMFLOAT2 m_texureCoord;
	DirectX::XMFLOAT3 m_normal;

	// constructor
	SVertexDataTextureNormal()
		: m_position(0.0f, 0.0f, 0.0f),
		m_texureCoord(0.0f, 0.0f),
		m_normal(0.0f, 0.0f, 0.0f)
	{
	}
	SVertexDataTextureNormal(float x, float y, float z, float u, float v, float nX, float nY, float nZ)
		: m_position(x, y, z), m_texureCoord(u, v), m_normal(nX, nY, nZ)
	{
	}
	SVertexDataTextureNormal(DirectX::XMFLOAT3 position, DirectX::XMFLOAT2 texureCoord, DirectX::XMFLOAT3 normal)
		: m_position(position), m_texureCoord(texureCoord), m_normal(normal)
	{
	}
};

//  for colors
struct SVertexDataColor
{
	// variables
	DirectX::XMFLOAT3 m_position;
	DirectX::XMFLOAT3 m_color;

	// constructor
	SVertexDataColor()
		: m_position(0.0f, 0.0f, 0.0f), m_color(0.0f, 0.0f, 0.0f)
	{
	}
	SVertexDataColor(float x, float y, float z, float r, float g, float b)
		: m_position(x, y, z), m_color(r, g, b)
	{
	}
	SVertexDataColor(DirectX::XMFLOAT3 position, DirectX::XMFLOAT3 color)
		: m_position(position), m_color(color)
	{
	}
};