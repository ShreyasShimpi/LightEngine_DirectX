#include "Engine.h"
#include "RenderWindow.h"

/*
	This is used to redirect the messages from the winProc function in Engine class.
*/
LRESULT CALLBACK RedirectWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_CLOSE: // window close message
		DestroyWindow(hwnd);
		return 0;

	default:
	{
		// retrieve ptr to engine class
		CEngine* const engine = reinterpret_cast<CEngine*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));

		// forward message to engine class handler
		return engine->HandleMessages(hwnd, uMsg, wParam, lParam);
	}
	}
}

/*
	This function we will pass when we create a window.
	The purpose of this function is when we create the window, it should bring back the
	the engine pointer that we passed in create window. and call the winProc function inside engine.
	
*/
LRESULT CALLBACK SetupWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_NCCREATE: // called when window is created
	{
		const CREATESTRUCTW* const structure = reinterpret_cast<CREATESTRUCTW*>(lParam);
		
		// create the engine pointer to store the data back from window 
		CEngine* engine = reinterpret_cast<CEngine*>(structure->lpCreateParams);

		// nullptr check
		if (engine == nullptr)
		{
			MessageBoxW(NULL, L"ERROR : Engine Pointer is Null", L"ERROR", MB_ICONERROR);
			exit(-1);
		}

		SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(engine));
		SetWindowLongPtr(hwnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(RedirectWindowProc));
		return engine->HandleMessages(hwnd, uMsg, wParam, lParam);
	}
	default:
		return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}
}


/*
	RENDER WINDOW DEFINIATIONS
*/
CRenderWindow::CRenderWindow()
{
	m_windowHandle = NULL;
	m_appInstanceHandle = NULL;

	m_windowTitleW = L""; // blank initially
	m_windowClassNameW = L""; // blank initially

	m_width = 0;
	m_height = 0;
}

CRenderWindow::~CRenderWindow()
{
	if (m_windowHandle != NULL)
	{
		UnregisterClassW(m_windowClassNameW.c_str(), m_appInstanceHandle);
		DestroyWindow(m_windowHandle);
	}
}

void CRenderWindow::RegisterWindowClass()
{
	WNDCLASSEXW wc = { 0 };
	wc.cbSize = sizeof(wc);
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc = &SetupWindowProc; // setup function to handle windows messages
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = m_appInstanceHandle; // hInstance
	wc.hIcon = nullptr;
	wc.hCursor = LoadCursor(nullptr, IDC_ARROW); // default cursor
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW); // background 
	wc.lpszMenuName = MAKEINTRESOURCEW(IDC_RESEARCHPROJECTLIGHTENGINE);
	wc.lpszClassName = m_windowClassNameW.c_str();
	wc.hIconSm = LoadIcon(wc.hInstance, MAKEINTRESOURCE(IDI_SMALL)); // default icon

	assert(RegisterClassExW(&wc));
}

bool CRenderWindow::Initialize(CEngine* engine, int width, int height, HINSTANCE hInstance, std::wstring windowTitle, std::wstring windowClassName)
{
	// initialize variables
	m_appInstanceHandle = hInstance;
	m_windowTitleW = windowTitle;
	m_windowClassNameW = windowClassName;
	m_width = width;
	m_height = height;

	// register window class
	RegisterWindowClass();

	// this is required to adjust the width and height of the actual content in the window
	// excluding the bars around the window

	RECT WindowRect;
	WindowRect.left = 100;
	WindowRect.right = width + WindowRect.left;
	WindowRect.top = 100;
	WindowRect.bottom = height + WindowRect.top;
	AdjustWindowRect(&WindowRect, WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU, FALSE);

	// create window & get hWnd
	m_windowHandle = CreateWindowW(
		m_windowClassNameW.c_str(),
		m_windowTitleW.c_str(),
		WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU,
		WindowRect.left,
		WindowRect.top,
		width,
		height,
		nullptr,
		nullptr,
		m_appInstanceHandle,
		engine // param for creating window
	);

	// null check
	if(m_windowHandle == NULL)
	{
		MessageBoxW(NULL, L"ERROR : WINDOW HANDLE IS NULL", L"ERROR", MB_ICONERROR);
		return false;
	}

	// show window
	ShowWindow(m_windowHandle, SW_SHOW);
	SetForegroundWindow(m_windowHandle);
	SetFocus(m_windowHandle);

	return true;
}

bool CRenderWindow::ProcessMessages()
{
	// Handle the windows messages.
	MSG msg;
	ZeroMemory(&msg, sizeof(MSG)); // Initialize the message structure.

	while (PeekMessage(&msg, m_windowHandle, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&msg); //Translate message from virtual key messages into character messages so we can dispatch the message
		DispatchMessage(&msg); //Dispatch message to our WindowProc for this window.
	}
	
	// Check if the window was closed
	if (msg.message == WM_NULL)
	{
		if (!IsWindow(m_windowHandle))
		{
			m_windowHandle = NULL; //Message processing loop takes care of destroying this window
			UnregisterClassW(m_windowClassNameW.c_str(), m_appInstanceHandle);
			return false;
		}
	}

	return true;
}

void CRenderWindow::SetTitleWindow(const std::string& title)
{
	// fail check
	if (!SetWindowTextA(m_windowHandle, title.c_str()))
	{
		return;
	}
}

