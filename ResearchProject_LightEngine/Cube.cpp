#include "Cube.h"
#include "Data.h"

// texture parser
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "ImGui/imgui.h"

CCube::CCube()
{
	// texture
	m_cubeTextureSRV = nullptr;
	m_cubeTexture2D = nullptr;

	m_isTextured = false;

	// matreial
	m_materialConstants.m_materialColor = { 0.5f, 0.5f, 0.5f };

	m_object.SetPosition(0.0f, 0.0f, 2.0f);
}

CCube::~CCube()
{
}

CObject& CCube::GetCubeObject()
{
	return m_object;
}

void CCube::CreateShaders(ID3D11Device* device)
{
	D3D11_INPUT_ELEMENT_DESC inputElementDesc[] =
	{
		{ "Position",0,DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0 },
		{ "TexCoord",0,DXGI_FORMAT_R32G32_FLOAT,0,D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA,0 },
		{ "Normal",0,DXGI_FORMAT_R32G32B32_FLOAT,0,D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA,0 },
	};

	// create vertex shader and input layout
	m_renderObjectsVS.CreateVertexShaderAndInput(device, inputElementDesc, ARRAYSIZE(inputElementDesc), L"CompiledShaders\\TextureNormalVS.cso");

	// create the pixel shader
	m_renderObjectsPS.CreatePixelShader(device, L"CompiledShaders\\ColorLightPS.cso");
	m_renderObjectsPSTexture.CreatePixelShader(device, L"CompiledShaders\\TextureLightPS.cso");
}

void CCube::InitializeCube(ID3D11Device* device)
{

	// create constant buffer Pixel
	HRESULT hr = m_materialCB.CreateConstantBuffer(device);
	if (FAILED(hr))
	{
		MessageBoxW(NULL, L"ERROR : Failed to create material constant buffer", L"ERROR", MB_ICONERROR);
		exit(-1);
	}

	hr = m_lightCB.CreateConstantBuffer(device);
	if (FAILED(hr))
	{
		MessageBoxW(NULL, L"ERROR : Failed to create light constant buffer", L"ERROR", MB_ICONERROR);
		exit(-1);
	}

	CreateShaders(device);

	// create texture
	SetTextureFromFile(device, g_sampleTexturePath);

	m_object.CreateBuffersVIC(device, "Models\\cube1.obj");
}

void CCube::SetTextureFromFile(ID3D11Device* device, const std::string& textureFilePath)
{
	// Load Image
	int texWidth;
	int texHeight;
	int texNumChannels;
	int texForceNumChannels = 4;

	unsigned char* testTextureBytes = stbi_load(textureFilePath.c_str(), &texWidth, &texHeight, &texNumChannels, texForceNumChannels);
	assert(testTextureBytes);
	int texBytesPerRow = 4 * texWidth;

	// Create Texture
	D3D11_TEXTURE2D_DESC textureDesc = {};
	textureDesc.Width = texWidth;
	textureDesc.Height = texHeight;
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.Usage = D3D11_USAGE_IMMUTABLE;
	textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = 0;

	// create sub resource data
	D3D11_SUBRESOURCE_DATA textureSubresourceData = {};
	textureSubresourceData.pSysMem = testTextureBytes;
	textureSubresourceData.SysMemPitch = texBytesPerRow;

	HRESULT hr = device->CreateTexture2D(&textureDesc, &textureSubresourceData, &m_cubeTexture2D);

	if (FAILED(hr))
	{
		MessageBoxW(NULL, L"ERROR : Failed to create object texture", L"ERROR", MB_ICONERROR);
		exit(-1);
	}

	// create the resource view on the texture
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Format = textureDesc.Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.MipLevels = 1;
	hr = device->CreateShaderResourceView(m_cubeTexture2D.Get(), &srvDesc, &m_cubeTextureSRV);

	if (FAILED(hr))
	{
		MessageBoxW(NULL, L"ERROR : Failed to create shader resource for object texture", L"ERROR", MB_ICONERROR);
		exit(-1);
	}
}

void CCube::UpdateCube(ID3D11DeviceContext* deviceContext, const DirectX::XMMATRIX& viewMatrix, const DirectX::XMMATRIX& projectionMatrix, SLightParametersCB& lightParams)
{

	// scaled because it should look like a "POINT" light right?
	deviceContext->VSSetShader(m_renderObjectsVS.GetVertexShader(), nullptr, 0u); // bind vertex shader

	if (!m_isTextured)
	{
		deviceContext->PSSetShader(m_renderObjectsPS.GetPixelShader(), nullptr, 0u);// bind pixel shader
	}
	else
	{
		deviceContext->PSSetShader(m_renderObjectsPSTexture.GetPixelShader(), nullptr, 0u);// bind pixel shader
	}
	deviceContext->IASetInputLayout(m_renderObjectsVS.GetInputLayout()); // bind vertex layout in Input layout 

	m_materialCB.UpdateAndMapData(deviceContext, m_materialConstants);
	deviceContext->PSSetConstantBuffers(0, 1, m_materialCB.GetAddressOfCB());

	m_lightCB.UpdateAndMapData(deviceContext, lightParams); // from light
	deviceContext->PSSetConstantBuffers(1, 1, m_lightCB.GetAddressOfCB());

	// if texture is set from file
	if (m_cubeTextureSRV != nullptr)
	{
		deviceContext->PSSetShaderResources(0, 1, m_cubeTextureSRV.GetAddressOf()); //Set Texture
	}

	m_object.Update(deviceContext, viewMatrix, projectionMatrix);
}

void CCube::UpdateWorldMatrix()
{
	m_object.UpdateWorldMatrix();
}

void CCube::ShowGUI()
{
	ImGui::Begin("Object");
	ImGui::Text("World Transform");

	ImGui::DragFloat3("Rotation about local X/Y/Z", m_object.GetRotationAddress(), 0.01f, -10.0f, 10.0f);
	ImGui::DragFloat3("Translation aboutX/Y/Z", m_object.GetPositionAddress(), 0.01f, -20.0f, 20.0f);

	ImGui::Text("Material");
	ImGui::DragFloat3("Color", &m_materialConstants.m_materialColor.x, 0.01f, 0.0f, 1.0f);

	ImGui::Checkbox("Turn sample texture on", &m_isTextured);

	UpdateWorldMatrix();
	ImGui::End();
}

