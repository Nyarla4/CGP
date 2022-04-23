////////////////////////////////////////////////////////////////////////////////
// Filename: main.cpp
////////////////////////////////////////////////////////////////////////////////
#include "systemclass.h"
//프로그램 시작 포인트

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow)
{
	SystemClass* System;//하위로 시스템 클래스를 가진다
	bool result;
	
	
	// Create the system object.
	System = new SystemClass;//시스템 "생성"
	if(!System)
	{
		return 0;
	}

	// Initialize and run the system object.
	result = System->Initialize();//시스템 "초기화"
	if(result)
	{
		System->Run();//시스템 "실행"
	}

	// Shutdown and release the system object.
	System->Shutdown();//끝났을때 "셧다운"
	delete System;//종료시 제거
	System = 0;

	return 0;
}