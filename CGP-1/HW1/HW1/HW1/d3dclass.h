////////////////////////////////////////////////////////////////////////////////
// Filename: d3dclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _D3DCLASS_H_
#define _D3DCLASS_H_


/////////////
// LINKING //
/////////////
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dx11.lib")
#pragma comment(lib, "d3dx10.lib")//11�� 10, ȥ���ؼ� ���


//////////////
// INCLUDES //
//////////////
#include <dxgi.h>
#include <d3dcommon.h>
#include <d3d11.h>
#include <d3dx10math.h>//��x�� ���� �ٸ��ǰ�


////////////////////////////////////////////////////////////////////////////////
// Class name: D3DClass
////////////////////////////////////////////////////////////////////////////////
class D3DClass
{
public:
	D3DClass();
	D3DClass(const D3DClass&);
	~D3DClass();

	bool Initialize(int, int, bool, HWND, bool, float, float);
	void Shutdown();
	
	void BeginScene(float, float, float, float);
	void EndScene();

	ID3D11Device* GetDevice();//GPU�� ������,
	ID3D11DeviceContext* GetDeviceContext();//�� ������ ���� �ƶ��� ������ ������´�
	//������ ������ �� ����, �� �ʿ䵵 �����Ƿ� ������ ������ �����´�. GPU�� �����ϴ� �������̽�.
	void GetProjectionMatrix(D3DXMATRIX&);//������ǥ��, ������ȯ
	void GetWorldMatrix(D3DXMATRIX&);//������ǥ��, �𵨸���ȯ
	void GetOrthoMatrix(D3DXMATRIX&);//������ǥ��, ���׺�ȯ
	//ȭ�鿡 �����ϱ� ���� �߰� �ܰ�
	void GetVideoCardInfo(char*, int&);//GPU�� ������ �ؽ�Ʈ(����)������ ������� �Լ�

	D3D11_FILL_MODE fillmode_(int);
	bool framechange(int);
private:
	bool m_vsync_enabled;
	int m_videoCardMemory;
	char m_videoCardDescription[128];
	IDXGISwapChain* m_swapChain;//�������, �������� ����� ����, ���� ȭ�鿡 ������(���)*1
	ID3D11Device* m_device;
	ID3D11DeviceContext* m_deviceContext;
	ID3D11RenderTargetView* m_renderTargetView;//���� Ÿ���� �� ����(�����)�� �ִ� ��
	ID3D11Texture2D* m_depthStencilBuffer;//��ü�� �ؽ���, z-buffer, 3D�� �߿�*2
	ID3D11DepthStencilState* m_depthStencilState;//����� ���� �� ���ٽ� ������ ����
	ID3D11DepthStencilView* m_depthStencilView;// , , �� ���ӻ�(������ ���̴� ��)
	ID3D11RasterizerState* m_rasterState;//������ȭ*3
	D3DXMATRIX m_projectionMatrix;
	D3DXMATRIX m_worldMatrix;
	D3DXMATRIX m_orthoMatrix;
};

#endif