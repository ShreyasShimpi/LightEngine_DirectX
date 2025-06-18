// internal includes
#include "Graphics.h"
#include "Data.h"

// external includes
#include <d3dcompiler.h>
#include <directxmath.h>
#include <vector>

// link d3d library files
#pragma comment(lib,"d3d11.lib")
#pragma comment(lib,"D3DCompiler.lib")

// ImGUI
#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_dx11.h"
#include "ImGui/imgui_impl_win32.h"

void CGraphics::CreateAllBuffers()
{
	// create cube buffers
	m_cube.InitializeCube(m_deviceD3D11.Get());

	// create light
	m_pointLight.InitializeLight(m_deviceD3D11.Get());

	// create bloo
	m_bloom.InitializeBloom(m_deviceD3D11.Get());
}

void CGraphics::CreateSwapChain(HWND hWnd)
{
	// used to create device
	// passed as a address to pointer to structure
	DXGI_SWAP_CHAIN_DESC sd = {};
	sd.BufferDesc.Width = 0;
	sd.BufferDesc.Height = 0;
	sd.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator = 0;
	sd.BufferDesc.RefreshRate.Denominator = 0;
	sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.BufferCount = 1;
	sd.OutputWindow = hWnd;
	sd.Windowed = TRUE;
	sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	// for checking results of d3d functions
	HRESULT hr;
	UINT swapCreateFlags = 0u;

	// create device and front/back buffers, and swap chain and rendering context
	hr = D3D11CreateDeviceAndSwapChain(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		swapCreateFlags,
		nullptr,
		0,
		D3D11_SDK_VERSION,
		&sd,
		&m_swapChainDXGI,
		&m_deviceD3D11,
		nullptr,
		&m_deviceContextD3D11);

	if (FAILED(hr))
	{
		MessageBoxW(NULL, L"ERROR : Failed to create device and swap chain", L"ERROR", MB_ICONERROR);
		exit(-1);
	}

	// front render target
	{
		// gain access to texture sub resource in swap chain (back buffer)
		Microsoft::WRL::ComPtr<ID3D11Resource> backBuffer;
		hr = m_swapChainDXGI->GetBuffer(
			0,
			__uuidof(ID3D11Resource),
			&backBuffer);

		if (FAILED(hr))
		{
			MessageBoxW(NULL, L"ERROR : Failed to get buffer from swap chain", L"ERROR", MB_ICONERROR);
			exit(-1);
		}

		// create render target view
		m_deviceD3D11->CreateRenderTargetView(
			backBuffer.Get(),
			nullptr,
			&m_backBufferRTV);

		if (FAILED(hr))
		{
			MessageBoxW(NULL, L"ERROR : Failed to create render target view", L"ERROR", MB_ICONERROR);
			exit(-1);
		}
	}
}

void CGraphics::CreateSampler()
{
	// Create Sampler State
	D3D11_SAMPLER_DESC samplerDesc = {};
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;

	HRESULT hr = m_deviceD3D11->CreateSamplerState(&samplerDesc, &m_samplerState);

	if (FAILED(hr))
	{
		MessageBoxW(NULL, L"ERROR : Failed to create sampler state", L"ERROR", MB_ICONERROR);
		exit(-1);
	}
}

void CGraphics::CreateRasterizer()
{
	CD3D11_RASTERIZER_DESC rasterizerDesc(D3D11_DEFAULT);
	rasterizerDesc.CullMode = D3D11_CULL_MODE::D3D11_CULL_FRONT;

	// Create the rasterizer state from the description we just filled out.
	HRESULT hr = m_deviceD3D11->CreateRasterizerState(&rasterizerDesc, &m_rasterizerState);
	if (FAILED(hr))
	{
		MessageBoxW(NULL, L"ERROR : Failed to create rasterizer state", L"ERROR", MB_ICONERROR);
		exit(-1);
	}
}

