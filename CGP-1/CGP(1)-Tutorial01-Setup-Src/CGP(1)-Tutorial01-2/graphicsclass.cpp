////////////////////////////////////////////////////////////////////////////////
// Filename: graphicsclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "graphicsclass.h"


GraphicsClass::GraphicsClass()//�����ִ� �κ�
{

}


GraphicsClass::GraphicsClass(const GraphicsClass& other)
{
}


GraphicsClass::~GraphicsClass()
{
}


bool GraphicsClass::Initialize(int screenWidth, int screenHeight, HWND hwnd)//�ʱ�ȭ, ���� ����ִ�.
{//����� ������ ������� �����

	return true;
}


void GraphicsClass::Shutdown()//�޸𸮸� �Ⱦ��� ������ ���� ���� ����ִ�
{

	return;
}


bool GraphicsClass::Frame()
{

	return true;
}


bool GraphicsClass::Render()//��x������ �ȵǾ������� ���� ���� ����ִ�
{

	return true;
}