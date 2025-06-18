#pragma once
#include <DirectXMath.h>
#include "Shaders.h"
#include "Object.h"

class CPointLight
{
private:
	// Point Light
	CObject m_lightObject;

	// shaders for cube
	CVertexShader m_lightObjectVS;
	CPixelShader m_lightObjectPS;


	// constant buffer data
	CConstantBuffer<SLightParametersCB> m_lightCB;

	// functions
	void CreateShaders(ID3D11Device* device);

public:
	// data
	SLightParametersCB m_lightParameters;

	// constructor
	CPointLight();

	// destructor
	~CPointLight();

	// getter
	CObject& GetLightObject();
	const SLightParametersCB& GetLightParams();

	void InitializeLight(ID3D11Device* device);
	void UpdateLight(ID3D11DeviceContext* deviceContext, const DirectX::XMMATRIX& viewMatrix, const DirectX::XMMATRIX& projectionMatrix);
	void UpdateLightWorldMatrix();
	void ShowGUI();
};

