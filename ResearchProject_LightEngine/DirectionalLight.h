#pragma once
#include <DirectXMath.h>

class CDirectionLight
{
private:
	DirectX::XMFLOAT3 m_directionLightDir;

public:
	// construction
	CDirectionLight();

	// getter
	const DirectX::XMFLOAT3& GetDirection();
	float* GetDirectionAddress();

	// setter
	void SetDirection(DirectX::XMFLOAT3 directionLightDir);

	void ShowGUI();
};