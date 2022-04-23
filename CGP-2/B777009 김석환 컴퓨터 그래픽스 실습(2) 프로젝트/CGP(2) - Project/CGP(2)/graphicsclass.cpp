////////////////////////////////////////////////////////////////////////////////
// Filename: graphicsclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "graphicsclass.h"


GraphicsClass::GraphicsClass()
{
	moveLeftRight = 0.0f;
	moveBackForward = 0.0f;

	ghost_rotate = 0.0f; cross_rotate = 0.0f; cross_height = 1.0f; crossH = 0.01f;
	gone_ghost_height = 0.0f; gone_ghost_H = 0.01f;

	m_D3D = 0;
	m_Camera = 0;
	m_Ghost = 0;
	m_Terr = 0;
	m_grave = 0;
	m_cross = 0;
	equip = false;
	m_SkyBox = 0;

	m_LightShader = 0;
	m_Light = 0;

	m_TextureShader = 0;
	m_Bitmap = 0;

	m_Text = 0;
	m_TextR = 0;
	isCollide = 0;

	tempX = tempY = tempZ = 0.0f;

	num_total = 0;
	poly_total = 0;
	num_ghost = 0;
	num_grave = 0;
	grave_line = 0;
	grave_term = 0.0f;

	for (int i = 0; i < 4; i++)
	{
		caught[i] = false;
		timecalc[i] = 0.0f;
		ghostRand[i] = -1;
	}

	m_time = 0;
	frametime = 0;

	m_ParticleShader = 0;
	m_ParticleSystem[0] = 0;
	m_ParticleSystem[1] = 0;

	sound = false;
	limitTime = 50.0f;

	m_SkyMap = 0;

	sceneWchange = false;
	sceneLchange = false;
}


GraphicsClass::GraphicsClass(const GraphicsClass& other)
{
}


GraphicsClass::~GraphicsClass()
{
}
int a = 0;

bool GraphicsClass::Initialize(int screenWidth, int screenHeight, HWND hwnd)
{
	bool result;
	screenW = screenWidth;
	screenH = screenHeight;
	srand(time(NULL));
	crossX = rand() % 30 + 1;
	crossZ = rand() % 30 + 1; //(((float)rand() - (float)rand()) / RAND_MAX) + 30.0f;
	for (int i = 0; i < 4; i++)
	{
		ghostRand[i] = rand() % 16;
		for (int j = 0; j < i; j++)
		{
			if (ghostRand[i] == ghostRand[j])
				i--;
		}
	}
	a = 0;

	XMMATRIX baseViewMatrix;

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
	m_Camera->setSize(0.75f, 1.0f, 0.75f);
	// Set the initial position of the camera.
	camPosition = XMVectorSet(0.0f, 2.0f, 0.0f, 0.0f);
	m_Camera->SetPosition(camPosition);	// for cube

	// 파티클 셰이더 개체를 만듭니다.
	m_ParticleShader = new ParticleShaderClass;
	if (!m_ParticleShader)
	{
		return false;
	}

	// 파티클 셰이더 개체를 초기화합니다.
	if (!m_ParticleShader->Initialize(m_D3D->GetDevice(), hwnd))
	{
		MessageBox(hwnd, L"Could not initialize the particle shader object.", L"Error", MB_OK);
		return false;
	}

	// 파티클 시스템 객체를 만듭니다.
	m_ParticleSystem[0] = new ParticleSystemClass;
	if (!m_ParticleSystem[0])
	{
		return false;
	}

	// 파티클 시스템 객체를 초기화합니다.
	if (!m_ParticleSystem[0]->Initialize(m_D3D->GetDevice(), L"./data/particle/star.dds",1))
	{
		return false;
	}
	
	// 파티클 시스템 객체를 만듭니다.
	m_ParticleSystem[1] = new ParticleSystemClass;
	if (!m_ParticleSystem[1])
	{
		return false;
	}

	// 파티클 시스템 객체를 초기화합니다.
	if (!m_ParticleSystem[1]->Initialize(m_D3D->GetDevice(), L"./data/particle/star.dds", 2))//temp.dds",2))
	{
		return false;
	}

	m_SkyBox = new ModelClass;
	if (!m_SkyBox)
	{
		return false;
	}

	// Initialize the model object.
	result = m_SkyBox->Initialize(m_D3D->GetDevice(), (char*)"./data/modelNtexture/skybox.obj", (WCHAR*)L"./data/modelNtexture/NightSky.dds");

	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the skybox object.", L"Error", MB_OK);
		return false;
	}
	// Create the model object.
	//m_Model = new ModelClass;
	//if (!m_Model)
	//{
	//	return false;
	//}
	//result = m_Model->Initialize(m_D3D->GetDevice(), (char*)"./data/human.obj", (WCHAR*)L"./data/human.dds");
	//if (!result)
	//{
	//	MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
	//	return false;
	//}
	m_Ghost = new ModelClass;
	if (!m_Ghost)
	{
		return false;
	}
	result = m_Ghost->Initialize(m_D3D->GetDevice(), (char*)"./data/modelNtexture/ghost.obj", (WCHAR*)L"./data/modelNtexture/ghost.dds");
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}
	m_Terr = new ModelClass;
	if (!m_Terr)
	{
		return false;
	}
	result = m_Terr->Initialize(m_D3D->GetDevice(), (char*)"./data/modelNtexture/cube.obj", (WCHAR*)L"./data/modelNtexture/terr.dds");
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}
	m_grave = new ModelClass;
	if (!m_grave)
	{
		return false;
	}
	result = m_grave->Initialize(m_D3D->GetDevice(), (char*)"./data/modelNtexture/grave.obj", (WCHAR*)L"./data/modelNtexture/grave.dds");
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}
	m_cross = new ModelClass;
	if (!m_cross)
	{
		return false;
	}
	result = m_cross->Initialize(m_D3D->GetDevice(), (char*)"./data/modelNtexture/cross.obj", (WCHAR*)L"./data/modelNtexture/cross___.dds");
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}
	m_house = new ModelClass;
	if (!m_house)
	{
		return false;
	}
	result = m_house->Initialize(m_D3D->GetDevice(), (char*)"./data/modelNtexture/house.obj", (WCHAR*)L"./data/modelNtexture/house.dds");
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}

	// Create the light shader object.
	m_LightShader = new LightShaderClass;
	if (!m_LightShader)
	{
		return false;
	}

	// Initialize the light shader object.
	result = m_LightShader->Initialize(m_D3D->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the light shader object.", L"Error", MB_OK);
		return false;
	}

	// Create the light object.
	m_Light = new LightClass;
	if (!m_Light)
	{
		return false;
	}

	// Initialize the light object.
	m_Light->SetAmbientColor(0.1f, 0.1f, 0.4f, 1.0f);
	m_Light->SetDiffuseColor(0.5f, 0.5f, 0.25f, 1.0f);
	m_Light->SetDirection(1.0f, -1.0f, 1.0f);
	m_Light->SetSpecularColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_Light->SetSpecularPower(256.0f);

	ambient = new XMFLOAT4(0.5f, 0.5f, 0.75f, 1.0f);
	diffuse = new XMFLOAT4(0.7f, 0.7f, 0.5f, 1.0f);
	specular = new XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	specularP = 32.0f;

	// Create the texture shader object.
	m_TextureShader = new TextureShaderClass;
	if (!m_TextureShader)
	{
		return false;
	}

	// Initialize the texture shader object.
	result = m_TextureShader->Initialize(m_D3D->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the texture shader object.", L"Error", MB_OK);
		return false;
	}

	
	// Create the bitmap object.
	m_Bitmap = new BitmapClass;
	if (!m_Bitmap)
	{
		return false;
	}

	// Initialize the bitmap object.
	result = m_Bitmap->Initialize(m_D3D->GetDevice(), screenWidth, screenHeight, L"./data/titleTemp.dds", 256, 256);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the bitmap object.", L"Error", MB_OK);
		return false;
	}
	
	// Initialize a base view matrix with the camera for 2D user interface rendering.
