#pragma once

// internal includes
#include "Camera.h"
#include "Cube.h"
#include "Bloom.h"
#include "PointLight.h"
#include "DirectionalLight.h"

// external includes
#include <d3d11.h>
#include <wrl.h>
#include <array>

class CGraphics
{
private:

	// COM objects
	Microsoft::WRL::ComPtr<ID3D11Device> m_deviceD3D11;
	Microsoft::WRL::ComPtr<IDXGISwapChain> m_swapChainDXGI;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_deviceContextD3D11;

	// sampler
	Microsoft::WRL::ComPtr<ID3D11SamplerState> m_samplerState;

	// rasterizer
	Microsoft::WRL::ComPtr<ID3D11RasterizerState> m_rasterizerState;

	// back buffer
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_backBufferRTV;

	// back render targets
	std::array<SRenderTarget, g_backRenderTargets> m_renderTargets;


	// depth stencil
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> m_depthStencilView;
	Microsoft::WRL::ComPtr<ID3D11Texture2D> m_depthStencil;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> m_depthStencilState;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> m_depthStencilStateNoDepth;

	// shaders and object
	CCube m_cube;

	// camera
	CCamera m_camera;

	// light
	CPointLight m_pointLight;
	CDirectionLight m_directionLight;

	// bloom
	CBloom m_bloom;

	// private functions
	void CreateSwapChain(HWND hWnd);
	void CreateAllBuffers();
	void CreateSampler();
	void CreateRasterizer();
	void CreateDepthStencil();
	void CreateViewPort();
	void SetupImGUI(HWND hWnd);

public:
	// constructor
	CGraphics();
	CGraphics(const CGraphics&) = delete;
	CGraphics& operator = (const CGraphics&) = delete;

	// destructor
	~CGraphics() = default;

	// getter
	CCamera& GetCamera();

	// functions
	void InitializeGraphics(HWND hWnd);
	void ClearFrame(float red, float green, float blue) noexcept;
	void Update();
	void Render();
};

