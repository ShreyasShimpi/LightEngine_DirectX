#include "Bloom.h"
#include "Shaders.h"
#include "Data.h"
#include <cmath>
#include "ImGui/imgui.h"

// screen data
// vertices to map window
std::array<SVertexDataTexture, 6> g_screenQuadrant =
{
	SVertexDataTexture{ -1.f,  1.f, 0.f, 0.f, 0.f },
	SVertexDataTexture{  1.f, -1.f, 0.f, 1.f, 1.f },
	SVertexDataTexture{ -1.f, -1.f, 0.f, 0.f, 1.f },
	SVertexDataTexture{ -1.f,  1.f, 0.f, 0.f, 0.f },
	SVertexDataTexture{  1.f,  1.f, 0.f, 1.f, 0.f },
	SVertexDataTexture{  1.f, -1.f, 0.f, 1.f, 1.f }
};

void CBloom::CalculateGaussianKernels()
{
	// getting Gaussian kernel values
	float sumOfKernels = m_blurParamData.m_coefficients[0]; // center
	for (UINT i = 0; i <= GAUSSIAN_RADIUS; ++i)
	{
		// formula for Gaussian kernel values
		m_blurParamData.m_coefficients[i] = (1.f / g_sigmaGK) * std::expf(-1.0f * static_cast<float>(i * i) / (2 * g_sigmaGK * g_sigmaGK));
		sumOfKernels += 2 * m_blurParamData.m_coefficients[i];
	}

	sumOfKernels -= m_blurParamData.m_coefficients[0];

	// normalize the values of kernel with sum
	float normalizeFactor = 1.f / sumOfKernels;
	for (UINT i = 0; i <= GAUSSIAN_RADIUS; ++i)
	{
		m_blurParamData.m_coefficients[i] *= normalizeFactor;
	}
}

CBloom::CBloom()
{
	// initialize constant buffer parameters
	m_blurParamData.m_radius = GAUSSIAN_RADIUS;
	m_blurParamData.m_direction = 0;
	CalculateGaussianKernels();

	m_compositeParameter = { 0.4f };
	m_thresholdParameter = { 0.5f };

	m_isBloomEffectOn = false;
}

CBloom::~CBloom()
{
}

ID3D11RenderTargetView** CBloom::GetFirstRTVAddress()
{
	return m_renderTargets[0].m_renderTargetView.GetAddressOf();
}

ID3D11RenderTargetView* CBloom::GetFirstRTV()
{
	return m_renderTargets[0].m_renderTargetView.Get();
}

const bool& CBloom::IsBloomOn()
{
	return m_isBloomEffectOn;
}

void CBloom::InitializeBloom(ID3D11Device* device)
{
	// initalize screen quadrant
	HRESULT hr = m_screenQuadrant.CreateVertexBuffer(device, g_screenQuadrant.data(), g_screenQuadrant.size());
	if (FAILED(hr))
	{
		MessageBoxW(NULL, L"ERROR : Failed to create screen quadrant buffer", L"ERROR", MB_ICONERROR);
		exit(-1);
	}

	// intialize constant buffers
	hr = m_thresholdConstantBuffer.CreateConstantBuffer(device);
	if (FAILED(hr))
	{
		MessageBoxW(NULL, L"ERROR : Failed to create threshold buffer", L"ERROR", MB_ICONERROR);
		exit(-1);
	}

	hr = m_compositeConstantBuffer.CreateConstantBuffer(device);
	if (FAILED(hr))
	{
		MessageBoxW(NULL, L"ERROR : Failed to create composite buffer", L"ERROR", MB_ICONERROR);
		exit(-1);
	}

	hr = m_blurConstantBuffer.CreateConstantBuffer(device);
	if (FAILED(hr))
	{
		MessageBoxW(NULL, L"ERROR : Failed to create blur buffer", L"ERROR", MB_ICONERROR);
		exit(-1);
	}

	// create the blur parameters
	m_blurParamData.m_radius = GAUSSIAN_RADIUS;
	m_blurParamData.m_direction = 0;

	m_compositeParameter = { 0.4f };
	m_thresholdParameter = { 0.5f };

	CreateRenderTargets(device);
	CreateShaders(device);
}

