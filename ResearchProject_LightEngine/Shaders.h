#pragma once

// external includes
#include <d3d11.h>
#include <wrl/client.h>
#include <d3dcompiler.h>
#include <string> // for wide string support

// link d3d library files
#pragma comment(lib, "D3DCompiler.lib")
#pragma comment(lib,"d3d11.lib")

/*
	Pixel Shader
*/
class CPixelShader
{
private:
	// variables
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pixelShader;
	Microsoft::WRL::ComPtr<ID3D10Blob> m_pixelBlob;

public:
	// getters
	ID3D11PixelShader* GetPixelShader() const { return m_pixelShader.Get(); };
	ID3D10Blob* GetPixelBlob() const { return m_pixelBlob.Get(); };

	// function
	bool CreatePixelShader(ID3D11Device* device, std::wstring pixelShaderPath);
};



/*
	Vertex Shader
*/
class CVertexShader
{
private:
	// variables
	Microsoft::WRL::ComPtr<ID3D11VertexShader> m_vertexShader;
	Microsoft::WRL::ComPtr<ID3D10Blob> m_vertexBlob;
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout; // input layout

public:
	// getters
	ID3D11VertexShader* GetVertexShader() const { return m_vertexShader.Get(); };
	ID3D10Blob* GetVertexBlob() const { return m_vertexBlob.Get(); }
	ID3D11InputLayout* GetInputLayout() const { return m_inputLayout.Get(); };

	// function
	bool CreateVertexShaderAndInput(ID3D11Device* device, D3D11_INPUT_ELEMENT_DESC* inputElementDesc, UINT inputElementDescSize, std::wstring vertexShaderPath);
};


/*
	Compute Shader
*/
class CComputeShader
{
private:
	// variables
	Microsoft::WRL::ComPtr<ID3D11ComputeShader> m_computeShader;
	Microsoft::WRL::ComPtr<ID3D10Blob> m_computeBlob;

public:
	// getters
	ID3D11ComputeShader* GetComputeShader() const { return m_computeShader.Get(); };
	ID3D10Blob* GetComputeShaderBlob() const { return m_computeBlob.Get(); }

	// function
	bool CreateComputeShader(ID3D11Device* device, std::wstring computeShaderPath);
};
