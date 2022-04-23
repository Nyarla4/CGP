////////////////////////////////////////////////////////////////////////////////
// Filename: systemclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "systemclass.h"


SystemClass::SystemClass()
{
	m_Input = 0;
	m_Graphics = 0;
	m_Fps = 0;
	m_Cpu = 0;
	m_Timer = 0;
	scene = 0;
	moveChk = true;
}


SystemClass::SystemClass(const SystemClass& other)
{
}


SystemClass::~SystemClass()
{
}


bool SystemClass::Initialize()
{
	int screenWidth, screenHeight;
	bool result;


	screenWidth = 0;
	screenHeight = 0;

	InitializeWindows(screenWidth, screenHeight);

	InitDirectInput();

	m_Input = new InputClass;
	if(!m_Input)
	{
		return false;
	}

	m_Input->Initialize(m_hinstance, m_hwnd, screenWidth, screenHeight);

	m_Graphics = new GraphicsClass;
	if(!m_Graphics)
	{
		return false;
	}

	// Initialize the graphics object.
	result = m_Graphics->Initialize(screenWidth, screenHeight, m_hwnd);
	if(!result)
	{
		return false;
	}
	
	// Create the fps object.
	m_Fps = new FpsClass;
	if (!m_Fps)
	{
		return false;
	}

	// Initialize the fps object.
	m_Fps->Initialize();
	// Create the cpu object.
	m_Cpu = new CpuClass;
	if (!m_Cpu)
	{
		return false;
	}

	// Initialize the cpu object.
	m_Cpu->Initialize();
	// Create the timer object.
	m_Timer = new TimerClass;
	if (!m_Timer)
	{
		return false;
	}

	// Initialize the timer object.
	result = m_Timer->Initialize();
	if (!result)
	{
		MessageBox(m_hwnd, L"Could not initialize the Timer object.", L"Error", MB_OK);
		return false;
	}

	m_Sound = new SoundClass;
	if (!m_Sound)
	{
		return false;
	}

	result = m_Sound->Initialize(m_hwnd, (CHAR*)"./data/sound/bgm.wav", (CHAR*)"./data/sound/SE.wav", (CHAR*)"./data/sound/get.wav");// , (CHAR*)"./models/Sound/win.wav", (CHAR*)"./models/Sound/lose.wav");
	if (!result)
	{
		MessageBox(m_hwnd, L"Could not initialize Direct Sound.", L"Error", MB_OK);
		return false;
	}

	scene = -1;

	return true;
}


void SystemClass::Shutdown()
{
	// Release the graphics object.
	if(m_Graphics)
	{
		m_Graphics->Shutdown();
		delete m_Graphics;
		m_Graphics = 0;
	}

	// Release the input object.
	if(m_Input)
	{
		delete m_Input;
		m_Input = 0;
	}

	// Release the timer object.
	if (m_Timer)
	{
		delete m_Timer;
		m_Timer = 0;
	}

	// Release the cpu object.
	if (m_Cpu)
	{
		m_Cpu->Shutdown();
		delete m_Cpu;
		m_Cpu = 0;
	}

	// Release the fps object.
	if (m_Fps)
	{
		delete m_Fps;
		m_Fps = 0;
	}

	if (m_Sound)
	{
		m_Sound->Shutdown();
		delete m_Sound;
		m_Sound = 0;
	}

	// Shutdown the window.
	ShutdownWindows();
	
	return;
}


void SystemClass::Run()
{
	MSG msg;
	bool done, result;


	// Initialize the message structure.
	ZeroMemory(&msg, sizeof(MSG));
	
	// Loop until there is a quit message from the window or the user.
	done = false;
	//m_Sound->playBGM();
	while(!done)
	{
		// Handle the windows messages.
		if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		// If windows signals to end the application then exit out.
		if(msg.message == WM_QUIT)
		{
			done = true;
		}
		else
		{
			// Otherwise do the frame processing.
			result = Frame();
			if(!result)
			{
				MessageBox(m_hwnd, L"Frame Processing Failed", L"Error", MB_OK);
				done = true;
			}
		}
		if (m_Input->IsEscapePressed() == true)
		{
			done = true;
		}
	}

	return;
}


