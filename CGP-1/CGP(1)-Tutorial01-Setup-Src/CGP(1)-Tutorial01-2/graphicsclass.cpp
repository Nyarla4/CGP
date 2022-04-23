////////////////////////////////////////////////////////////////////////////////
// Filename: graphicsclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "graphicsclass.h"


GraphicsClass::GraphicsClass()//보여주는 부분
{

}


GraphicsClass::GraphicsClass(const GraphicsClass& other)
{
}


GraphicsClass::~GraphicsClass()
{
}


bool GraphicsClass::Initialize(int screenWidth, int screenHeight, HWND hwnd)//초기화, 아직 비어있다.
{//현재는 윈도우 사이즈로 맞춰둠

	return true;
}


void GraphicsClass::Shutdown()//메모리를 안쓰고 있으니 역시 아직 비어있다
{

	return;
}


bool GraphicsClass::Frame()
{

	return true;
}


bool GraphicsClass::Render()//다x연결이 안되어있으니 역시 아직 비어있다
{

	return true;
}