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
	struct VertexType//�׷��Ƚ� Ŭ������ �Ѱ��ش�
	{
		D3DXVECTOR3 position;
	    D3DXVECTOR2 texture;
		D3DXVECTOR3 normal;//�븻:�������� ���� �߰�
	};

	struct ModelType//�� Ÿ����ü�� ���� �߰�, txt���� ���뿡 ��ġ, ������ ������ ���ؽ� ���̴��� �Ѱ��ش�.
	{
		float x, y, z;//���� ���� ���ؽ� ��ǥ
		float tu, tv;//������ ���� �ؽ����� UV
		float nx, ny, nz;//������ ���� �븻 ��ǥ(����, ���������ε�)
	};

public:
	ModelClass();
	ModelClass(const ModelClass&);
	~ModelClass();

	bool Initialize(ID3D11Device*, char*, WCHAR*);
	void Shutdown();
	void Render(ID3D11DeviceContext*);

	int GetIndexCount();//��� �ε����� �������� �����´�
	ID3D11ShaderResourceView* GetTexture();

	bool LoadModel(char*);//���� �ҷ��´�
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

	ModelType* m_model;//�о���� ���� �ִ´�
};

#endif