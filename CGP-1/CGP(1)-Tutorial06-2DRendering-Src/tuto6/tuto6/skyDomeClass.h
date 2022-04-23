#pragma once

#include <dxgi.h>
#include <d3dcommon.h>
#include <d3d11.h>
#include <d3dx10math.h>
#include <DirectXMath.h>

class skyDomeClass
{
private:
	struct ModelType
	{
		float x, y, z;
		float tu, tv;
		float nx, ny, nz;
	};
	struct VertexType
	{
		//XMFLOAT3 position;
	};
public:
	skyDomeClass();
	skyDomeClass(const skyDomeClass&);
	~skyDomeClass();

	bool Initialize(ID3D11Device*);
	void Shutdown();
	void Render(ID3D11DeviceContext*);

	int GetIndexCount();



private:

};

skyDomeClass::skyDomeClass()
{
}

skyDomeClass::~skyDomeClass()
{
}
