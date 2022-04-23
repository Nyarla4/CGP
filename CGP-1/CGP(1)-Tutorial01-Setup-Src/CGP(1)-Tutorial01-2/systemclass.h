////////////////////////////////////////////////////////////////////////////////
// Filename: systemclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _SYSTEMCLASS_H_
#define _SYSTEMCLASS_H_
//최상위 클래스

///////////////////////////////
// PRE-PROCESSING DIRECTIVES //
///////////////////////////////
#define WIN32_LEAN_AND_MEAN


//////////////
// INCLUDES //
//////////////
#include <windows.h>
//시스템 클래스 내에서 윈도우 API 사용


///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "inputclass.h"
#include "graphicsclass.h"


////////////////////////////////////////////////////////////////////////////////
// Class name: SystemClass
////////////////////////////////////////////////////////////////////////////////
class SystemClass//시스템 클래스 정의
{
public:
	SystemClass();
	SystemClass(const SystemClass&);
	~SystemClass();

	bool Initialize();//초기화
	void Shutdown();//종료
	void Run();//실행

	LRESULT CALLBACK/*콜백 함수*/ MessageHandler(HWND, UINT, WPARAM, LPARAM);//메시지 핸들러
	//콜백 함수 : 미리 등록해두고 일이 생기면 처리
private:
	bool Frame();
	void InitializeWindows(int&, int&);
	void ShutdownWindows();

private:
	LPCWSTR m_applicationName;//어플리케이션 이름
	HINSTANCE m_hinstance;//인스턴스
	HWND m_hwnd;//핸들러

	InputClass* m_Input;//인풋 접근 포인터
	GraphicsClass* m_Graphics;//그래픽 접근 포인터
};


/////////////////////////
// FUNCTION PROTOTYPES //
/////////////////////////
static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
//윈도우 프로시져, 시스템 클래스 밖에 있다, 마찬가지로 콜백함수

/////////////
// GLOBALS //
/////////////
static SystemClass* ApplicationHandle = 0;
//전역변수, root

#endif