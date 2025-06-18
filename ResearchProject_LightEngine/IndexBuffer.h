#pragma once

#include <d3d11.h>
#include <wrl/client.h> // used for COM ptr

class CIndexBuffer
{
private:
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_indexBuffer;
	UINT m_indicesCount;

public:
	// constructor
	CIndexBuffer()
	{
		m_indicesCount = 0u;
	}

	// destructor
	~CIndexBuffer()
	{
	}

	// function
	// it create the vertex buffer with the index array we pass in.
	HRESULT CreateIndexBuffer(ID3D11Device* device, unsigned short* indicesData, UINT indicesCount)
	{
		m_indicesCount = indicesCount;

		// the create buffer function reallocated the memory if called again
		// so its initalized again, then we clear out the existing content in it
		if (m_indexBuffer.Get() != nullptr)
		{
			m_indexBuffer.Reset();
		}

		// create buffer description
		D3D11_BUFFER_DESC indexBufferDesc = {0};

		// fill the properties
		indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		indexBufferDesc.CPUAccessFlags = 0u;
		indexBufferDesc.MiscFlags = 0u;
		indexBufferDesc.ByteWidth = sizeof(unsigned short) * indicesCount;
		indexBufferDesc.StructureByteStride = sizeof(unsigned short);

		// create sub resource data
		D3D11_SUBRESOURCE_DATA indexSubresourceData = {0};

		// fill sub resource data
		indexSubresourceData.pSysMem = indicesData;

		// the function only wants to use it as an array (starting ptr),
		// we only need to give.
		// we also fill this buffer to device
		HRESULT hr = device->CreateBuffer(&indexBufferDesc, &indexSubresourceData, &m_indexBuffer);
		return hr;
	}

	HRESULT CreateIndexBuffer(ID3D11Device* device, unsigned int* indicesData, UINT indicesCount)
	{
		m_indicesCount = indicesCount;

		// create buffer description
		D3D11_BUFFER_DESC indexBufferDesc = {0};

		// fill the properties
		indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		indexBufferDesc.CPUAccessFlags = 0u;
		indexBufferDesc.MiscFlags = 0u;
		indexBufferDesc.ByteWidth = sizeof(unsigned int) * indicesCount;
		indexBufferDesc.StructureByteStride = sizeof(unsigned int);

		// create sub resource data
		D3D11_SUBRESOURCE_DATA indexSubresourceData = {0};

		// fill sub resource data
		indexSubresourceData.pSysMem = indicesData;

		// the function only wants to use it as an array (starting ptr),
		// we only need to give.
		// we also fill this buffer to device
		HRESULT hr = device->CreateBuffer(&indexBufferDesc, &indexSubresourceData, &m_indexBuffer);
		return hr;
	}

	// getters
	ID3D11Buffer* GetIndexBuffer() const { return m_indexBuffer.Get(); }
	ID3D11Buffer* const* GetAddressOfIB() const { return m_indexBuffer.GetAddressOf(); }
	UINT GetIndicesCount() const { return m_indicesCount; }
};