#include "Shaders.h"

/*
	--------------------
		Pixel Shader
	--------------------
*/
bool CPixelShader::CreatePixelShader(ID3D11Device* device, std::wstring pixelShaderPath)
{
	// read the cso file created from vertex shader by hlsl compiler 
	HRESULT hr = D3DReadFileToBlob(
		pixelShaderPath.c_str(),
		m_pixelBlob.GetAddressOf());

	// fail check
	if (FAILED(hr))
	{
		MessageBoxW(NULL, L"ERROR : Failed to read the pixel shader file", L"ERROR", MB_ICONERROR);
		exit(-1);
	}

	// create pixel shader
	hr = device->CreatePixelShader(
		m_pixelBlob.Get()->GetBufferPointer(),
		m_pixelBlob.Get()->GetBufferSize(),
		NULL,
		m_pixelShader.GetAddressOf());

	// fail check
	if (FAILED(hr))
	{
		MessageBoxW(NULL, L"ERROR : Failed to create pixel shader", L"ERROR", MB_ICONERROR);
		exit(-1);
	}

	// if all good, return true
	return true;
}

/*
	--------------------
		Vertex Shader
	--------------------
*/
bool CVertexShader::CreateVertexShaderAndInput(ID3D11Device* device, D3D11_INPUT_ELEMENT_DESC* inputElementDesc, UINT inputElementDescSize, std::wstring vertexShaderPath)
{
	// read the cso file created from pixel shader by hlsl compiler 
	HRESULT hr = D3DReadFileToBlob(
		vertexShaderPath.c_str(),
		m_vertexBlob.GetAddressOf());
	// fail check
	if (FAILED(hr))
	{
		MessageBoxW(NULL, L"ERROR : Failed to read the vertex shader file", L"ERROR", MB_ICONERROR);
		exit(-1);
	}

	// create vertex shader
	hr = device->CreateVertexShader(
		m_vertexBlob->GetBufferPointer(),
		m_vertexBlob->GetBufferSize(),
		NULL,
		m_vertexShader.GetAddressOf());

	// fail check
	if (FAILED(hr))
	{
		MessageBoxW(NULL, L"ERROR : Failed to create vertex shader", L"ERROR", MB_ICONERROR);
		exit(-1);
	}

	// create input layout
	// this 
	hr = device->CreateInputLayout(
		inputElementDesc,
		inputElementDescSize,
		m_vertexBlob->GetBufferPointer(),
		m_vertexBlob->GetBufferSize(),
		m_inputLayout.GetAddressOf());

	// fail check
	if (FAILED(hr))
	{
		MessageBoxW(NULL, L"ERROR : Failed to create input layout", L"ERROR", MB_ICONERROR);
		exit(-1);
	}

	// if all good, return true
	return true;
}


/*
	--------------------
		Compute Shader
	--------------------
*/
bool CComputeShader::CreateComputeShader(ID3D11Device* device, std::wstring computeShaderPath)
{
	// read the cso file created from vertex shader by hlsl compiler 
	HRESULT hr = D3DReadFileToBlob(
		computeShaderPath.c_str(),
		m_computeBlob.GetAddressOf());

	// fail check
	if (FAILED(hr))
	{
		MessageBoxW(NULL, L"ERROR : Failed to read the Compute shader file", L"ERROR", MB_ICONERROR);
		exit(-1);
	}

	// create pixel shader
	hr = device->CreateComputeShader(
		m_computeBlob.Get()->GetBufferPointer(),
		m_computeBlob.Get()->GetBufferSize(),
		NULL,
		m_computeShader.GetAddressOf());

	// fail check
	if (FAILED(hr))
	{
		MessageBoxW(NULL, L"ERROR : Failed to create compute shader", L"ERROR", MB_ICONERROR);
		exit(-1);
	}

	// if all good, return true
	return true;
}