bool SystemClass::Frame()
{
	bool result;
	int mouseX, mouseY;
	m_Timer->Frame();
	m_Fps->Frame();
	m_Cpu->Frame();
	DetectInput();
	// Check if the user pressed escape and wants to exit the application.
	result = m_Input->Frame();
	if (!result) {
		return false;
	}
	m_Input->GetMouseLocation(mouseX, mouseY);
	if (m_Graphics->sound)
	{
		m_Sound->playSE();
		m_Graphics->sound = false;
	}
	if (m_Graphics->sound2)
	{
		m_Sound->playSE2();
		m_Graphics->sound2 = false;
	}
	if (m_Graphics->sceneWchange)
	{
		scene = 2;
		moveChk = false;
		m_Sound->stopFile(1);
		m_Sound->stopFile(2);
		m_Sound->stopFile(5);
		m_Graphics->sceneWchange = false;
	}
	if (m_Graphics->sceneLchange)
	{
		scene = 3;
		moveChk = false;
		m_Sound->stopFile(1);
		m_Sound->stopFile(2);
		m_Sound->stopFile(5);
		m_Graphics->sceneLchange = false;
	}
	if (scene == -1)
	{
		m_Sound->stopFile(1);
	}
	if (scene == 1)
	{
		m_Sound->stopFile(1);
	}
	// Do the frame processing for the graphics object.
	result = m_Graphics->Frame(m_Fps->GetFps(), m_Cpu->GetCpuPercentage(), m_Timer->GetTime(), scene);
	if (!result)
	{
		return false;
	}

	return true;
}


LRESULT CALLBACK SystemClass::MessageHandler(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam)
{
	return DefWindowProc(hwnd, umsg, wparam, lparam);
}

void SystemClass::InitializeWindows(int& screenWidth, int& screenHeight)
{
	WNDCLASSEX wc;
	DEVMODE dmScreenSettings;
	int posX, posY;


	// Get an external pointer to this object.	
	ApplicationHandle = this;

	// Get the instance of this application.
	m_hinstance = GetModuleHandle(NULL);

	// Give the application a name.
	m_applicationName = L"Engine";

	// Setup the windows class with default settings.
	wc.style         = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc   = WndProc;
	wc.cbClsExtra    = 0;
	wc.cbWndExtra    = 0;
	wc.hInstance     = m_hinstance;
	wc.hIcon		 = LoadIcon(NULL, IDI_WINLOGO);
	wc.hIconSm       = wc.hIcon;
	wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.lpszMenuName  = NULL;
	wc.lpszClassName = m_applicationName;
	wc.cbSize        = sizeof(WNDCLASSEX);
	
	// Register the window class.
	RegisterClassEx(&wc);

	// Determine the resolution of the clients desktop screen.
	screenWidth  = GetSystemMetrics(SM_CXSCREEN);
	screenHeight = GetSystemMetrics(SM_CYSCREEN);

	// Setup the screen settings depending on whether it is running in full screen or in windowed mode.
	if(FULL_SCREEN)
	{
		// If full screen set the screen to maximum size of the users desktop and 32bit.
		memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));
		dmScreenSettings.dmSize       = sizeof(dmScreenSettings);
		dmScreenSettings.dmPelsWidth  = (unsigned long)screenWidth;
		dmScreenSettings.dmPelsHeight = (unsigned long)screenHeight;
		dmScreenSettings.dmBitsPerPel = 32;			
		dmScreenSettings.dmFields     = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		// Change the display settings to full screen.
		ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN);

		// Set the position of the window to the top left corner.
		posX = posY = 0;
	}
	else
	{
		// If windowed then set it to 800x600 resolution.
		screenWidth  = 800;
		screenHeight = 600;

		// Place the window in the middle of the screen.
		posX = (GetSystemMetrics(SM_CXSCREEN) - screenWidth)  / 2;
		posY = (GetSystemMetrics(SM_CYSCREEN) - screenHeight) / 2;
	}

	// Create the window with the screen settings and get the handle to it.
	m_hwnd = CreateWindowEx(WS_EX_APPWINDOW, m_applicationName, m_applicationName, 
						    WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP,
						    posX, posY, screenWidth, screenHeight, NULL, NULL, m_hinstance, NULL);

	// Bring the window up on the screen and set it as main focus.
	ShowWindow(m_hwnd, SW_SHOW);
	SetForegroundWindow(m_hwnd);
	SetFocus(m_hwnd);

	// Hide the mouse cursor.
	ShowCursor(false);

	return;
}