//	m_Camera->SetPosition(0.0f, 0.0f, -1.0f);
	m_Camera->Render(camYaw, camPitch, moveLeftRight, moveBackForward);
	m_Camera->GetViewMatrix(baseViewMatrix);

	// Create the text object.
	m_Text = new TextClass;
	if (!m_Text)
	{
		return false;
	}

	// Initialize the text object.
	result = m_Text->Initialize(m_D3D->GetDevice(), m_D3D->GetDeviceContext(), hwnd, screenWidth, screenHeight, baseViewMatrix, L"./data/fontdata.txt", L"./data/font.dds");
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the text object.", L"Error", MB_OK);
		return false;
	}
	
	m_TextR = new TextClass;
	if (!m_TextR)
	{
		return false;
	}

	result = m_TextR->Initialize(m_D3D->GetDevice(), m_D3D->GetDeviceContext(), hwnd, screenWidth, screenHeight, baseViewMatrix, L"./data/fontdata.txt", L"./data/font.dds");
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the text object.", L"Error", MB_OK);
		return false;
	}

	m_SkyMap = new SkymapClass;
	if (!m_SkyMap)
	{
		return false;
	}

	// Initialize the skymap object.
	result = m_SkyMap->Initialize(m_D3D->GetDevice(), (WCHAR*)L"./data/NightSky.dds", 10, 10);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the Skymap object.", L"Error", MB_OK);
		return false;
	}

	return true;
}


