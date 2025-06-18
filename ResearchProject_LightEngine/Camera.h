#pragma once
#include <DirectXMath.h>
class CCamera
{
private:
	// variables
	DirectX::XMVECTOR m_cameraPosition;
	DirectX::XMVECTOR m_cameraRotation;

	DirectX::XMMATRIX m_viewMatrix;
	DirectX::XMMATRIX m_projectionMatrix;

	DirectX::XMFLOAT3 m_camPos;
	DirectX::XMFLOAT3 m_camRot;


public:
	const DirectX::XMVECTOR m_defaultForwardDirection = DirectX::XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
	const DirectX::XMVECTOR m_defaultUpDirection = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	const DirectX::XMVECTOR m_defaultLeftDirection = DirectX::XMVectorSet(-1.0f, 0.0f, 0.0f, 0.0f);

	DirectX::XMVECTOR m_currentForwardDir;
	DirectX::XMVECTOR m_currentLeftDir;

	void UpdateDirections();

public:

	// constructor
	CCamera();

	// destructor
	~CCamera();


	// getters
	const DirectX::XMVECTOR& GetCameraPosition() const;
	const DirectX::XMVECTOR& GetCameraRotation() const;
	const DirectX::XMMATRIX& GetViewMatrix() const;
	const DirectX::XMMATRIX& GetProjectionMatrix() const;
	const DirectX::XMVECTOR& GetCurrentForward();
	const DirectX::XMVECTOR& GetCurrentLeft();


	// setters
	void SetCameraPosition(float x, float y, float z);
	void SetCameraRotation(float pitch, float yaw, float roll);

	void OffsetCameraPosition(float x, float y, float z);
	void OffsetCameraRotation(float pitch, float yaw, float roll);

	void SetProjectionMatrix(float fovInDegree, float aspectRatio, float nearZ, float farZ);
	void SetViewMatrix();

	void ShowCameraGUI();
};
