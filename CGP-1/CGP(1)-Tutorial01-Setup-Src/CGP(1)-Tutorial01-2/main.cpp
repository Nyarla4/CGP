////////////////////////////////////////////////////////////////////////////////
// Filename: main.cpp
////////////////////////////////////////////////////////////////////////////////
#include "systemclass.h"
//���α׷� ���� ����Ʈ

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow)
{
	SystemClass* System;//������ �ý��� Ŭ������ ������
	bool result;
	
	
	// Create the system object.
	System = new SystemClass;//�ý��� "����"
	if(!System)
	{
		return 0;
	}

	// Initialize and run the system object.
	result = System->Initialize();//�ý��� "�ʱ�ȭ"
	if(result)
	{
		System->Run();//�ý��� "����"
	}

	// Shutdown and release the system object.
	System->Shutdown();//�������� "�˴ٿ�"
	delete System;//����� ����
	System = 0;

	return 0;
}