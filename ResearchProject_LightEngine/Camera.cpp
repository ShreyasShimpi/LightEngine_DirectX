#include "Camera.h"
#include "Data.h"

#include "ImGui/imgui.h"

CCamera::CCamera()
{
	m_camPos = DirectX::XMFLOAT3(0.0f, 0.0f, g_defaultCameraPositionZ);
	m_cameraPosition = DirectX::XMLoadFloat3(&m_camPos);

	m_camRot = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_cameraRotation = DirectX::XMLoadFloat3(&m_camRot);

	// update view matrix
	SetViewMatrix();

	// default initialization if user dont set it mnaully
	SetProjectionMatrix(g_defaultFOV, float(WIDTH) / float(HEIGHT), g_defaultNearZ, g_defaultFarZ);
}

CCamera::~CCamera()
{
}

void CCamera::UpdateDirections()
{
	DirectX::XMMATRIX rotationMatrix = DirectX::XMMatrixRotationRollPitchYaw(m_camRot.x, m_camRot.y, 0.0f);

	m_currentForwardDir = DirectX::XMVector3TransformCoord(m_defaultForwardDirection, rotationMatrix);
	m_currentLeftDir = DirectX::XMVector3TransformCoord(m_defaultLeftDirection, rotationMatrix);
}

void CCamera::SetCameraPosition(float x, float y, float z)
{
	m_camPos = DirectX::XMFLOAT3(x, y, z);
	m_cameraPosition = DirectX::XMLoadFloat3(&m_camPos);

	SetViewMatrix();
}

void CCamera::SetCameraRotation(float pitch, float yaw, float roll)
{
	m_camRot = DirectX::XMFLOAT3(pitch, yaw, roll);
	m_cameraRotation = DirectX::XMLoadFloat3(&m_camRot);

	SetViewMatrix();
}

void CCamera::OffsetCameraPosition(float x, float y, float z)
{
	m_camPos.x += x;
	m_camPos.y += y;
	m_camPos.z += z;

	m_cameraPosition = DirectX::XMLoadFloat3(&m_camPos);
	SetViewMatrix();
}

void CCamera::OffsetCameraRotation(float pitch, float yaw, float roll)
{
	m_camRot.x += pitch;
	m_camRot.y += yaw;
	m_camRot.z += roll;

	m_cameraRotation = XMLoadFloat3(&m_camRot);
	SetViewMatrix();
}


void CCamera::SetProjectionMatrix(float fovInDegree, float aspectRatio, float nearZ, float farZ)
{
	float fovInRadians = (fovInDegree / 360.0f) * DirectX::XM_2PI;
	m_projectionMatrix = DirectX::XMMatrixPerspectiveFovLH(fovInRadians, aspectRatio, nearZ, farZ);
}


void CCamera::SetViewMatrix()
{
	//  camera rotation matrix
	DirectX::XMFLOAT3 rotation;
	DirectX::XMStoreFloat3(&rotation, m_cameraRotation); // store in float 3

	//  camera position matrix
	DirectX::XMFLOAT3 position;
	DirectX::XMStoreFloat3(&position, m_cameraPosition); // store in float 3

	// get rotation matrix from rotation vector
	DirectX::XMMATRIX camRotationMatrix =
		DirectX::XMMatrixRotationRollPitchYaw(
			rotation.x,
			rotation.y,
			rotation.z
		);

	// Get the look at position for camera
	DirectX::XMVECTOR cameraLookAtPosition = DirectX::XMVector3TransformCoord(
		m_defaultForwardDirection,
		camRotationMatrix); // this will get the direction to look

	// get the local up direction of the camera
	DirectX::XMVECTOR localUpDirection = DirectX::XMVector3TransformCoord(
		m_defaultUpDirection,
		camRotationMatrix);

	DirectX::XMFLOAT3 cameraLookAtPositionF;
	DirectX::XMStoreFloat3(&cameraLookAtPositionF, cameraLookAtPosition);

	cameraLookAtPositionF.x += position.x; // and we move the vector to current camera position
	cameraLookAtPositionF.y += position.y;
	cameraLookAtPositionF.z += position.z;

	// load the updated postion
	cameraLookAtPosition = DirectX::XMLoadFloat3(&cameraLookAtPositionF);

	// update view matrix
	m_viewMatrix = DirectX::XMMatrixLookAtLH(m_cameraPosition, cameraLookAtPosition, localUpDirection);

	UpdateDirections();
}

void CCamera::ShowCameraGUI()
{
	ImGui::Begin("Camera");
	ImGui::DragFloat3("Translation X/Y/Z", &m_camPos.x, 0.01f, -20.0f, 20.0f);
	ImGui::DragFloat3("Rotation about local X/Y/Z", &m_camRot.x, 0.01f, -10.0f, 10.0f);

	SetCameraPosition(m_camPos.x, m_camPos.y, m_camPos.z);
	SetCameraRotation(m_camRot.x, m_camRot.y, m_camRot.z);

	ImGui::End();
}

const DirectX::XMVECTOR& CCamera::GetCameraPosition() const
{
	return m_cameraPosition;
};
const DirectX::XMVECTOR& CCamera::GetCameraRotation() const
{
	return m_cameraRotation;
};
const DirectX::XMMATRIX& CCamera::GetViewMatrix() const
{
	return m_viewMatrix;
};
const DirectX::XMMATRIX& CCamera::GetProjectionMatrix() const
{
	return m_projectionMatrix;
};

const DirectX::XMVECTOR& CCamera::GetCurrentForward()
{
	return m_currentForwardDir;
}

const DirectX::XMVECTOR& CCamera::GetCurrentLeft()
{
	return m_currentLeftDir;
}
