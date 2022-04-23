////////////////////////////////////////////////////////////////////////////////
// Filename: modelclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _MODELCLASS_H_
#define _MODELCLASS_H_


//////////////
// INCLUDES //
//////////////
#include <d3d11.h>
#include <d3dx10math.h>


///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "textureclass.h"

#include <fstream>
using namespace std;

////////////////////////////////////////////////////////////////////////////////
// Class name: ModelClass
////////////////////////////////////////////////////////////////////////////////
class ModelClass
{
private:
	struct VertexType//그래픽스 클래스로 넘겨준다
	{
		D3DXVECTOR3 position;
	    D3DXVECTOR2 texture;
		D3DXVECTOR3 normal;//노말:법선벡터 새로 추가
	};

	struct ModelType//모델 타입자체가 새로 추가, txt파일 내용에 매치, 여기의 정보를 버텍스 셰이더로 넘겨준다.
	{
		float x, y, z;//앞의 셋은 버텍스 좌표
		float tu, tv;//다음의 둘은 텍스쳐의 UV
		float nx, ny, nz;//마지막 셋은 노말 좌표(외적, 법선벡터인듯)
	};

public:
	ModelClass();
	ModelClass(const ModelClass&);
	~ModelClass();

	bool Initialize(ID3D11Device*, char*, WCHAR*);
	void Shutdown();
	void Render(ID3D11DeviceContext*);

	int GetIndexCount();//몇개의 인덱스를 가졌는지 가져온다
	ID3D11ShaderResourceView* GetTexture();

	bool LoadModel(char*);//모델을 불러온다
	void ReleaseModel();


private:
	bool InitializeBuffers(ID3D11Device*);
	void ShutdownBuffers();
	void RenderBuffers(ID3D11DeviceContext*);

	bool LoadTexture(ID3D11Device*, WCHAR*);
	void ReleaseTexture();

private:
	ID3D11Buffer *m_vertexBuffer, *m_indexBuffer;
	int m_vertexCount, m_indexCount;
	TextureClass* m_Texture;

	ModelType* m_model;//읽어들인 모델을 넣는다
};

#endif