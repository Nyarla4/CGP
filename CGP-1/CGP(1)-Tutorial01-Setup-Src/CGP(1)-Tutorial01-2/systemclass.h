////////////////////////////////////////////////////////////////////////////////
// Filename: systemclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _SYSTEMCLASS_H_
#define _SYSTEMCLASS_H_
//�ֻ��� Ŭ����

///////////////////////////////
// PRE-PROCESSING DIRECTIVES //
///////////////////////////////
#define WIN32_LEAN_AND_MEAN


//////////////
// INCLUDES //
//////////////
#include <windows.h>
//�ý��� Ŭ���� ������ ������ API ���


///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "inputclass.h"
#include "graphicsclass.h"


////////////////////////////////////////////////////////////////////////////////
// Class name: SystemClass
////////////////////////////////////////////////////////////////////////////////
class SystemClass//�ý��� Ŭ���� ����
{
public:
	SystemClass();
	SystemClass(const SystemClass&);
	~SystemClass();

	bool Initialize();//�ʱ�ȭ
	void Shutdown();//����
	void Run();//����

	LRESULT CALLBACK/*�ݹ� �Լ�*/ MessageHandler(HWND, UINT, WPARAM, LPARAM);//�޽��� �ڵ鷯
	//�ݹ� �Լ� : �̸� ����صΰ� ���� ����� ó��
private:
	bool Frame();
	void InitializeWindows(int&, int&);
	void ShutdownWindows();

private:
	LPCWSTR m_applicationName;//���ø����̼� �̸�
	HINSTANCE m_hinstance;//�ν��Ͻ�
	HWND m_hwnd;//�ڵ鷯

	InputClass* m_Input;//��ǲ ���� ������
	GraphicsClass* m_Graphics;//�׷��� ���� ������
};


/////////////////////////
// FUNCTION PROTOTYPES //
/////////////////////////
static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
//������ ���ν���, �ý��� Ŭ���� �ۿ� �ִ�, ���������� �ݹ��Լ�

/////////////
// GLOBALS //
/////////////
static SystemClass* ApplicationHandle = 0;
//��������, root

#endif