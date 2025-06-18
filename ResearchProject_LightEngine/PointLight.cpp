#include "PointLight.h"

#include "ImGui/imgui.h"

CPointLight::CPointLight()
{
	// dummy arguments for light shader
	m_lightParameters.m_ambientLightColor = { 1.0f, 1.0f, 1.0f };
	m_lightParameters.m_ambientLightStrength = 1.0f;
	m_lightParameters.m_pointLightColor = { 1.0f, 1.0f, 1.0f };
	m_lightParameters.m_pointLightStrength = 1.0f;
	m_lightParameters.m_pointLightAttenuation = { 1.0f, 0.045f, 0.0007f };
	m_lightParameters.m_pointLightPosition = { 0.0f, 0.0f, 0.0f };
	m_lightParameters.m_pointLightRadius = 100.0f;
	m_lightParameters.m_directionalLightDir = { 0.0f, 0.0f, -1.0f };
}

CPointLight::~CPointLight()
{
}

CObject& CPointLight::GetLightObject()
{
	return m_lightObject;
}

const SLightParametersCB& CPointLight::GetLightParams()
{
	return m_lightParameters;
}


void CPointLight::InitializeLight(ID3D11Device* device)
{
	// create constant buffer Pixel
	HRESULT hr = m_lightCB.CreateConstantBuffer(device);
	if (FAILED(hr))
	{
		MessageBoxW(NULL, L"ERROR : Failed to create light constant buffer", L"ERROR", MB_ICONERROR);
		exit(-1);
	}

	CreateShaders(device);
	m_lightObject.CreateBuffersVIC(device, "Models\\sphere.obj");
}

void CPointLight::CreateShaders(ID3D11Device* device)
{
	D3D11_INPUT_ELEMENT_DESC inputElementDesc[] =
	{
		{ "Position",0,DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0 },
		{ "TexCoord",0,DXGI_FORMAT_R32G32_FLOAT,0,D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA,0 },
		{ "Normal",0,DXGI_FORMAT_R32G32B32_FLOAT,0,D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA,0 },
	};

	// create vertex shader and input layout
	m_lightObjectVS.CreateVertexShaderAndInput(device, inputElementDesc, ARRAYSIZE(inputElementDesc), L"CompiledShaders\\TextureNormalVS.cso");

	// create the pixel shader
	m_lightObjectPS.CreatePixelShader(device, L"CompiledShaders\\PointLightPS.cso");
}


void CPointLight::UpdateLight(ID3D11DeviceContext* deviceContext, const DirectX::XMMATRIX& viewMatrix, const DirectX::XMMATRIX& projectionMatrix)
{
	deviceContext->VSSetShader(m_lightObjectVS.GetVertexShader(), nullptr, 0u); // bind vertex shader
	deviceContext->PSSetShader(m_lightObjectPS.GetPixelShader(), nullptr, 0u);// bind pixel shader
	deviceContext->IASetInputLayout(m_lightObjectVS.GetInputLayout()); // bind vertex layout in Input layout 

	m_lightCB.UpdateAndMapData(deviceContext, m_lightParameters);
	deviceContext->PSSetConstantBuffers(0, 1, m_lightCB.GetAddressOfCB());

	m_lightObject.Update(deviceContext, viewMatrix, projectionMatrix);
}


void CPointLight::UpdateLightWorldMatrix()
{
	// scaled because it should look like a "POINT" light right?
	// scaled sphere to 0.05
	m_lightObject.UpdateWorldMatrix(DirectX::XMMatrixScaling(0.05f, 0.05f, 0.05f));
}

void CPointLight::ShowGUI()
{
	ImGui::Begin("Light");
	ImGui::Text("Point Light");
	ImGui::Text("Position");
	ImGui::DragFloat3("Position X/Y/Z", m_lightObject.GetPositionAddress(), 0.01f, -20.0f, 20.0f);
	m_lightParameters.m_pointLightPosition = m_lightObject.GetPosition();

	ImGui::Text("Light Parameters");
	ImGui::DragFloat3("Light Color", &m_lightParameters.m_pointLightColor.x, 0.01f, 0.0f, 1.0f);
	ImGui::DragFloat("Light Strength", &m_lightParameters.m_pointLightStrength, 0.01f, 0.0f, 10.0f);
	ImGui::DragFloat3("Light Attenuation", &m_lightParameters.m_pointLightAttenuation.x, 0.001f, 0.00001f, 1.0f);
	ImGui::DragFloat("Light Range/Radius", &m_lightParameters.m_pointLightRadius, 1.0f, 1.0f, 1000.0f);

	ImGui::Text("Ambient light");
	ImGui::DragFloat3("Color", &m_lightParameters.m_ambientLightColor.x, 0.01f, 0.0f, 1.0f);
	ImGui::DragFloat("Strength", &m_lightParameters.m_ambientLightStrength, 0.01f, 0.0f, 1.0f);

	UpdateLightWorldMatrix();
	ImGui::End();
}
