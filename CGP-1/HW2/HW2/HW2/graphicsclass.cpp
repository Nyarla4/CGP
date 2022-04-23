////////////////////////////////////////////////////////////////////////////////
// Filename: graphicsclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "graphicsclass.h"


GraphicsClass::GraphicsClass()
{
	m_D3D = 0;
	m_Camera = 0;
	for (int i = 0; i < 4; i++) {
		m_Model[i] = 0;
	}
	m_TextureShader = 0;
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
	if(!m_D3D)
	{
		return false;
	}

	// Initialize the Direct3D object.
	result = m_D3D->Initialize(screenWidth, screenHeight, VSYNC_ENABLED, hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize Direct3D.", L"Error", MB_OK);
		return false;
	}

	// Create the camera object.
	m_Camera = new CameraClass;
	if(!m_Camera)
	{
		return false;
	}

	// Set the initial position of the camera.
	m_Camera->SetPosition(0.0f, 0.0f, -10.0f);
	
	// Create the model object.
	m_Model[0] = new ModelClass; if (!m_Model) { return false; }
	m_Model[1] = new ModelClass; if (!m_Model) { return false; }
	m_Model[2] = new ModelClass; if (!m_Model) { return false; }
	m_Model[3] = new ModelClass; if (!m_Model) { return false; }

	// Initialize the model object.
	result = m_Model[0]->Initialize(m_D3D->GetDevice(), (CHAR*)"./data/helmet.obj", (WCHAR*)L"./data/brushed metal background 1605.jpg"); if (!result) { MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);		return false; }
	result = m_Model[1]->Initialize(m_D3D->GetDevice(), (CHAR*)"./data/pot.obj", (WCHAR*)L"./data/green-grass-textures.jpg"); if (!result) { MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);		return false; }
	result = m_Model[2]->Initialize(m_D3D->GetDevice(), (CHAR*)"./data/Guitar_01.obj", (WCHAR*)L"./data/vector-polished-metallic-texture.jpg"); if (!result) { MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);		return false; }
	result = m_Model[3]->Initialize(m_D3D->GetDevice(), (CHAR*)"./data/cube.obj", (WCHAR*)L"./data/KakaoTalk_20210429_102919305.jpg"); if (!result) { MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);		return false; }
	

	// Create the texture shader object.
	m_TextureShader = new TextureShaderClass;
	if(!m_TextureShader)
	{
		return false;
	}

	// Initialize the texture shader object.
	result = m_TextureShader->Initialize(m_D3D->GetDevice(), hwnd);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the texture shader object.", L"Error", MB_OK);
		return false;
	}

	return true;
}


void GraphicsClass::Shutdown()
{
	// Release the texture shader object.
	if(m_TextureShader)
	{
		m_TextureShader->Shutdown();
		delete m_TextureShader;
		m_TextureShader = 0;
	}

	// Release the model object.
	if(m_Model)
	{
		for (int i = 0; i < 4; i++) {
			m_Model[i]->Shutdown();
			delete m_Model[i];
			m_Model[i] = 0;
		}
	}

	// Release the camera object.
	if(m_Camera)
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


bool GraphicsClass::Frame()
{
	bool result; static float rotation = 0.0f;

	// Update the rotation variable each frame.
	rotation += (float)D3DX_PI * 0.01f;
	if (rotation > 360.0f)
	{
		rotation -= 360.0f;
	}
	// Render the graphics scene.
	result = Render(rotation);
	if(!result)
	{
		return false;
	}

	return true;
}


bool GraphicsClass::Render(float rotation)
{
	D3DXMATRIX worldMatrix[4], viewMatrix, projectionMatrix;
	bool result; D3DXMATRIX moveMat[4], rotaMat[4], scaMat[4];


	// Clear the buffers to begin the scene.
	m_D3D->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

	// Generate the view matrix based on the camera's position.
	m_Camera->Render();//카메라 렌더 먼저

	// Get the world, view, and projection matrices from the camera and d3d objects.
	m_Camera->GetViewMatrix(viewMatrix);
	for (int i = 0; i < 4; i++) {
		m_D3D->GetWorldMatrix(worldMatrix[i]);
	}
	m_D3D->GetProjectionMatrix(projectionMatrix);

	// Rotate the world matrix by the rotation value so that the triangle will spin.
	
	D3DXMatrixTranslation(&moveMat[0], -3.0f, 2.0f, 10.0f);	D3DXMatrixRotationY(&rotaMat[0], rotation);
	D3DXMatrixTranslation(&moveMat[1], 3.0f, 0.0f, 0.0f);	D3DXMatrixRotationY(&rotaMat[1], rotation);
	D3DXMatrixTranslation(&moveMat[2], -3.0f, -2.0f, 0.0f);	D3DXMatrixRotationY(&rotaMat[2], -rotation);
	D3DXMatrixTranslation(&moveMat[3], 0.0f, -3.0f, 15.0f);	D3DXMatrixRotationY(&rotaMat[3], 0.0f);
	D3DXMatrixScaling(&scaMat[0], 0.1f, 0.1f, 0.1f);
	D3DXMatrixScaling(&scaMat[1], 5.0f, 5.0f, 5.0f);
	D3DXMatrixScaling(&scaMat[2], 5.0f, 5.0f, 5.0f);
	D3DXMatrixScaling(&scaMat[3], 75.0f, 0.1f, 75.0f);
	for (int i = 0; i < 4; i++)
	{
		worldMatrix[i] = scaMat[i] * rotaMat[i] * moveMat[i];
	}
	// Put the model vertex and index buffers on the graphics pipeline to prepare them for drawing.
	
	// Render the model using the texture shader.
	for (int i = 0; i < 4; i++) {
		m_Model[i]->Render(m_D3D->GetDeviceContext());
		result = m_TextureShader->Render(m_D3D->GetDevice(), m_D3D->GetDeviceContext(), m_Model[i]->GetIndexCount(), worldMatrix[i], viewMatrix, projectionMatrix,
			m_Model[i]->GetTexture());//더 나중에 하는 게 텍스쳐 렌더
	}
	if(!result)
	{
		return false;
	}

	// Present the rendered scene to the screen.
	m_D3D->EndScene();//렌더에 필요한것들은 비긴씬과 엔드씬 사이

	return true;
}

void GraphicsClass::filterChange(int mode)
{
	m_TextureShader->filterMode = mode;
}