void CGraphics::CreateDepthStencil()
{
	HRESULT hr;
	// required pointers
	Microsoft::WRL::ComPtr<ID3D11Texture2D> depthStencil;

	// create depth stencil texture
	D3D11_TEXTURE2D_DESC depthDesc = { 0 };

	depthDesc.Width = WIDTH;
	depthDesc.Height = HEIGHT;
	depthDesc.MipLevels = 1u;
	depthDesc.ArraySize = 1u;
	depthDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthDesc.SampleDesc.Count = 1u;
	depthDesc.SampleDesc.Quality = 0u;
	depthDesc.Usage = D3D11_USAGE_DEFAULT;
	depthDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;

	hr = m_deviceD3D11->CreateTexture2D(&depthDesc, nullptr, &depthStencil);
	if (FAILED(hr))
	{
		MessageBoxW(NULL, L"ERROR : Failed to create texture for depth stencil", L"ERROR", MB_ICONERROR);
		exit(-1);
	}

	hr = m_deviceD3D11->CreateDepthStencilView(
		depthStencil.Get(),
		NULL, // depth stencil view
		&m_depthStencilView
	);
	if (FAILED(hr))
	{
		MessageBoxW(NULL, L"ERROR : Failed to create depth stencil view", L"ERROR", MB_ICONERROR);
		exit(-1);
	}

	// create depth stencil state
	D3D11_DEPTH_STENCIL_DESC depthStencilDesc = { 0 };
	depthStencilDesc.DepthEnable = TRUE;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;

	hr = m_deviceD3D11->CreateDepthStencilState(&depthStencilDesc, m_depthStencilState.GetAddressOf());
	if (FAILED(hr))
	{
		MessageBoxW(NULL, L"ERROR : Failed to create depth stencil state( with depth)", L"ERROR", MB_ICONERROR);
		exit(-1);
	}

	// for no depth
	depthStencilDesc.DepthEnable = FALSE;
	hr = m_deviceD3D11->CreateDepthStencilState(&depthStencilDesc, m_depthStencilStateNoDepth.GetAddressOf());
	if (FAILED(hr))
	{
		MessageBoxW(NULL, L"ERROR : Failed to create depth stencil state( without depth)", L"ERROR", MB_ICONERROR);
		exit(-1);
	}
}

void CGraphics::CreateViewPort()
{
	// configure viewport
	D3D11_VIEWPORT vp;

	vp.Width = WIDTH * g_viewPortScaleW;
	vp.Height = HEIGHT * g_viewPortScaleH;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0.0f;
	vp.TopLeftY = 0.0f;
	m_deviceContextD3D11->RSSetViewports(1u, &vp);

}

void CGraphics::SetupImGUI(HWND hWnd)
{
	//Setup ImGui
	IMGUI_CHECKVERSION();

	// craete context
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();

	ImGui_ImplDX11_Init(m_deviceD3D11.Get(), m_deviceContextD3D11.Get());
	ImGui_ImplWin32_Init(hWnd);

	// style ( I like Dark theme everywhere...)
	ImGui::StyleColorsDark();
}

CGraphics::CGraphics()
{
}

CCamera& CGraphics::GetCamera()
{
	return m_camera;
};

void CGraphics::InitializeGraphics(HWND hWnd)
{
	CreateSwapChain(hWnd);
	CreateSampler();
	CreateDepthStencil();
	CreateViewPort();
	CreateAllBuffers();
	SetupImGUI(hWnd);
}


void CGraphics::ClearFrame(float red, float green, float blue) noexcept
{
	const float color[] = { red,green,blue,1.0f };
	if (m_bloom.IsBloomOn())
	{
		m_deviceContextD3D11->ClearRenderTargetView(m_bloom.GetFirstRTV(), color);
	}
	else
	{
		m_deviceContextD3D11->ClearRenderTargetView(m_backBufferRTV.Get(), color);
	}

	m_deviceContextD3D11->ClearDepthStencilView(m_depthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0u);
}

void CGraphics::Update()
{
	UINT offset = 0u;

	if (m_bloom.IsBloomOn())
	{
		m_deviceContextD3D11->OMSetRenderTargets(1u, m_bloom.GetFirstRTVAddress(), m_depthStencilView.Get());
	}
	else
	{
		m_deviceContextD3D11->OMSetRenderTargets(1u, m_backBufferRTV.GetAddressOf(), m_depthStencilView.Get());
	}

	{
		m_deviceContextD3D11->OMSetDepthStencilState(m_depthStencilState.Get(), 0u);// bind depth state
		m_deviceContextD3D11->PSSetSamplers(0, 1, m_samplerState.GetAddressOf());
		m_deviceContextD3D11->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		// cube object
		{
			SLightParametersCB  lightParams;
			lightParams = m_pointLight.GetLightParams();
			lightParams.m_directionalLightDir = m_directionLight.GetDirection();

			m_cube.UpdateCube(m_deviceContextD3D11.Get(), m_camera.GetViewMatrix(), m_camera.GetProjectionMatrix(), lightParams);
		}

		// light
		{
			m_pointLight.UpdateLight(m_deviceContextD3D11.Get(), m_camera.GetViewMatrix(), m_camera.GetProjectionMatrix());
		}
	}

	if (m_bloom.IsBloomOn())
	{
		m_bloom.Update(m_deviceContextD3D11.Get(), m_backBufferRTV.GetAddressOf(), m_depthStencilStateNoDepth.Get());
	}
}

void CGraphics::Render()
{
	// draw GUI of ImGUI
	// Start the Dear ImGui frame
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	m_cube.ShowGUI();
	m_pointLight.ShowGUI();
	m_directionLight.ShowGUI();
	m_camera.ShowCameraGUI();
	m_bloom.ShowGUI();

	//render Draw Data
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

	HRESULT hr;
	SUCCEEDED(hr = m_swapChainDXGI->Present(g_vSync, 0u));
}

