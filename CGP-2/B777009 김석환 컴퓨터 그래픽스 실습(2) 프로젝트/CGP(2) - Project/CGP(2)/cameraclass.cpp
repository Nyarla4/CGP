////////////////////////////////////////////////////////////////////////////////
// Filename: cameraclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "cameraclass.h"


CameraClass::CameraClass()
{
	m_position.x = 0.0f;
	m_position.y = 0.0f;
	m_position.z = 0.0f;

	m_rotation.x = 0.0f;
	m_rotation.y = 0.0f;
	m_rotation.z = 0.0f;

	m_collider = new ColliderType;//콜라이더 타입 변수 추가
	sizX = sizY = sizZ = 0.0f;//카메라 사이즈 변수 추가
}


CameraClass::CameraClass(const CameraClass& other)
{
}


CameraClass::~CameraClass()
{
}


void CameraClass::SetPosition(XMVECTOR camPosition)
{
	this->camPosition = camPosition;
}


void CameraClass::SetRotation(float x, float y, float z)
{
	m_rotation.x = x;
	m_rotation.y = y;
	m_rotation.z = z;
}


XMFLOAT3 CameraClass::GetPosition()
{
	XMFLOAT3 returnPosition;
	XMStoreFloat3(&returnPosition, camPosition);
	return returnPosition;
}


XMFLOAT3 CameraClass::GetRotation()
{
	return m_rotation;
}

// This uses the position and rotation of the camera to build and to update the view matrix.
void CameraClass::Render()
{
	XMVECTOR up, position, lookAt;
	float yaw, pitch, roll;
	XMMATRIX rotationMatrix;

	// Setup the vector that points upwards.
	up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

	// Setup the position of the camera in the world.
	position = XMLoadFloat3(&m_position);

	// Setup where the camera is looking by default.
	lookAt = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);

	// Set the yaw (Y axis), pitch (X axis), and roll (Z axis) rotations in radians.
	pitch = m_rotation.x * 0.0174532925f;
	yaw   = m_rotation.y * 0.0174532925f;
	roll  = m_rotation.z * 0.0174532925f;

	// Create the rotation matrix from the yaw, pitch, and roll values.
	rotationMatrix = XMMatrixRotationRollPitchYaw(pitch, yaw, roll);

	// Transform the lookAt and up vector by the rotation matrix so the view is correctly rotated at the origin.
	lookAt = XMVector3TransformCoord(lookAt, rotationMatrix);
	up = XMVector3TransformCoord(up, rotationMatrix);

	// Translate the rotated camera position to the location of the viewer.
	lookAt = position + lookAt;

	// Finally create the view matrix from the three updated vectors.
	m_viewMatrix = XMMatrixLookAtLH(position, lookAt, up);

	return;
}


void CameraClass::GetViewMatrix(XMMATRIX& viewMatrix)
{
	viewMatrix = m_viewMatrix;
}

void CameraClass::Render(float camYaw, float camPitch, float moveLeftRight, float moveBackForward)
{
	camRotationMatrix = XMMatrixRotationRollPitchYaw(camPitch, camYaw, 0);

	camTarget = XMVector3TransformCoord(DefaultForward, camRotationMatrix);
	camTarget = XMVector3Normalize(camTarget);

	XMMATRIX RotateYTempMatrix;
	RotateYTempMatrix = XMMatrixRotationY(camYaw);

	camRight = XMVector3TransformCoord(DefaultRight, RotateYTempMatrix);
	camUp = XMVector3TransformCoord(camUp, RotateYTempMatrix);
	camForward = XMVector3TransformCoord(DefaultForward, RotateYTempMatrix);

	camPosition += moveLeftRight * camRight;
	camPosition += moveBackForward * camForward;

	moveLeftRight = 0.0f;
	moveBackForward = 0.0f;

	camTarget = camPosition + camTarget;

	m_viewMatrix = XMMatrixLookAtLH(camPosition, camTarget, camUp);
	
	return;
}

void CameraClass::setCollider()//콜라이더 설정 추가
{
	XMFLOAT3 returnPosition;
	XMStoreFloat3(&returnPosition, camPosition);
	m_collider->Mx = returnPosition.x + sizX / 2;
	m_collider->mx = returnPosition.x - sizX / 2;
	m_collider->My = returnPosition.y + sizY / 2;
	m_collider->my = returnPosition.y - sizY / 2;
	m_collider->Mz = returnPosition.z + sizZ / 2;
	m_collider->mz = returnPosition.z - sizZ / 2;
}

void CameraClass::setSize(float x, float y, float z)//카메라 사이즈 설정 추가
{
	sizX = x; sizY = y; sizZ = z;
}

bool CameraClass::checkCollider(float max, float mix, float may, float miy, float maz, float miz)//충돌 체크 함수 추가, 충돌 시 트루, 그 외 펄스
{
	if ((mix <= m_collider->Mx && m_collider->Mx <= max) || (mix <= m_collider->mx && m_collider->mx <= max))
	{
		if ((miy <= m_collider->My && m_collider->My <= may) || (miy <= m_collider->my && m_collider->my <= may))
		{
			if ((miz <= m_collider->Mz && m_collider->Mz <= maz) || (miz <= m_collider->mz && m_collider->mz <= maz))
			{
				return true;
			}
		}
	}

	return false;
}
void CameraClass::Shutdown()//릴리즈용 셧다운 함수 추가
{
	if (m_collider)
	{
		delete m_collider;
		m_collider = 0;
	}
}