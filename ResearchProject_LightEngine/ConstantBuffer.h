#pragma once

#include <d3d11.h>
#include <wrl/client.h>


// template for different vertex structure declaration
template<typename ConstantBufferStruct>
class CConstantBuffer
{
private:
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_constantBuffer;
	UINT m_constantBufferSize;

public:
	// constructor
	CConstantBuffer()
	{
		m_constantBufferSize = 0u;
	}
	CConstantBuffer(const CConstantBuffer<ConstantBufferStruct>& rhs) = delete;

	// destructor
	~CConstantBuffer()
	{
	}

	// function
	// it create the vertex buffer with the vertex data we pass in
	HRESULT CreateConstantBuffer(ID3D11Device* device)
	{
		// the create buffer function reallocated the memory if called again
		// so its initalized again, then we clear out the existing content in it
		if (m_constantBuffer.Get() != nullptr)
		{
			m_constantBuffer.Reset();
		}

		// create buffer description
		D3D11_BUFFER_DESC constantBufferDesc = {};
		constantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		constantBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
		constantBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		constantBufferDesc.MiscFlags = 0u;
		constantBufferDesc.ByteWidth = static_cast<UINT>(sizeof(ConstantBufferStruct) + (16 - (sizeof(ConstantBufferStruct) % 16))); // allign it to 16 bytes
		constantBufferDesc.StructureByteStride = 0u;

		// the function only wants to use it as an array (starting ptr),
		// we only need to give.
		// we also fill this buffer to device
		HRESULT hr = device->CreateBuffer(&constantBufferDesc, 0u, m_constantBuffer.GetAddressOf());
		if (FAILED(hr))
		{
			MessageBoxW(NULL, L"ERROR : Failed to create constant buffer", L"ERROR", MB_ICONERROR);
			exit(-1);
		}
		return hr;
	}

	// update the constant buffer data and map to device context
	bool UpdateAndMapData(ID3D11DeviceContext* deviceContext, ConstantBufferStruct& data)
	{
		D3D11_MAPPED_SUBRESOURCE mappedResource;
		HRESULT hr = deviceContext->Map(
			m_constantBuffer.Get(),
			0,
			D3D11_MAP_WRITE_DISCARD,
			0,
			&mappedResource);

		if (FAILED(hr))
		{
			MessageBoxW(NULL, L"ERROR : Could not Map data to constant buffer", L"ERROR", MB_ICONERROR);
			exit(-1);
		}

		CopyMemory(mappedResource.pData, &data, sizeof(ConstantBufferStruct));

		deviceContext->Unmap(m_constantBuffer.Get(), 0);
		return true;
	}

	// getters
	ID3D11Buffer* GetConstantBuffer() const { return m_constantBuffer.Get(); }
	ID3D11Buffer* const* GetAddressOfCB() const { return m_constantBuffer.GetAddressOf(); }
	UINT GetConstantBufferSize() const { return m_constantBufferSize; }
};