////////////////////////////////////////////////////////////////////////////////
// Filename: cameraclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _CAMERACLASS_H_
#define _CAMERACLASS_H_


//////////////
// INCLUDES //
//////////////
#include <directxmath.h>

#include "AlignedAllocationPolicy.h"

using namespace DirectX;

////////////////////////////////////////////////////////////////////////////////
// Class name: CameraClass
////////////////////////////////////////////////////////////////////////////////
class CameraClass : public AlignedAllocationPolicy<16>
{
private:
	struct ColliderType
	{
		float Mx, mx;
		float My, my;
		float Mz, mz;
	};
public:
    CameraClass();
    CameraClass(const CameraClass&);
    ~CameraClass();

    void SetPosition(XMVECTOR);
    void SetRotation(float, float, float);

    XMFLOAT3 GetPosition();
    XMFLOAT3 GetRotation();

    void Render();
    void Render(float camYaw, float camPitch, float moveLeftRight, float moveBackForward);
    void GetViewMatrix(XMMATRIX&);

    void setCollider();
    void setSize(float, float, float);
    bool checkCollider(float, float, float, float, float, float);
    void Shutdown();

private:
    XMFLOAT3 m_position;
    XMFLOAT3 m_rotation;
    XMMATRIX m_viewMatrix;

    ColliderType* m_collider;
    float sizX, sizY, sizZ;

    XMMATRIX camView;
    XMMATRIX camProjection;
    XMVECTOR camPosition;
    XMVECTOR camTarget = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
    XMVECTOR camUp = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

    XMVECTOR DefaultForward = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
    XMVECTOR DefaultRight = XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);
    XMVECTOR camForward = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
    XMVECTOR camRight = XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);

    XMMATRIX camRotationMatrix;
    XMMATRIX groundWorld;

    float moveLeftRight = 0.0f;
    float moveBackForward = 0.0f;

    float camYaw = 0.0f;
    float camPitch = 0.0f;
};

#endif