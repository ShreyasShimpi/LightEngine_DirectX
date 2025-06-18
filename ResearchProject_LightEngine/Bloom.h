#pragma once
#include <d3d11.h>
#include <wrl.h>

#include "Shaders.h"
#include "ConstantBuffer.h"
#include "VertexBuffer.h"
#include "ConstantBufferTypes.h"
#include "VertexBufferTypes.h"
#include <array>

// data
constexpr unsigned short g_backRenderTargets = 3;
constexpr float g_sigmaGK = 10.0f;

constexpr ID3D11RenderTargetView* g_nullRenderTarget = nullptr;
constexpr ID3D11ShaderResourceView* g_nullShaderResourceView = nullptr;
constexpr ID3D11UnorderedAccessView* g_nullUnorderedAccessView = nullptr;
constexpr UINT g_nullOffset = -1;

// render targets : used for dispatching to compute shaders
struct SRenderTarget
{
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_renderTargetView;
	Microsoft::WRL::ComPtr<ID3D11Texture2D> m_renderTargetTexture;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_shaderResourceView;
	Microsoft::WRL::ComPtr<ID3D11UnorderedAccessView> m_unorderedAccessView;
};

class CBloom
{
private:
	bool m_isBloomEffectOn;
	std::array<SRenderTarget, g_backRenderTargets> m_renderTargets;

	// compute shader
	// parameters for compute shaders
	CVertexShader m_quadrantCompositeShaderVS;
	CPixelShader m_quadrantCompositeShaderPS;
	CComputeShader m_downSamplerShader;
	CComputeShader m_blurShader;

	CVertexBuffer<SVertexDataTexture> m_screenQuadrant;
	CConstantBuffer<SThresholdParameter> m_thresholdConstantBuffer;
	CConstantBuffer<SCompositeParameter> m_compositeConstantBuffer;
	CConstantBuffer<SBlurringParameters> m_blurConstantBuffer;

	SBlurringParameters m_blurParamData;
	SCompositeParameter m_compositeParameter;
	SThresholdParameter m_thresholdParameter;

	// functions
	void CalculateGaussianKernels();
	void CreateRenderTargets(ID3D11Device* device);
	void CreateShaders(ID3D11Device* device);

public:
	// constructor
	CBloom();

	// destructor
	~CBloom();

	// getter
	ID3D11RenderTargetView** GetFirstRTVAddress();
	ID3D11RenderTargetView* GetFirstRTV();
	const bool& IsBloomOn();

	void InitializeBloom(ID3D11Device* device);
	void Update(ID3D11DeviceContext* deviceContext, ID3D11RenderTargetView* const* backBuffer, ID3D11DepthStencilState* noDepthStencilState);
	void ShowGUI();
};

