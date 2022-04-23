////////////////////////////////////////////////////////////////////////////////
// Filename: graphicsclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "graphicsclass.h"


GraphicsClass::GraphicsClass()
{
	m_D3D = 0;
	m_Camera = 0;
	for (int i = 0; i < 3; i++) {
		m_Model[i] = 0;
	}
	m_ColorShader = 0;
	r = g = b = 0.0f;
	frame = 0.0f;
	fillmode = 1;
	lightmode = 0;
}


GraphicsClass::GraphicsClass(const GraphicsClass& other)
{
}


GraphicsClass::~GraphicsClass()
{
}


bool GraphicsClass::Initialize(int screenWidth, int screenHeight, HWND hwnd)
{
	bool result;


	// Create the Direct3D object.
	m_D3D = new D3DClass;
	if (!m_D3D)
	{
		return false;
	}

	// Initialize the Direct3D object.
	result = m_D3D->Initialize(screenWidth, screenHeight, VSYNC_ENABLED, hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize Direct3D.", L"Error", MB_OK);
		return false;
	}

	m_Camera = new CameraClass;
	if (!m_Camera)
	{
		return false;
	}

	// Set the initial position of the camera.카메라 위치 초기화(설정)
	m_Camera->SetPosition(0.0f, 0.0f, -10.0f);//왼손 좌표계라 -(즉, 뒤)로 좀 빼둬야함

	// Create the model object.
	for (int i = 0; i < 3; i++)
	{
		m_Model[i] = new ModelClass;
	}
	if (!m_Model)
	{
		return false;
	}

	// Initialize the model object.
	for (int i = 0; i < 3; i++) {
		result = m_Model[i]->Initialize(i, m_D3D->GetDevice());
		if (!result)
		{
			MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
			return false;
		}
	}
	

	// Create the color shader object.
	m_ColorShader = new ColorShaderClass;
	if (!m_ColorShader)
	{
		return false;
	}

	// Initialize the color shader object.
	result = m_ColorShader->Initialize(m_D3D->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the color shader object.", L"Error", MB_OK);
		return false;
	}

	return true;
}


void GraphicsClass::Shutdown()
{

	// Release the color shader object.
	if (m_ColorShader)
	{
		m_ColorShader->Shutdown();
		delete m_ColorShader;
		m_ColorShader = 0;
	}

	// Release the model object.
	if (m_Model)
	{
		for (int i = 0; i < 3; i++) {
			m_Model[i]->Shutdown();
			delete m_Model[i];
			m_Model[i] = 0;
		}
	}

	// Release the camera object.
	if (m_Camera)
	{
		delete m_Camera;
		m_Camera = 0;
	}

	// Release the D3D object.
	if(m_D3D)
	{
		m_D3D->Shutdown();
		delete m_D3D;
		m_D3D = 0;
	}

	return;
}


bool GraphicsClass::Frame(HWND hwnd)
{
	bool result;
	int screenWidth = GetSystemMetrics(SM_CXSCREEN);
	int screenHeight = GetSystemMetrics(SM_CYSCREEN);
	//m_D3D->Initialize(screenWidth, screenHeight, VSYNC_ENABLED, hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR, fillmode);
	// Render the graphics scene.
	result = Render();
	if(!result)
	{
		return false;
	}

	return true;
}

bool GraphicsClass::Render()//몬가 몬가 일어나는 곳, 매트릭스들 존재하기도 하고
{//그리는게 일어나는 곳

	D3DXMATRIX worldMatrix[3], viewMatrix, projectionMatrix;
	D3DXMATRIX moveMat[3], rotaMat[3];
	bool result;
	// Clear the buffers to begin the scene.
	m_D3D->BeginScene(r, g, b, 1.0f);
	
	// Generate the view matrix based on the camera's position.
	m_Camera->Render();

	// Get the world, view, and projection matrices from the camera and d3d objects.
	m_Camera->GetViewMatrix(viewMatrix);
	for (int i = 0; i < 3; i++){
		m_D3D->GetWorldMatrix(worldMatrix[i]);
	}
	m_D3D->GetProjectionMatrix(projectionMatrix);
	//매트릭스(행렬?)다가지고왔다
	m_D3D->framechange(fillmode);
	float angle = 1.0f; frame += 0.1f;
	D3DXMatrixTranslation(&moveMat[0], -3.0f, 2.0f, 0.0f);
	D3DXMatrixRotationX(&rotaMat[1], angle * frame);
	D3DXMatrixTranslation(&moveMat[1], 3.0f, 0.0f, 0.0f);
	D3DXMatrixRotationY(&rotaMat[0], angle * frame);
	D3DXMatrixTranslation(&moveMat[2], -3.0f, -2.0f, 0.0f);
	D3DXMatrixRotationZ(&rotaMat[2], angle * frame);
	
	for (int i = 0; i < 3; i++)
	{
		worldMatrix[i] = rotaMat[i] * moveMat[i];
	}

	for (int i = 0; i < 3; i++) {
		m_Model[i]->Render(m_D3D->GetDeviceContext());
		result = m_ColorShader->Render(m_D3D->GetDevice(), m_D3D->GetDeviceContext(), m_Model[i]->GetIndexCount(),
			worldMatrix[i], viewMatrix, projectionMatrix, lightmode);
		if (!result)
		{
			return false;
		}
	}

	// Present the rendered scene to the screen.
	m_D3D->EndScene();

	return true;
}