void SystemClass::ShutdownWindows()
{
	// Show the mouse cursor.
	ShowCursor(true);

	// Fix the display settings if leaving full screen mode.
	if(FULL_SCREEN)
	{
		ChangeDisplaySettings(NULL, 0);
	}

	// Remove the window.
	DestroyWindow(m_hwnd);
	m_hwnd = NULL;

	// Remove the application instance.
	UnregisterClass(m_applicationName, m_hinstance);
	m_hinstance = NULL;

	// Release the pointer to this class.
	ApplicationHandle = NULL;

	return;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam)
{
	switch(umessage)
	{
		// Check if the window is being destroyed.
		case WM_DESTROY:
		{
			PostQuitMessage(0);
			return 0;
		}

		// Check if the window is being closed.
		case WM_CLOSE:
		{
			PostQuitMessage(0);		
			return 0;
		}

		// All other messages pass to the message handler in the system class.
		default:
		{
			return ApplicationHandle->MessageHandler(hwnd, umessage, wparam, lparam);
		}
	}
}

bool SystemClass::InitDirectInput()
{
	DirectInput8Create(m_hinstance,
		DIRECTINPUT_VERSION,
		IID_IDirectInput8,
		(void**)&DirectInput,
		NULL);

	DirectInput->CreateDevice(GUID_SysKeyboard,
		&DIKeyboard,
		NULL);

	DirectInput->CreateDevice(GUID_SysMouse,
		&DIMouse,
		NULL);

	DIKeyboard->SetDataFormat(&c_dfDIKeyboard);
	DIKeyboard->SetCooperativeLevel(m_hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);

	DIMouse->SetDataFormat(&c_dfDIMouse);
	DIMouse->SetCooperativeLevel(m_hwnd, DISCL_EXCLUSIVE | DISCL_NOWINKEY | DISCL_FOREGROUND);

	return true;
}

void SystemClass::DetectInput()
{
	DIMOUSESTATE mouseCurrState;

	BYTE keyboardState[256];

	DIKeyboard->Acquire();
	DIMouse->Acquire();

	DIMouse->GetDeviceState(sizeof(DIMOUSESTATE), &mouseCurrState);

	DIKeyboard->GetDeviceState(sizeof(keyboardState), (LPVOID)&keyboardState);

	float speed = 0.3f;
	if (moveChk&&!(scene==-1))
	{
		if ((keyboardState[DIK_A] & 0x80) && onceKeyDowned2 == false)
		{
			m_Graphics->moveLeftRight = -speed;
			onceKeyDowned2 = true;
		}
		if ((keyboardState[DIK_D] & 0x80) && onceKeyDowned2 == false)
		{
			m_Graphics->moveLeftRight = speed;
			onceKeyDowned2 = true;
		}
		if (!(keyboardState[DIK_A] & 0x80) && !(keyboardState[DIK_D] & 0x80) && onceKeyDowned2 == true)
		{
			m_Graphics->moveLeftRight = 0;
			onceKeyDowned2 = false;
		}
		if ((keyboardState[DIK_W] & 0x80) && onceKeyDowned == false)
		{
			m_Graphics->moveBackForward = speed;
			onceKeyDowned = true;
		}
		if ((keyboardState[DIK_S] & 0x80) && onceKeyDowned == false)
		{
			m_Graphics->moveBackForward = -speed;
			onceKeyDowned = true;
		}
		if (!(keyboardState[DIK_W] & 0x80) && !(keyboardState[DIK_S] & 0x80) && onceKeyDowned == true)
		{
			m_Graphics->moveBackForward = 0;
			onceKeyDowned = false;
		}
		if ((mouseCurrState.lX != mouseLastState.lX) || (mouseCurrState.lY != mouseLastState.lY))
		{
			m_Graphics->camYaw += mouseLastState.lX * 0.001f;
			//if (m_Graphics->camYaw > 6.3f)
			//	m_Graphics->camYaw -= 6.3f;
			//if (m_Graphics->camYaw < -6.3f)
			//	m_Graphics->camYaw += 6.3f;
			m_Graphics->camPitch += mouseCurrState.lY * 0.001f;

			mouseLastState = mouseCurrState;
		}
	}
	if (scene == -1)
	{
		if (keyboardState[DIK_SPACE])
		{
			scene = 1;
			initData();
			moveChk = true;
		}
	}
	else
	{
		if (keyboardState[DIK_R] && resetChk == false)
		{
			scene = 1;
			initData();
			resetChk = true;
			moveChk = true;
		}
		if ((!keyboardState[DIK_R]) && resetChk == true)
		{
			resetChk = false;
		}
	}
	if (scene == 2 || scene == 3)
	{
		if (keyboardState[DIK_Q])
		{
			scene = -1;
			initData();
		}
	}

	return;
}

void SystemClass::initData()
{
	m_Graphics->initData();
	m_Sound->stopFile(1);
	m_Sound->stopFile(2);
	m_Sound->stopFile(5);
	m_Sound->playBGM();
}