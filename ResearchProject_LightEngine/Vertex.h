#pragma once
#include <DirectXMath.h>

struct SVertexData
{
	// variables
	DirectX::XMFLOAT3 m_position;
	DirectX::XMFLOAT2 m_texureCoord;

	// constructor
	SVertexData()
		: m_position(0.0f, 0.0f, 0.0f), m_texureCoord(0.0f, 0.0f)
	{
	}
	SVertexData(float x, float y, float z, float u, float v)
		: m_position(x, y, z), m_texureCoord(u, v) 
	{
	}
};