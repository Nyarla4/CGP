#ifndef _SKYMAPCLASS_H_
#define _SKYMAPCLASS_H_


#include <d3d11.h>
#include <directxmath.h>
#include <d3dcompiler.h>
#include <vector>

using namespace DirectX;

#include "textureclass.h"
#include "DirectXTex.h"
#include "DDSTextureLoader.h"

#include <fstream>

class SkymapClass
{
private:

	struct Vertex	//Overloaded Vertex Structure
	{
		Vertex() {}
		Vertex(float x, float y, float z,
			float u, float v,
			float nx, float ny, float nz)
			: pos(x, y, z), texCoord(u, v), normal(nx, ny, nz) {}

		XMFLOAT3 pos;
		XMFLOAT2 texCoord;
		XMFLOAT3 normal;
	};

	struct cbPerObject
	{
		XMMATRIX  WVP;
		XMMATRIX World;
	};

	cbPerObject cbPerObj;

public:
	SkymapClass();
	SkymapClass(const SkymapClass&);
	~SkymapClass();

	bool Initialize(ID3D11Device*, WCHAR*, int, int);
	void Shutdown();
	void Render(ID3D11DeviceContext*, XMMATRIX, XMMATRIX, XMMATRIX);

private:
	bool InitializeBuffers(ID3D11Device*, int, int);
	void ShutdownBuffers();
	void RenderBuffers(ID3D11DeviceContext*, XMMATRIX, XMMATRIX, XMMATRIX);

private:

	ID3D11Buffer* cbPerObjectBuffer;
	ID3D11SamplerState* CubesTexSamplerState;

	ID3D11Buffer* d2dVertBuffer;
	ID3D11Buffer* d2dIndexBuffer;
	ID3D11ShaderResourceView* d2dTexture;

	ID3D11Buffer* sphereIndexBuffer;
	ID3D11Buffer* sphereVertBuffer;
	ID3D11VertexShader* SKYMAP_VS;
	ID3D11PixelShader* SKYMAP_PS;
	ID3D10Blob* SKYMAP_VS_Buffer;
	ID3D10Blob* SKYMAP_PS_Buffer;

	TextureClass* m_Texture;

	ID3D11ShaderResourceView* smrv;

	XMMATRIX Rotationx;
	XMMATRIX Rotationy;

public:
	int NumSphereVertices;
	int NumSphereFaces;

	XMMATRIX WVP;
};

#endif