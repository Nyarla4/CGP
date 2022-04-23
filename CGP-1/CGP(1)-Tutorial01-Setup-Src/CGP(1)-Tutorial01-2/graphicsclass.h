////////////////////////////////////////////////////////////////////////////////
// Filename: graphicsclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _GRAPHICSCLASS_H_
#define _GRAPHICSCLASS_H_
//�׷��� ������ �̰��� ����

//////////////
// INCLUDES //
//////////////
#include <windows.h>


/////////////
// GLOBALS //
/////////////
const bool FULL_SCREEN = false;//��üȭ��
const bool VSYNC_ENABLED = true;//������ũ
const float SCREEN_DEPTH = 1000.0f;//(��ǥ�� ���ؿ���)�󸶳� ������
const float SCREEN_NEAR = 0.1f;//(��ǥ�� ���ؿ���)�󸶳� ����
//0.1

////////////////////////////////////////////////////////////////////////////////
// Class name: GraphicsClass
////////////////////////////////////////////////////////////////////////////////
class GraphicsClass
{
public:
	GraphicsClass();
	GraphicsClass(const GraphicsClass&);
	~GraphicsClass();

	bool Initialize(int, int, HWND);//�ʱ�ȭ
	void Shutdown();//�˴ٿ�
	bool Frame();//�ý����� run�� �ش�

private:
	bool Render();//������!

private:

};

#endif