void GraphicsClass::Shutdown()
{
	if (m_house)
	{
		m_house->Shutdown();
		delete m_house;
		m_house = 0;
	}
	if (m_SkyMap)
	{
		m_SkyMap->Shutdown();
		delete m_SkyMap;
		m_SkyMap = 0;
	}

	if (m_ParticleSystem)
	{
		for (int i = 0; i < 2; i++)
		{
			m_ParticleSystem[i]->Shutdown();
			delete m_ParticleSystem[i];
			m_ParticleSystem[i] = 0;
		}
	}

	// particle shader 객체를 해제한다.
	if (m_ParticleShader)
	{
		m_ParticleShader->Shutdown();
		delete m_ParticleShader;
		m_ParticleShader = 0;
	}
	// Release the model object.
	if (m_SkyBox)
	{
		m_SkyBox->Shutdown();
		delete m_SkyBox;
		m_SkyBox = 0;
	}
	//if(m_Model)
	//{
	//	m_Model->Shutdown();
	//	delete m_Model;
	//	m_Model = 0;
	//}
	if (m_Ghost)
	{
		m_Ghost->Shutdown();
		delete m_Ghost;
		m_Ghost = 0;
	}
	if (m_Terr)
	{
		m_Terr->Shutdown();
		delete m_Terr;
		m_Terr = 0;
	}
	if (m_grave)
	{
		m_grave->Shutdown();
		delete m_grave;
		m_grave = 0;
	}
	if (m_cross)
	{
		m_cross->Shutdown();
		delete m_cross;
		m_cross = 0;
	}
	// Release the camera object.
	if(m_Camera)
	{
		m_Camera->Shutdown();
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

	// Release the light object.
	if (m_Light)
	{
		delete m_Light;
		m_Light = 0;
	}

	// Release the light shader object.
	if (m_LightShader)
	{
		m_LightShader->Shutdown();
		delete m_LightShader;
		m_LightShader = 0;
	}
	
	// Release the bitmap object.
	if (m_Bitmap)
	{
		m_Bitmap->Shutdown();
		delete m_Bitmap;
		m_Bitmap = 0;
	}

	// Release the texture shader object.
	if (m_TextureShader)
	{
		m_TextureShader->Shutdown();
		delete m_TextureShader;
		m_TextureShader = 0;
	}

	if (m_Text)
	{
		m_Text->Shutdown();
		delete m_Text;
		m_Text = 0;
	}
	if (m_TextR)
	{
		m_TextR->Shutdown();
		delete m_TextR;
		m_TextR = 0;
	}

	return;
}

bool GraphicsClass::Frame()
{
	bool result;

	static float rotation = 0.0f;


	// Update the rotation variable each frame.
	rotation += XM_PI * 0.005f;
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

bool GraphicsClass::Frame(int fps, int cpu, float frameTime, int scene)
{
	bool result;
	static float rotation = 0.0f;
	int ghostNum = num_ghost;
	// Update the rotation variable each frame.
	rotation += (float)XM_PI * 0.005f;
	if (rotation > 360.0f)
	{
		rotation -= 360.0f;
	}
	switch (scene)
	{
	case -1:
		m_Bitmap->textureChange(m_D3D->GetDevice(), L"./data/titleTemp.dds");
	{
		result = m_TextR->SetFps(-2, m_D3D->GetDeviceContext());
		if (!result)
		{
			return false;
		}
		result = m_TextR->SetCpu(-2, m_D3D->GetDeviceContext());
		if (!result)
		{
			return false;
		}
		result = m_TextR->SetNum(-2, m_D3D->GetDeviceContext());
		if (!result)
		{
			return false;
		}
		result = m_TextR->SetPoly(-2, m_D3D->GetDeviceContext());
		if (!result)
		{
			return false;
		}
		result = m_TextR->SetScreen(-3, -3, m_D3D->GetDeviceContext());
		if (!result)
		{
			return false;
		}
		result = m_TextR->SetTimer(-3, m_D3D->GetDeviceContext());
		if (!result)
		{
			return false;
		}
		result = m_TextR->SetGhost(-3, m_D3D->GetDeviceContext());
		if (!result)
		{
			return false;
		}
	}
		result = RenderTitle();
		break;
	case 1://메인 게임
	{
		if (fps <= 0)
		{
			m_time = 0;
		}
		else
		{
			m_time += 1.0f / fps;
		}
		for (int i = 0; i < 4; i++)
		{
			if (caught[i])
				ghostNum--;
		}
		// Set the frames per second.
		result = m_Text->SetFps(fps, m_D3D->GetDeviceContext());
		if (!result)
		{
			return false;
		}

		// Set the cpu usage.
		result = m_Text->SetCpu(cpu, m_D3D->GetDeviceContext());
		if (!result)
		{
			return false;
		}

		result = m_Text->SetNum(num_total - num_ghost + ghostNum, m_D3D->GetDeviceContext());
		if (!result)
		{
			return false;
		}

		result = m_Text->SetPoly(poly_total, m_D3D->GetDeviceContext());
		if (!result)
		{
			return false;
		}

		result = m_Text->SetScreen(screenW, screenH, m_D3D->GetDeviceContext());
		if (!result)
		{
			return false;
		}

		result = m_Text->SetTimer(limitTime - m_time, m_D3D->GetDeviceContext());
		if (!result)
		{
			return false;
		}

		result = m_Text->SetGhost(ghostNum, m_D3D->GetDeviceContext());
		if (!result)
		{
			return false;
		}

		m_ParticleSystem[0]->Frame(frameTime, m_D3D->GetDeviceContext());
		m_ParticleSystem[1]->Frame(frameTime, m_D3D->GetDeviceContext());
		
		result = Render(rotation);
		
		if (caught[0] && caught[1] && caught[2] && caught[3])
		{
			camPosition = XMVectorSet(0.0f, 2.0f, 0.0f, 0.0f);
			sceneWchange = true;
			record = m_time + 1.0f;
		}
		if (limitTime - m_time <= 0)
		{
			camPosition = XMVectorSet(0.0f, 2.0f, 0.0f, 0.0f);
			sceneLchange = true;
			gone_ghost_height = 0.5f;
			gone_ghost_H = 0.01f;
		}
	}
		break;
	case 2://승리화면
		m_Bitmap->textureChange(m_D3D->GetDevice(), L"./data/winTemp.dds");
		{
			result = m_TextR->SetFps(-2, m_D3D->GetDeviceContext());
			if (!result)
			{
				return false;
			}
			result = m_TextR->SetCpu(-2, m_D3D->GetDeviceContext());
			if (!result)
			{
				return false;
			}
			result = m_TextR->SetNum(-2, m_D3D->GetDeviceContext());
			if (!result)
			{
				return false;
			}
			result = m_TextR->SetPoly(-2, m_D3D->GetDeviceContext());
			if (!result)
			{
				return false;
			}
			result = m_TextR->SetScreen(-1, record, m_D3D->GetDeviceContext());
			if (!result)
			{
				return false;
			}
			result = m_TextR->SetTimer(-3, m_D3D->GetDeviceContext());
			if (!result)
			{
				return false;
			}
			result = m_TextR->SetGhost(-3, m_D3D->GetDeviceContext());
			if (!result)
			{
				return false;
			}
		}
		result = RenderWin();
		break;
	case 3://패배화면
		m_Bitmap->textureChange(m_D3D->GetDevice(), L"./data/loseTemp.dds");
		
		result = RenderLose();
		break;
	default:
		break;
	}
	if (!result)
	{
		return false;
	}
	

	return true;
}

bool GraphicsClass::Render(float rotation)
{
	if (a == 0)
	{
		printf("ghost rand %d %d %d %d\n", ghostRand[0], ghostRand[1], ghostRand[2], ghostRand[3]);
		a++;
	}
	num_ghost = 4;
	num_grave = 16; grave_line = 4.0f; grave_term = 20.0f;
	num_house = 5;
	poly_total = m_Ghost->getPolygon() * num_ghost + m_grave->getPolygon() * num_grave
		+ m_Terr->getPolygon() + m_cross->getPolygon() + m_house->getPolygon() * num_house;
	num_total = num_ghost + num_grave + 1 + 1 + num_house;
	XMMATRIX worldMatrix, viewMatrix, projectionMatrix, orthoMatrix;
	bool result;
	XMMATRIX world_ghost[4], trans_ghost[4], rotate_ghost[4], scale_ghost[4];
	XMMATRIX ghost_orbit; XMMATRIX ghost_rotation;
	XMMATRIX cross_orbit; XMMATRIX cross_rotation; XMMATRIX cam_trans;
	XMMATRIX world_terr, trans_terr, rotate_terr, scale_terr;
	XMMATRIX world_cross, trans_cross, rotate_cross, scale_cross;
	XMMATRIX world_grave[16], trans_grave[16], rotate_grave[16], scale_grave[16];
	XMMATRIX world_house[5], trans_house[5], rotate_house[5], scale_house[5];

	XMMATRIX world_sky, trans_sky, rotate_sky, scale_sky;

	//XMMATRIX worldMatrix_skymap;

	XMMATRIX TextRotate;
	//XMMATRIX TextScale;
	TextRotate = XMMatrixRotationY(XMConvertToRadians(0.1));//3.141562f);
	//printf("text : %.1lf", rotation);
	//TextScale = XMMatrixScaling(1.5f, 1.2f, 1.0f);

	XMMATRIX world_parti[4], trans_parti[4], rotate_parti[4];// , scale_parti[4];
	XMMATRIX world_parti2, trans_parti2, rotate_parti2;// 십자가 거

	XMMATRIX m_ColliderInfo_[4]; XMVECTOR c_row_3_[4]; float c_m30_[4]; float c_m32_[4]; XMFLOAT3 cameraPosition;
	//*************************************************

	// Clear the buffers to begin the scene.
	m_D3D->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

	// Generate the view matrix based on the camera's position.
	m_Camera->Render(camYaw, camPitch, moveLeftRight, moveBackForward);
	//월드매트릭스 가져오는 부분
	{
		m_Camera->GetViewMatrix(viewMatrix);
		m_D3D->GetWorldMatrix(worldMatrix);
		//m_D3D->GetWorldMatrix(worldMatrix2);
		for (int i = 0; i < num_grave; i++)
			m_D3D->GetWorldMatrix(world_grave[i]);
		for (int i = 0; i < num_ghost; i++)
		{
			m_D3D->GetWorldMatrix(world_ghost[i]);
			m_D3D->GetWorldMatrix(world_parti[i]);
		}
		for (int i = 0; i < num_house; i++)
		{
			m_D3D->GetWorldMatrix(world_house[i]);
		}
		m_D3D->GetWorldMatrix(world_parti2);
		m_D3D->GetWorldMatrix(world_terr);
		m_D3D->GetWorldMatrix(world_sky);
		m_D3D->GetWorldMatrix(world_cross);
		//m_D3D->GetWorldMatrix(worldMatrix_skymap);
		m_D3D->GetProjectionMatrix(projectionMatrix);

		m_D3D->GetOrthoMatrix(orthoMatrix);
	}
	//스카이박스 부분
	{
		m_D3D->TurnOffCulling();

		//skydome
		scale_sky = XMMatrixScaling(1000.0f, 1000.0f, 1000.0f);
		rotate_sky = XMMatrixRotationY(0.0f);
		camPos = m_Camera->GetPosition();
		trans_sky = XMMatrixTranslation(camPos.x, camPos.y, camPos.z);

		world_sky = scale_sky * rotate_sky * trans_sky;
		m_D3D->TurnDSLessEqualOn();
		m_SkyMap->Render(m_D3D->GetDeviceContext(), world_sky, viewMatrix, projectionMatrix);
		m_D3D->TurnDSLessEqualOff();

		//sky
		/*

		m_SkyBox->Render(m_D3D->GetDeviceContext());
		result = m_TextureShader->Render(m_D3D->GetDeviceContext(), m_SkyBox->GetIndexCount(),
			world_sky, viewMatrix, projectionMatrix,
			m_SkyBox->GetTexture());
		if (!result)
		{
			return false;
		}*/


		m_D3D->TurnOnCulling();
	}

	//grave
	float grave_x, grave_y, grave_z;
	grave_y = -1.5f;
	for (int i = 0; i < num_grave; i++)
		scale_grave[i] = XMMatrixScaling(1.0f, 1.0f, 1.0f);

	for (int i = 0; i < num_grave; i++)
	{
		grave_x = (((i - i % (int)grave_line) / grave_line) * grave_term) - (((grave_line - 1.0f) / 2.0f) * grave_term);
		grave_z = ((i % (int)grave_line) * grave_term + grave_term);// -(((grave_line + 1.0f) / 2.0f) * grave_term);

		trans_grave[i] = XMMatrixTranslation(grave_x, grave_y, grave_z);
	}

	//gho
	ghost_orbit = XMMatrixTranslation(2.0f, 0.0f, 0.0f);
	ghost_rotation = XMMatrixRotationY(ghost_rotate);
	ghost_rotate += 0.05f;
	for (int i = 0; i < num_ghost; i++)
	{
		scale_ghost[i] = XMMatrixScaling(1.0f, 1.0f, 1.0f);
		rotate_ghost[i] = XMMatrixRotationY(0.0f);
		if (caught[i])
			trans_ghost[i] = XMMatrixTranslation(0.0f, -10.0f, 0.0f);
		else
			trans_ghost[i] = trans_grave[ghostRand[i]];
		trans_ghost[i] *= XMMatrixTranslation(0.0f, 2.0f, 0.0f);
	}

	//parti
	for (int i = 0; i < num_ghost; i++)
	{
		//scale_parti[i] = XMMatrixScaling(1.0f, 1.0f, 1.0f);
		rotate_parti[i] = XMMatrixRotationY(camYaw);
		trans_parti[i] = trans_grave[ghostRand[i]];
		trans_parti[i] *= XMMatrixTranslation(0.0f, 2.0f, 0.0f);
	}

	//terr
	scale_terr = XMMatrixScaling(1000.0f, 0.1f, 1000.0f);
	rotate_terr = XMMatrixRotationY(0.0f);
	trans_terr = XMMatrixTranslation(0.0f, -2.0f, 2.0f);

	//cross
	scale_cross = XMMatrixScaling(1.0f, 1.0f, 1.0f);
	rotate_cross = XMMatrixRotationY(cross_rotate);
	trans_cross = XMMatrixTranslation(crossX, cross_height, crossZ);
	cam_trans = XMMatrixTranslation(m_Camera->GetPosition().x, m_Camera->GetPosition().y, m_Camera->GetPosition().z);
	if (!equip)
	{
		cross_rotate += 0.05f;
		cross_height += crossH;
		if (cross_height > 1.5f)
			crossH *= -1;
		if (cross_height < 0.5f)
			crossH *= -1;
	}
	else
	{
		cross_rotate = XMConvertToRadians(3.141592);
		//cross_rotation = XMMatrixRotationY(camYaw+ 4.71238898038f);
		//cross_rotation *= XMMatrixRotationX(camPitch);
		cross_rotation = XMMatrixRotationRollPitchYaw(camPitch, camYaw, 0);
		cross_orbit = XMMatrixTranslation(1.0f, -0.750f, 2.5f);
		scale_cross = XMMatrixScaling(0.5f, 0.5f, 0.5f);
		rotate_cross *= XMMatrixRotationY(XMConvertToRadians(90.0f));
		rotate_cross *= XMMatrixRotationZ(XMConvertToRadians(30.0f));
	}

	//파티클2
	rotate_parti2 = XMMatrixRotationY(camYaw);
	trans_parti2 = trans_cross;
	trans_parti2 *= XMMatrixTranslation(0.0f, -2.0f, 0.0f);

	//집

	float house_y;
	house_y = 8.0f;
	trans_house[0] = XMMatrixTranslation(70.0f, house_y, 200.0f);
	trans_house[1] = XMMatrixTranslation(-135.0f, house_y, 50.0f);
	trans_house[2] = XMMatrixTranslation(-110.0f, house_y, 200.0f);
	trans_house[3] = XMMatrixTranslation(-20.0f, house_y, -80.0f);
	trans_house[4] = XMMatrixTranslation(95.0f, house_y, 50.0f);
	for (int i = 0; i < num_house; i++)
	{
		scale_house[i] = XMMatrixScaling(10.0f, 10.0f, 10.0f); rotate_house[i] = XMMatrixRotationY(0.0f);
	}

	// world, collider
	m_Camera->setCollider();
	//-고스트
	for (int i = 0; i < num_ghost; i++)
	{
		world_ghost[i] = scale_ghost[i] * rotate_ghost[i] * ghost_orbit * ghost_rotation * trans_ghost[i];
		m_Ghost->SetCollider(scale_ghost[i], ghost_orbit * ghost_rotation * trans_ghost[i]);
		isCollide = m_Camera->checkCollider(m_Ghost->GetMaxX(), m_Ghost->GetMinX(), m_Ghost->GetMaxY(),
			m_Ghost->GetMinY(), m_Ghost->GetMaxZ(), m_Ghost->GetMinZ());
		if (isCollide)
		{
			if (equip)
			{
				caught[i] = true;
				printf("고스트 콜라이더 %d\n",ghostRand[i]);
				timecalc[i] = m_time;
				sound = true;
			}
		}
	}
	//-그레이브
	for (int i = 0; i < num_grave; i++)
	{
		XMMATRIX m_ColliderInfo = trans_grave[i];
		XMVECTOR c_row_3 = m_ColliderInfo.r[3];
		float c_m30 = XMVectorGetX(c_row_3);
		float c_m32 = XMVectorGetZ(c_row_3);

		 //카메라 위치를 얻는다.
		XMFLOAT3 cameraPosition = m_Camera->GetPosition();
		 //아크 탄젠트 함수를 사용하여 현재 카메라 위치를 향하도록 빌보드 모델에 적용해야하는 회전을 계산합니다.
		double angle = atan2(c_m30 - cameraPosition.x, c_m32 - cameraPosition.z) * (180.0 / XM_PI);
		 //회전을 라디안으로 변환합니다.
		float rotation = (float)angle * 0.0174532925f;
		 //세계 행렬을 사용하여 원점에서 빌보드 회전을 설정합니다.
		rotate_grave[i] = XMMatrixRotationY(rotation);
		//world_grave[i] = XMMatrixRotationY(rotation);
		 //마지막으로 회전 및 변환 행렬을 결합하여 빌보드 모델의 최종 행렬을 만듭니다.
		//world_grave[i] = XMMatrixMultiply(world_grave[i], trans_grave[i]);

		world_grave[i] = scale_grave[i] * rotate_grave[i] * trans_grave[i];
		m_grave->SetCollider(scale_grave[i], trans_grave[i] * XMMatrixTranslation(0.0f, 2.0f, 0.0f));
		isCollide = m_Camera->checkCollider(m_grave->GetMaxX(), m_grave->GetMinX(), m_grave->GetMaxY(),
			m_grave->GetMinY(), m_grave->GetMaxZ(), m_grave->GetMinZ());
		if (isCollide)
		{
			//printf("현재 위치 : %.1lf, %.1lf\n상대 위치 : %.1lf, %.1lf\n",
			//	m_Camera->GetPosition().x, m_Camera->GetPosition().z,
			//	m_grave->GetMinX() + (m_grave->GetMaxX() - m_grave->GetMinX()) / 2.0f,
			//	m_grave->GetMinZ() + (m_grave->GetMaxZ() - m_grave->GetMinZ()) / 2.0f
			//);
			//if (tempX != 0 || tempZ != 0)
			//{
			//	printf("testX : %.1f\n", m_Camera->GetPosition().x - tempX);
			//	printf("testZ : %.1f\n", m_Camera->GetPosition().z - tempZ);
			//	printf("cam Yaw : %.5f\n", camYaw);
			//	
			//}
			//tempX = m_Camera->GetPosition().x;
			//tempZ = m_Camera->GetPosition().z;
			//printf("그레이브 콜라이더\n");
			moveBackForward = 0;
			moveLeftRight = 0;
		}
	}
	//-파티클
	for (int i = 0; i < num_ghost; i++)
	{
		cameraPosition = m_Camera->GetPosition();
		m_ColliderInfo_[i] = trans_grave[ghostRand[i]];
		c_row_3_[i] = m_ColliderInfo_[i].r[3];
		c_m30_[i] = XMVectorGetX(c_row_3_[i]);
		c_m32_[i] = XMVectorGetZ(c_row_3_[i]);
		world_parti[i] = rotate_parti[i] * trans_parti[i];
	}
	world_parti2 = rotate_parti2 * trans_parti2;
	world_terr = scale_terr * rotate_terr * trans_terr;
	//-십자가
	if (!equip)
		world_cross = scale_cross * rotate_cross * trans_cross;
	else
		world_cross = scale_cross * rotate_cross * cross_orbit * cross_rotation * cam_trans;
	if (!equip)
	{
		m_cross->SetCollider(scale_cross, trans_cross);
		isCollide = m_Camera->checkCollider(m_cross->GetMaxX()+0.5f, m_cross->GetMinX()-0.5f, m_cross->GetMaxY()+0.5f,
			m_cross->GetMinY()-0.5f, m_cross->GetMaxZ()+0.5f, m_cross->GetMinZ()-0.5f);
		
		if (isCollide)
		{
			equip = true;
			printf("장착됨\n");
			sound2 = true;
		}
	}
	//-집
	for (int i = 0; i < num_house; i++)
	{
		//XMMATRIX m_ColliderInfo = trans_house[i];
		//XMVECTOR c_row_3 = m_ColliderInfo.r[3];
		//float c_m30 = XMVectorGetX(c_row_3);
		//float c_m32 = XMVectorGetZ(c_row_3);
		//
		// //카메라 위치를 얻는다.
		//XMFLOAT3 cameraPosition = m_Camera->GetPosition();
		// //아크 탄젠트 함수를 사용하여 현재 카메라 위치를 향하도록 빌보드 모델에 적용해야하는 회전을 계산합니다.
		//double angle = atan2(c_m30 - cameraPosition.x, c_m32 - cameraPosition.z) * (180.0 / XM_PI);
		// //회전을 라디안으로 변환합니다.
		//float rotation = (float)angle * 0.0174532925f;
		// //세계 행렬을 사용하여 원점에서 빌보드 회전을 설정합니다.
		//rotate_house[i] = XMMatrixRotationY(rotation);
		////마지막으로 회전 및 변환 행렬을 결합하여 빌보드 모델의 최종 행렬을 만듭니다.
		//world_grave[i] = XMMatrixMultiply(world_grave[i], trans_grave[i]);

		world_house[i] = scale_house[i] * rotate_house[i] * trans_house[i];
		m_house->SetCollider(scale_house[i], trans_house[i]);
		isCollide = m_Camera->checkCollider(m_house->GetMaxX(), m_house->GetMinX()+7, m_house->GetMaxY(),
			m_house->GetMinY(), m_house->GetMaxZ()+11, m_house->GetMinZ()+7);
		if (isCollide)
		{
			moveBackForward = 0;
			moveLeftRight = 0;
		}
	}
	
	worldMatrix = TextRotate;

	//렌더
	m_Terr->Render(m_D3D->GetDeviceContext());
	result = m_LightShader->Render(m_D3D->GetDeviceContext(), m_Terr->GetIndexCount(),
		world_terr, viewMatrix, projectionMatrix,
		m_Terr->GetTexture(),
		m_Light->GetDirection(), m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(),
		m_Camera->GetPosition(), m_Light->GetSpecularColor(), m_Light->GetSpecularPower(), rotate_terr);
	if (!result)
	{
		return false;
	}

	//-집
	m_D3D->TurnOffCulling();
	for (int i = 0; i < num_house; i++)
	{
		m_house->Render(m_D3D->GetDeviceContext());

		result = m_LightShader->Render(m_D3D->GetDeviceContext(), m_house->GetIndexCount(),
			world_house[i], viewMatrix, projectionMatrix,
			m_house->GetTexture(),
			m_Light->GetDirection(), m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(),
			m_Camera->GetPosition(), m_Light->GetSpecularColor(), m_Light->GetSpecularPower(), rotate_house[i]);
		if (!result)
		{
			return false;
		}
	}
	m_D3D->TurnOnCulling();

	m_D3D->TurnZBufferOff();
	m_D3D->TurnOnAlphaBlending();
	m_ParticleSystem[1]->Render(m_D3D->GetDeviceContext());
	if (!equip)
	{
		if (!m_ParticleShader->Render(m_D3D->GetDeviceContext(), m_ParticleSystem[1]->GetIndexCount(), world_parti2, viewMatrix, projectionMatrix,
			m_ParticleSystem[1]->GetTexture()))
		{
			return false;
		}
	}
	m_D3D->TurnOffAlphaBlending();
	m_D3D->TurnZBufferOn();

	//-고스트
	for (int i = 0; i < num_ghost; i++)
	{
		if ((float)sqrt(pow(c_m30_[i] - cameraPosition.x, 2) + pow(c_m32_[i] - cameraPosition.z, 2)) < 10.0f)
		{
			m_Ghost->Render(m_D3D->GetDeviceContext());
			result = m_LightShader->Render(m_D3D->GetDeviceContext(), m_Ghost->GetIndexCount(),
				world_ghost[i], viewMatrix, projectionMatrix,
				m_Ghost->GetTexture(),
				m_Light->GetDirection(), *ambient, *diffuse,
				m_Camera->GetPosition(), *specular, specularP, rotate_ghost[i]);
			if (!result)
			{
				return false;
			}
		}
	}
	//-그레이브
	for (int i = 0; i < num_grave; i++)
	{
		m_grave->Render(m_D3D->GetDeviceContext());
		
		result = m_LightShader->Render(m_D3D->GetDeviceContext(), m_grave->GetIndexCount(),
			world_grave[i], viewMatrix, projectionMatrix,
			m_grave->GetTexture(),
			m_Light->GetDirection(), m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(),
			m_Camera->GetPosition(), m_Light->GetSpecularColor(), m_Light->GetSpecularPower(), rotate_grave[i]);
		if (!result)
		{
			return false;
		}
	}
	
	
	m_D3D->TurnZBufferOff();
	
	m_D3D->TurnOnAlphaBlending();
	result = m_Text->Render(m_D3D->GetDeviceContext(), worldMatrix, orthoMatrix);
	if (!result)
	{
		return false;
	}
	
	//성불파티클
	m_ParticleSystem[0]->Render(m_D3D->GetDeviceContext());
	for (int i = 0; i < num_ghost; i++)
	{
		if ((timecalc[i]!=0)&&timecalc[i] + 5 >= m_time)
		{
			if (!m_ParticleShader->Render(m_D3D->GetDeviceContext(), m_ParticleSystem[0]->GetIndexCount(), world_parti[i], viewMatrix, projectionMatrix,
				m_ParticleSystem[0]->GetTexture()))
			{
				return false;
			}
		}
	}
	
	m_D3D->TurnOffAlphaBlending();
	
	m_D3D->TurnZBufferOn();

	m_cross->Render(m_D3D->GetDeviceContext());
	result = m_LightShader->Render(m_D3D->GetDeviceContext(), m_cross->GetIndexCount(),
		world_cross, viewMatrix, projectionMatrix,
		m_cross->GetTexture(),
		m_Light->GetDirection(), *ambient, *diffuse,
		m_Camera->GetPosition(), *specular, specularP, rotate_cross);
	if (!result)
	{
		return false;
	}

	m_D3D->EndScene();

	return true;
}

bool GraphicsClass::RenderWin()
{
	XMMATRIX worldMatrix, viewMatrix, projectionMatrix, orthoMatrix;
	bool result;
	XMMATRIX TextRotate;
	XMMATRIX world_sky, trans_sky, rotate_sky, scale_sky;
	XMMATRIX world_ghost[4], trans_ghost[4], rotate_ghost[4], scale_ghost[4];
	XMMATRIX bitmapWorld, bitmapScale, bitmapRotate, bitmapTrans;
	
	
	TextRotate = XMMatrixRotationY(XMConvertToRadians(0.1));
	m_D3D->BeginScene(0.50f, 0.750f, 0.750f, 1.0f);
	
	m_Camera->SetPosition(camPosition);
	m_Camera->Render(camYaw, camPitch, moveLeftRight, moveBackForward);
	
	m_Camera->GetViewMatrix(viewMatrix);
	m_D3D->GetWorldMatrix(worldMatrix);
	for (size_t i = 0; i < 4; i++)
	{
		m_D3D->GetWorldMatrix(world_ghost[i]);
	}
	m_D3D->GetWorldMatrix(bitmapWorld);
	m_D3D->GetProjectionMatrix(projectionMatrix);
	m_D3D->GetOrthoMatrix(orthoMatrix);
	camYaw = 0.0f; camPitch = 0.0f;

	bitmapScale = XMMatrixScaling(2.5f, 2.5f, 1.0f);
	bitmapTrans=XMMatrixTranslation(0.0f, 0.0f, 5.0f);
	XMMATRIX m_ColliderInfo = bitmapTrans;
	XMVECTOR c_row_3 = m_ColliderInfo.r[3];
	float c_m30 = XMVectorGetX(c_row_3);
	float c_m32 = XMVectorGetZ(c_row_3);
	//카메라 위치를 얻는다.
	XMFLOAT3 cameraPosition = m_Camera->GetPosition();
	//아크 탄젠트 함수를 사용하여 현재 카메라 위치를 향하도록 빌보드 모델에 적용해야하는 회전을 계산합니다.
	double angle = atan2(c_m30 - cameraPosition.x, c_m32 - cameraPosition.z) * (180.0 / XM_PI);
	//회전을 라디안으로 변환합니다.
	float rotation = (float)angle * 0.0174532925f;
	//세계 행렬을 사용하여 원점에서 빌보드 회전을 설정합니다.
	bitmapRotate = XMMatrixRotationY(rotation);

	//스카이박스 부분
	
	{
		m_D3D->TurnOffCulling();

		//skydome
		scale_sky = XMMatrixScaling(1000.0f, 1000.0f, 1000.0f);
		rotate_sky = XMMatrixRotationY(5.8);
		rotate_sky *= XMMatrixRotationX(-6.0);
		camPos = m_Camera->GetPosition();
		trans_sky = XMMatrixTranslation(camPos.x, camPos.y, camPos.z);

		world_sky = scale_sky * rotate_sky * trans_sky;
		m_D3D->TurnDSLessEqualOn();
		m_SkyMap->Render(m_D3D->GetDeviceContext(), world_sky, viewMatrix, projectionMatrix);
		m_D3D->TurnDSLessEqualOff();

		m_D3D->TurnOnCulling();
	}

	ghost_rotate += 0.05f;
	gone_ghost_height += gone_ghost_H;
	if (gone_ghost_height > 3.0f)
		gone_ghost_H *= -1;
	if (gone_ghost_height < -3.0f)
		gone_ghost_H *= -1;
	printf("%.1f\n", gone_ghost_height);
	for (int i = 0; i < num_ghost; i++)
	{
		scale_ghost[i] = XMMatrixScaling(1.0f, 1.0f, 1.0f);
		rotate_ghost[i] = XMMatrixRotationY(ghost_rotate);
		rotate_ghost[i] *= XMMatrixRotationZ(-0.35);
		if (i == 0)
			trans_ghost[i] = XMMatrixTranslation(4 * 2 + (1.0f / 2.0f), gone_ghost_height + (-1 * 2), 30.0f);
		else
			trans_ghost[i] = XMMatrixTranslation(4 * i + (1.0f / 2.0f), gone_ghost_height + (i * 2), 30.0f);
		//trans_ghost[i] *= XMMatrixTranslation(0.0f, 2.0f, 0.0f);
	}
	
	worldMatrix = TextRotate;
	bitmapWorld = bitmapScale * bitmapRotate * bitmapTrans;
	for (int i = 0; i < num_ghost; i++)
	{
		world_ghost[i] = scale_ghost[i] * rotate_ghost[i] * trans_ghost[i];
		
	}
	m_D3D->TurnOffCulling();
	for (int i = 0; i < num_ghost; i++)
	{
		//if ((float)sqrt(pow(c_m30_[i] - cameraPosition.x, 2) + pow(c_m32_[i] - cameraPosition.z, 2)) < 10.0f)
		{
			m_Ghost->Render(m_D3D->GetDeviceContext());
			result = m_LightShader->Render(m_D3D->GetDeviceContext(), m_Ghost->GetIndexCount(),
				world_ghost[i], viewMatrix, projectionMatrix,
				m_Ghost->GetTexture(),
				m_Light->GetDirection(), m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(),
				m_Camera->GetPosition(), m_Light->GetSpecularColor(), m_Light->GetSpecularPower(), rotate_ghost[i]);
			if (!result)
			{
				return false;
			}
		}
	}
	m_D3D->TurnOnCulling();
	m_D3D->TurnZBufferOff();
	m_D3D->TurnOnAlphaBlending();
	result = m_Bitmap->Render(m_D3D->GetDeviceContext(), screenW / 2 - 175, screenH / 2 - 120);
	
	result = m_TextureShader->Render(m_D3D->GetDeviceContext(), m_Bitmap->GetIndexCount(), bitmapWorld, viewMatrix, orthoMatrix, m_Bitmap->GetTexture());
	if (!result)
	{
		return false;
	}
	result = m_TextR->Render(m_D3D->GetDeviceContext(), worldMatrix, orthoMatrix);
	if (!result)
	{
		return false;
	}
	m_D3D->TurnOffAlphaBlending();
	m_D3D->TurnZBufferOn();
	
	m_D3D->EndScene();

	return true;

}

bool GraphicsClass::RenderLose()
{
	XMMATRIX worldMatrix, viewMatrix, projectionMatrix, orthoMatrix;
	bool result;
	XMMATRIX TextRotate;
	XMMATRIX world_sky, trans_sky, rotate_sky, scale_sky;
	XMMATRIX world_ghost[4], trans_ghost[4], rotate_ghost[4], scale_ghost[4];
	XMMATRIX ghost_orbit; XMMATRIX ghost_rotation;
	XMMATRIX bitmapWorld, bitmapScale, bitmapRotate, bitmapTrans;

	TextRotate = XMMatrixRotationY(XMConvertToRadians(0.1));
	m_D3D->BeginScene(01.0f, 0.50f, 0.750f, 1.0f);
	camYaw = 0.0f; camPitch = 0.0f;
	m_Camera->SetPosition(camPosition);
	m_Camera->Render(camYaw, camPitch, moveLeftRight, moveBackForward);

	m_Camera->GetViewMatrix(viewMatrix);
	m_D3D->GetWorldMatrix(worldMatrix);
	for (size_t i = 0; i < 4; i++)
	{
		m_D3D->GetWorldMatrix(world_ghost[i]);
	}
	m_D3D->GetWorldMatrix(bitmapWorld);
	m_D3D->GetProjectionMatrix(projectionMatrix);
	m_D3D->GetOrthoMatrix(orthoMatrix);
	//스카이박스 부분
	{
		m_D3D->TurnOffCulling();

		//skydome
		scale_sky = XMMatrixScaling(1000.0f, 1000.0f, 1000.0f);
		rotate_sky = XMMatrixRotationY(5.8);
		rotate_sky *= XMMatrixRotationX(-6.0);
		camPos = m_Camera->GetPosition();
		trans_sky = XMMatrixTranslation(camPos.x, camPos.y, camPos.z);

		world_sky = scale_sky * rotate_sky * trans_sky;
		m_D3D->TurnDSLessEqualOn();
		m_SkyMap->Render(m_D3D->GetDeviceContext(), world_sky, viewMatrix, projectionMatrix);
		m_D3D->TurnDSLessEqualOff();

		m_D3D->TurnOnCulling();
	}

	m_Camera->GetViewMatrix(viewMatrix);
	m_D3D->GetWorldMatrix(worldMatrix);
	m_D3D->GetProjectionMatrix(projectionMatrix);
	m_D3D->GetOrthoMatrix(orthoMatrix);

	bitmapScale = XMMatrixScaling(2.5f, 2.5f, 1.0f);
	bitmapTrans = XMMatrixTranslation(0.0f, 0.0f, 5.0f);
	XMMATRIX m_ColliderInfo = bitmapTrans;
	XMVECTOR c_row_3 = m_ColliderInfo.r[3];
	float c_m30 = XMVectorGetX(c_row_3);
	float c_m32 = XMVectorGetZ(c_row_3);
	//카메라 위치를 얻는다.
	XMFLOAT3 cameraPosition = m_Camera->GetPosition();
	//아크 탄젠트 함수를 사용하여 현재 카메라 위치를 향하도록 빌보드 모델에 적용해야하는 회전을 계산합니다.
	double angle = atan2(c_m30 - cameraPosition.x, c_m32 - cameraPosition.z) * (180.0 / XM_PI);
	//회전을 라디안으로 변환합니다.
	float rotation = (float)angle * 0.0174532925f;
	//세계 행렬을 사용하여 원점에서 빌보드 회전을 설정합니다.
	bitmapRotate = XMMatrixRotationY(rotation);

	bitmapWorld = bitmapScale * bitmapRotate * bitmapTrans;

	ghost_orbit = XMMatrixTranslation(7.5f, 0.0f, 0.0f);
	
	ghost_rotate += 0.01f;
	gone_ghost_height += gone_ghost_H;
	if (gone_ghost_height > 1.0f)
		gone_ghost_H *= -1;
	if (gone_ghost_height < 0.0f)
		gone_ghost_H *= -1;
	for (int i = 0; i < num_ghost; i++)
	{
		scale_ghost[i] = XMMatrixScaling(1.0f, 1.0f, 1.0f);
		rotate_ghost[i] = XMMatrixRotationY(ghost_rotate);
		//rotate_ghost[i] *= XMMatrixRotationZ();
		trans_ghost[i] = XMMatrixTranslation(camPos.x, camPos.y, camPos.z);
		trans_ghost[i] *= XMMatrixTranslation(0.0f, gone_ghost_height, 0.0f);
	}

	worldMatrix = TextRotate;
	for (int i = 0; i < num_ghost; i++)
	{
		ghost_rotation = XMMatrixRotationY(ghost_rotate+(i*1.5));
		world_ghost[i] = scale_ghost[i] * rotate_ghost[i] * ghost_orbit * ghost_rotation * trans_ghost[i];

	}
	m_D3D->TurnOffCulling();
	for (int i = 0; i < num_ghost; i++)
	{
		//if ((float)sqrt(pow(c_m30_[i] - cameraPosition.x, 2) + pow(c_m32_[i] - cameraPosition.z, 2)) < 10.0f)
		{
			m_Ghost->Render(m_D3D->GetDeviceContext());
			result = m_LightShader->Render(m_D3D->GetDeviceContext(), m_Ghost->GetIndexCount(),
				world_ghost[i], viewMatrix, projectionMatrix,
				m_Ghost->GetTexture(),
				m_Light->GetDirection(), m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(),
				m_Camera->GetPosition(), m_Light->GetSpecularColor(), m_Light->GetSpecularPower(), rotate_ghost[i]);
			if (!result)
			{
				return false;
			}
		}
	}
	m_D3D->TurnOnCulling();

	m_D3D->TurnZBufferOff();
	m_D3D->TurnOnAlphaBlending();
	result = m_Bitmap->Render(m_D3D->GetDeviceContext(), screenW / 2 - 175, screenH / 2 - 150);
	result = m_TextureShader->Render(m_D3D->GetDeviceContext(), m_Bitmap->GetIndexCount(), bitmapWorld, viewMatrix, orthoMatrix, m_Bitmap->GetTexture());
	if (!result)
	{
		return false;
	}
	/*result = m_TextR->Render(m_D3D->GetDeviceContext(), worldMatrix, orthoMatrix);
	if (!result)
	{
		return false;
	}*/
	m_D3D->TurnOffAlphaBlending();
	m_D3D->TurnZBufferOn();

	m_D3D->EndScene();

	return true;

}

bool GraphicsClass::RenderTitle()
{
	XMMATRIX worldMatrix, viewMatrix, projectionMatrix, orthoMatrix;
	bool result;
	XMMATRIX bitmapScale,bitmapRotate, bitmapTrans;
	XMMATRIX world_sky, trans_sky, rotate_sky, scale_sky;
	/*temp -= 0.5f;
	if (temp < -300)
		temp += 100.0f;
	printf("temp : %.1f\n", temp);*/
	bitmapScale = XMMatrixScaling(3.0f, 2.0f, 1.0f);
	bitmapTrans = XMMatrixTranslation(0.0f, 0.0f ,0.0f);
	
	XMMATRIX m_ColliderInfo = bitmapTrans;
	XMVECTOR c_row_3 = m_ColliderInfo.r[3];
	float c_m30 = XMVectorGetX(c_row_3);
	float c_m32 = XMVectorGetZ(c_row_3);
	//카메라 위치를 얻는다.
	XMFLOAT3 cameraPosition = m_Camera->GetPosition();
	//아크 탄젠트 함수를 사용하여 현재 카메라 위치를 향하도록 빌보드 모델에 적용해야하는 회전을 계산합니다.
	double angle = atan2(c_m30 - cameraPosition.x, c_m32 - cameraPosition.z) * (180.0 / XM_PI);
	//회전을 라디안으로 변환합니다.
	float rotation = (float)angle * 0.0174532925f;
	//세계 행렬을 사용하여 원점에서 빌보드 회전을 설정합니다.
	bitmapRotate = XMMatrixRotationY(rotation);
	//bitmapRotate = XMMatrixRotationY(XMConvertToRadians(0.0f));

	m_D3D->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);
	//camYaw = 6.9f; camPitch = -0.3f;
	camPosition = XMVectorSet(0.0f, 2.0f, -10.0f, 0.0f);
	m_Camera->SetPosition(camPosition);
	m_Camera->Render(camYaw, camPitch, moveLeftRight, moveBackForward);

	m_Camera->GetViewMatrix(viewMatrix);
	m_D3D->GetWorldMatrix(worldMatrix);
	m_D3D->GetProjectionMatrix(projectionMatrix);
	m_D3D->GetOrthoMatrix(orthoMatrix);
	//스카이박스 부분
	
	//{
	//	m_D3D->TurnOffCulling();
	//
	//	//skydome
	//	scale_sky = XMMatrixScaling(1000.0f, 1000.0f, 1000.0f);
	//	rotate_sky = XMMatrixRotationY(0.0f);
	//	camPos = m_Camera->GetPosition();
	//	trans_sky = XMMatrixTranslation(camPos.x, camPos.y, camPos.z);
	//
	//	world_sky = scale_sky * rotate_sky * trans_sky;
	//	m_D3D->TurnDSLessEqualOn();
	//	m_SkyMap->Render(m_D3D->GetDeviceContext(), world_sky, viewMatrix, projectionMatrix);
	//	m_D3D->TurnDSLessEqualOff();
	//
	//	m_D3D->TurnOnCulling();
	//}

	m_Camera->GetViewMatrix(viewMatrix);
	m_D3D->GetWorldMatrix(worldMatrix);
	m_D3D->GetProjectionMatrix(projectionMatrix);
	m_D3D->GetOrthoMatrix(orthoMatrix);

	worldMatrix = bitmapScale* bitmapRotate;
	
	
	m_D3D->TurnZBufferOff();
	m_D3D->TurnOnAlphaBlending();
	//result = m_TextR->Render(m_D3D->GetDeviceContext(), worldMatrix, orthoMatrix);
	//if (!result)
	//{
	//	return false;
	//}
	result = m_Bitmap->Render(m_D3D->GetDeviceContext(), screenW / 2-120, screenH / 2 - 150);//screenW / 2 - 100, screenH / 2 - 100);
	result = m_TextureShader->Render(m_D3D->GetDeviceContext(), m_Bitmap->GetIndexCount(), worldMatrix, viewMatrix, orthoMatrix, m_Bitmap->GetTexture());
	m_D3D->TurnOffAlphaBlending();
	m_D3D->TurnZBufferOn();

	m_D3D->EndScene();

	return true;

}

void GraphicsClass::initData()
{
	srand(time(NULL));
	crossX = rand() % 50 - 25;
	crossZ = rand() % 50 + 1; //(((float)rand() - (float)rand()) / RAND_MAX) + 30.0f;
	for (int i = 0; i < 4; i++)
	{
		caught[i] = false;
		timecalc[i] = 0.0f;
		ghostRand[i] = rand() % 16;
		for (int j = 0; j < i; j++)
		{
			if (ghostRand[i] == ghostRand[j])
				i--;
		}
	}
	a = 0;
	camPosition = XMVectorSet(0.0f, 2.0f, 0.0f, 0.0f);
	m_Camera->SetPosition(camPosition);	// for cube
	camYaw = 0.0f; camPitch = 0.0f;
	m_Camera->Render(camYaw, camPitch, moveLeftRight, moveBackForward);
	
	m_time = -1;

	equip = false;

	gone_ghost_height = 0.0f; gone_ghost_H = 0.1f;
}