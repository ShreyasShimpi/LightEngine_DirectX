#include "Mesh.h"


template<typename T>
CMesh<T>::CMesh(ID3D11Device* device, std::vector<T>& vertices, std::vector<unsigned short>& indices)
{
	HRESULT hr;
	hr = m_vertexBuffer.CreateVertexBuffer(device, vertices.data(), vertices.size());
	if (FAILED(hr))
	{
		MessageBoxW(NULL, L"ERROR : Failed to create vertex buffer in mesh", L"ERROR", MB_ICONERROR);
		exit(-1);
	}

	hr = m_indexBuffer.CreateIndexBuffer(device, indices.data(), indices.size());
	if (FAILED(hr))
	{
		MessageBoxW(NULL, L"ERROR : Failed to create index buffer in mesh", L"ERROR", MB_ICONERROR);
		exit(-1);
	}
}

template<typename T>
CMesh<T>::CMesh(const CMesh& mesh)
{
	m_indexBuffer = mesh.m_indexBuffer;
	m_vertexBuffer = mesh.m_vertexBuffer;
}

template<typename T>
void CMesh<T>::Update(ID3D11DeviceContext* deviceContext)
{
	UINT offset = 0;
	deviceContext->IASetVertexBuffers(0, 1, m_vertexBuffer.GetAddressOfVB(), m_vertexBuffer.GetStridePointer(), &offset);

	// IMPORTANT :
	// R16 format for short, R32 for int
	deviceContext->IASetIndexBuffer(m_indexBuffer.GetIndexBuffer(), DXGI_FORMAT::DXGI_FORMAT_R16_UINT, 0);
	deviceContext->DrawIndexed(m_indexBuffer.GetIndicesCount(), 0, 0);
}
