#pragma once
#include <DirectXMath.h>
#include "Shaders.h"
#include "Object.h"

class CCube
{
private:
	// object
	CObject m_object;
	bool m_isTextured;

	// shaders for cube
	CVertexShader m_renderObjectsVS;
	CPixelShader m_renderObjectsPS;
	CPixelShader m_renderObjectsPSTexture;

	// for texture if any
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_cubeTextureSRV;// texture for image
	Microsoft::WRL::ComPtr<ID3D11Texture2D> m_cubeTexture2D;

	// constant buffer data
	CConstantBuffer<SMaterialParametersCB> m_materialCB;

	SLightParametersCB m_lightParameters;
	CConstantBuffer<SLightParametersCB> m_lightCB;

public:
	// data
	SMaterialParametersCB m_materialConstants;

	// constructor
	CCube();

	// destructor
	~CCube();

	// getter
	CObject& GetCubeObject();

	// setter
	void CreateShaders(ID3D11Device* device);

	// functions
	void SetTextureFromFile(ID3D11Device* device, const std::string& textureFilePath);
	void InitializeCube(ID3D11Device* device);
	void UpdateCube(ID3D11DeviceContext* deviceContext, const DirectX::XMMATRIX& viewMatrix, const DirectX::XMMATRIX& projectionMatrix, SLightParametersCB& lightParams);
	void UpdateWorldMatrix();
	void ShowGUI();
};