void CBloom::CreateRenderTargets(ID3D11Device* device)
{
	HRESULT hr;
	// back render targets

	const UINT targetWidths[g_backRenderTargets] = { WIDTH,  WIDTH / 2, WIDTH / 2 };
	const UINT targetHeights[g_backRenderTargets] = { HEIGHT, HEIGHT / 2, HEIGHT / 2 };

	for (UINT i = 0; i < g_backRenderTargets; ++i)
	{
		SRenderTarget& currRenderTarget = m_renderTargets[i];


		// Create texture
		D3D11_TEXTURE2D_DESC textureDesc = {};
		textureDesc.Width = targetWidths[i];
		textureDesc.Height = targetHeights[i];
		textureDesc.MipLevels = 1;
		textureDesc.ArraySize = 1;
		textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		textureDesc.SampleDesc.Count = 1;
		textureDesc.Usage = D3D11_USAGE_DEFAULT;
		textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS;
		textureDesc.CPUAccessFlags = 0;
		textureDesc.MiscFlags = 0;

		hr = device->CreateTexture2D(
			&textureDesc,
			NULL,
			&currRenderTarget.m_renderTargetTexture);
		if (FAILED(hr))
		{
			MessageBoxW(NULL, L"ERROR : Failed to create render target texture", L"ERROR", MB_ICONERROR);
			exit(-1);
		}

		// Create render target view
		D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc = {};
		renderTargetViewDesc.Format = textureDesc.Format;
		renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
		renderTargetViewDesc.Texture2D.MipSlice = 0;

		hr = device->CreateRenderTargetView(
			currRenderTarget.m_renderTargetTexture.Get(),
			&renderTargetViewDesc,
			&currRenderTarget.m_renderTargetView);

		if (FAILED(hr))
		{
			MessageBoxW(NULL, L"ERROR : Failed to create render target view", L"ERROR", MB_ICONERROR);
			exit(-1);

		}

		// create shader resource views
		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
		srvDesc.Format = textureDesc.Format;
		srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Texture2D.MostDetailedMip = 0;
		srvDesc.Texture2D.MipLevels = 1;

		hr = device->CreateShaderResourceView(
			currRenderTarget.m_renderTargetTexture.Get(),
			&srvDesc,
			&currRenderTarget.m_shaderResourceView);

		if (FAILED(hr))
		{
			MessageBoxW(NULL, L"ERROR : Failed to create render target shader resource view", L"ERROR", MB_ICONERROR);
			exit(-1);
		}

		// create unordered access views
		D3D11_UNORDERED_ACCESS_VIEW_DESC uavDesc = {};
		uavDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		uavDesc.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE2D;
		uavDesc.Texture2D.MipSlice = 0;

		hr = device->CreateUnorderedAccessView(
			currRenderTarget.m_renderTargetTexture.Get(),
			&uavDesc,
			&currRenderTarget.m_unorderedAccessView);

		if (FAILED(hr))
		{
			MessageBoxW(NULL, L"ERROR : Failed to create render target unordered access view", L"ERROR", MB_ICONERROR);
			exit(-1);
		}
	}

}

void CBloom::CreateShaders(ID3D11Device* device)
{
	// for screen quadrant texture
	{
		D3D11_INPUT_ELEMENT_DESC inputElementDesc1[] =
		{
			{ "Position",0,DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0 },
			{ "TexCoord",0,DXGI_FORMAT_R32G32_FLOAT,0,D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA,0 },
		};
		// create vertex shader and input layout
		m_quadrantCompositeShaderVS.CreateVertexShaderAndInput(device, inputElementDesc1, ARRAYSIZE(inputElementDesc1), L"CompiledShaders\\TextureScrQuadVS.cso");

		// create the pixel shader
		m_quadrantCompositeShaderPS.CreatePixelShader(device, L"CompiledShaders\\TextureScrQuadPS.cso");
	}

	// compute shader Downsample
	{
		m_downSamplerShader.CreateComputeShader(device, L"CompiledShaders\\DownSampleCS.cso");
	}

	// compute shader Blur
	{
		m_blurShader.CreateComputeShader(device, L"CompiledShaders\\BlurCS.cso");
	}
}

