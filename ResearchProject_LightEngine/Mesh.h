#pragma once

#include <vector>

#include "VertexBufferTypes.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"

template<typename T>
class CMesh
{
private:
	CVertexBuffer<T> m_vertexBuffer;
	CIndexBuffer m_indexBuffer;

public:
	// constructor
	CMesh(ID3D11Device* device, std::vector<T>& vertices, std::vector<unsigned short>& indices);
	CMesh(const CMesh& mesh);

	void Update(ID3D11DeviceContext* deviceContext);
};