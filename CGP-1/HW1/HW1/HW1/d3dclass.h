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
#pragma comment(lib, "d3dx10.lib")//11과 10, 혼용해서 사용


//////////////
// INCLUDES //
//////////////
#include <dxgi.h>
#include <d3dcommon.h>
#include <d3d11.h>
#include <d3dx10math.h>//다x에 따라 다른건가


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

	ID3D11Device* GetDevice();//GPU의 정보를,
	ID3D11DeviceContext* GetDeviceContext();//그 정보를 통한 맥락상 정보를 가지고온다
	//직접적 수정을 할 수도, 할 필요도 없으므로 포인터 변수로 가져온다. GPU와 연결하는 인터페이스.
	void GetProjectionMatrix(D3DXMATRIX&);//투영좌표계, 투영변환
	void GetWorldMatrix(D3DXMATRIX&);//월드좌표계, 모델링변환
	void GetOrthoMatrix(D3DXMATRIX&);//뷰잉좌표계, 뷰잉변환
	//화면에 투시하기 위한 중간 단계
	void GetVideoCardInfo(char*, int&);//GPU의 정보를 텍스트(가시)형으로 얻기위한 함수

	D3D11_FILL_MODE fillmode_(int);
	bool framechange(int);
private:
	bool m_vsync_enabled;
	int m_videoCardMemory;
	char m_videoCardDescription[128];
	IDXGISwapChain* m_swapChain;//더블버퍼, 렌더링할 백버퍼 관리, 실제 화면에 렌더링(출력)*1
	ID3D11Device* m_device;
	ID3D11DeviceContext* m_deviceContext;
	ID3D11RenderTargetView* m_renderTargetView;//렌더 타겟을 뷰 공간(백버퍼)에 넣는 애
	ID3D11Texture2D* m_depthStencilBuffer;//물체의 텍스쳐, z-buffer, 3D에 중요*2
	ID3D11DepthStencilState* m_depthStencilState;//대상의 깊이 및 스텐실 버퍼의 묘사
	ID3D11DepthStencilView* m_depthStencilView;// , , 의 보임새(실질적 보이는 것)
	ID3D11RasterizerState* m_rasterState;//래스터화*3
	D3DXMATRIX m_projectionMatrix;
	D3DXMATRIX m_worldMatrix;
	D3DXMATRIX m_orthoMatrix;
};

#endif