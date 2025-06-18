#pragma once
#include <d3d11.h>
#include <memory>
#include <wrl/client.h>

// template for different vertex structure declaration
template<typename VertexStruct>
class CVertexBuffer
{
private:
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_vertexBuffer;
	UINT m_verticesCount;
	UINT m_stride; // gap between two structures

	CVertexBuffer(const CVertexBuffer<VertexStruct>& rhs);

public:
	// constructor
	CVertexBuffer()
	{
		m_verticesCount = 0u;
		m_stride = sizeof(VertexStruct);
	}

	// destructor
	~CVertexBuffer()
	{
	}

	// function
	// it create the vertex buffer with the vertex data we pass in
	HRESULT CreateVertexBuffer(ID3D11Device* device, VertexStruct* vertexData, UINT verticesCount)
	{
		// the create buffer function reallocated the memory if called again
		// so its initalized again, then we clear out the existing content in it
		if (m_vertexBuffer.Get() != nullptr)
		{
			m_vertexBuffer.Reset();
		}

		m_verticesCount = verticesCount;

		// create buffer description
		D3D11_BUFFER_DESC vertexBufferDesc = { 0 };

		// fill the properties
		vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		vertexBufferDesc.CPUAccessFlags = 0u;
		vertexBufferDesc.MiscFlags = 0u;
		vertexBufferDesc.ByteWidth = sizeof(VertexStruct) * verticesCount;
		vertexBufferDesc.StructureByteStride = sizeof(VertexStruct);

		// create sub resource data
		D3D11_SUBRESOURCE_DATA vertexSubresourceData = { 0 };

		// fill sub resource data
		vertexSubresourceData.pSysMem = vertexData;

		// the function only wants to use it as an array (starting ptr),
		// we only need to give.
		// we also fill this buffer to device
		HRESULT hr = device->CreateBuffer(&vertexBufferDesc, &vertexSubresourceData, m_vertexBuffer.GetAddressOf());
		if (FAILED(hr))
		{
			MessageBoxW(NULL, L"ERROR : Failed to vertex consatnt buffer", L"ERROR", MB_ICONERROR);
			exit(-1);
		}
		return hr;
	}

	// getters
	ID3D11Buffer* GetVertexBuffer() const { return m_vertexBuffer.Get(); }
	ID3D11Buffer* const* GetAddressOfVB() const { return m_vertexBuffer.GetAddressOf(); }
	UINT GetVertexCount() const { return m_verticesCount; }
	const UINT GetStride() const { return m_stride; }
	const UINT* GetStridePointer() const { return &m_stride; }
};