void CBloom::Update(ID3D11DeviceContext* deviceContext, ID3D11RenderTargetView* const* backBuffer, ID3D11DepthStencilState* noDepthStencilState)
{
	// unbind render target for next operation to perform 
	deviceContext->OMSetRenderTargets(1, &g_nullRenderTarget, nullptr);

	// bind depth stencil with NO DEPTH
	deviceContext->OMSetDepthStencilState(noDepthStencilState, 0u);

	// operation 1  :  downsampling
	{
		m_thresholdConstantBuffer.UpdateAndMapData(deviceContext, m_thresholdParameter);

		deviceContext->CSSetShader(m_downSamplerShader.GetComputeShader(), 0, 0);
		deviceContext->CSSetShaderResources(0, 1, m_renderTargets[0].m_shaderResourceView.GetAddressOf());
		deviceContext->CSSetUnorderedAccessViews(0, 1, m_renderTargets[1].m_unorderedAccessView.GetAddressOf(), &g_nullOffset);
		deviceContext->CSSetConstantBuffers(0, 1, m_thresholdConstantBuffer.GetAddressOfCB());

		deviceContext->Dispatch(WIDTH / 16, HEIGHT / 16, 1);

		// unbind UAV and SRVs
		deviceContext->CSSetShaderResources(0, 1, &g_nullShaderResourceView);
		deviceContext->CSSetUnorderedAccessViews(0, 1, &g_nullUnorderedAccessView, &g_nullOffset);
	}

	// operation 2 : gaussian blur the render targets with half resolution
	{
		deviceContext->CSSetShader(m_blurShader.GetComputeShader(), 0, 0);
		std::array<Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>, 2> computeShadersSRV =
		{
			m_renderTargets[1].m_shaderResourceView,
			m_renderTargets[2].m_shaderResourceView
		};

		std::array<Microsoft::WRL::ComPtr<ID3D11UnorderedAccessView>, 2> computeShadersUAV =
		{
			m_renderTargets[2].m_unorderedAccessView,
			m_renderTargets[1].m_unorderedAccessView
		};

		for (UINT dir = 0; dir < 2; dir++) // dir : ( 0 = horizontal, 1 = vertical )
		{
			m_blurParamData.m_direction = dir;
			m_blurConstantBuffer.UpdateAndMapData(deviceContext, m_blurParamData);
			deviceContext->CSSetShaderResources(0, 1, computeShadersSRV[dir].GetAddressOf());
			deviceContext->CSSetUnorderedAccessViews(0, 1, computeShadersUAV[dir].GetAddressOf(), &g_nullOffset);
			deviceContext->CSSetConstantBuffers(0, 1, m_blurConstantBuffer.GetAddressOfCB());

			deviceContext->Dispatch(WIDTH / 16, HEIGHT / 16, 1);

			// unbind UAV and SRVs
			deviceContext->CSSetShaderResources(0, 1, &g_nullShaderResourceView);
			deviceContext->CSSetUnorderedAccessViews(0, 1, &g_nullUnorderedAccessView, &g_nullOffset);
		}
	}

	// operation 3 : merge the gaussian blurred half resolution image
	//  with original image(texture obtained from render target)
	UINT offset1 = 0u;
	{
		// bind render target from back buffer
		deviceContext->OMSetRenderTargets(1u, backBuffer, nullptr);

		deviceContext->VSSetShader(m_quadrantCompositeShaderVS.GetVertexShader(), nullptr, 0u); // bind vertex shader
		deviceContext->PSSetShader(m_quadrantCompositeShaderPS.GetPixelShader(), nullptr, 0u);// bind pixel shader
		deviceContext->IASetInputLayout(m_quadrantCompositeShaderVS.GetInputLayout()); // bind vertex layout in Input layout 

		// screen quadrant does not have update function so doing it raw here
		deviceContext->IASetVertexBuffers(0, 1, m_screenQuadrant.GetAddressOfVB(), m_screenQuadrant.GetStridePointer(), &offset1);

		deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		std::array<ID3D11ShaderResourceView*, 2> compositeSRVs =
		{
			m_renderTargets[0].m_shaderResourceView.Get(),
			m_renderTargets[1].m_shaderResourceView.Get()
		};
		deviceContext->PSSetShaderResources(0, 2, &compositeSRVs[0]);

		m_compositeConstantBuffer.UpdateAndMapData(deviceContext, m_compositeParameter);
		deviceContext->PSSetConstantBuffers(0, 1, m_compositeConstantBuffer.GetAddressOfCB());

		deviceContext->Draw(g_screenQuadrant.size(), 0); // not indexed

		//unbind SRVs
		deviceContext->PSSetShaderResources(0, 1, &g_nullShaderResourceView);
	}
}

void CBloom::ShowGUI()
{
	ImGui::Begin("Bloom");
	ImGui::Checkbox("Turn Bloom On", &m_isBloomEffectOn);
	ImGui::DragFloat("Composite Parameter", &m_compositeParameter.m_coefficient, 0.01f, 0.0f, 1.0f); // to change the parameter
	ImGui::DragFloat("Threshold Parameter", &m_thresholdParameter.m_threshold, 0.01f, 0.0f, 1.0f); // to change the parameter
	ImGui::End();
}
