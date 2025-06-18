#include "DirectionalLight.h"

#include "ImGui/imgui.h"

CDirectionLight::CDirectionLight()
{
	m_directionLightDir = { 0.0f, 0.0f, -1.0f };
}

const DirectX::XMFLOAT3& CDirectionLight::GetDirection()
{
	return m_directionLightDir;
};
float* CDirectionLight::GetDirectionAddress() // for imgui
{
	return &m_directionLightDir.x;
}

void CDirectionLight::SetDirection(DirectX::XMFLOAT3 directionLightDir)
{
	m_directionLightDir = directionLightDir;
}

void CDirectionLight::CDirectionLight::ShowGUI()
{
	ImGui::Begin("Directional Light");
	ImGui::DragFloat3("Direction", &m_directionLightDir.x, 0.01f, -1.0f, 1.0f);
	ImGui::End();
}
