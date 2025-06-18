#include "Engine.h"
#include "windowsx.h";
#include "RenderWindow.h"
#include "Graphics.h"
#include "Data.h"

CEngine::CEngine()
{
	m_keyPressed = '.'; // dummy key for keyup message : no key pressed
}

CEngine::~CEngine()
{
}

bool CEngine::Initialize(HINSTANCE hInstance, std::wstring window_title, std::wstring window_class, int width, int height)
{
	// default constructed
	m_renderWindow = std::make_unique<CRenderWindow>();

	if (!m_renderWindow->Initialize(this, width, height, hInstance, window_title, window_class))
	{
		return false;
	}

	// graphics initiated
	m_graphics = std::make_unique<CGraphics>();

	// initialize
	m_graphics->InitializeGraphics(m_renderWindow->GetWindowHandle());

	return true;
}

bool CEngine::ProcessMessages()
{
	return m_renderWindow->ProcessMessages();
}

void CEngine::Update()
{
	// will be used for key inputs
	m_time.Tick();

	// FPS TITLE
	short fps = 1000.0f / m_time.DeltaTime();
	std::string title = "FPS : " + std::to_string(fps);
	m_renderWindow->SetTitleWindow(title);

	// camera movement
	float cameraSpeed = g_cameraMoveSpeed * m_time.DeltaTime();


	switch (m_keyPressed)
	{
	case 'W': // forward
	{

		DirectX::XMFLOAT3 currentDir;
		DirectX::XMStoreFloat3(&currentDir, m_graphics->GetCamera().GetCurrentForward());

		m_graphics->GetCamera().OffsetCameraPosition(
			currentDir.x * cameraSpeed,
			currentDir.y * cameraSpeed,
			currentDir.z * cameraSpeed);
		break;
	}
	case 'S': // backward
	{
		DirectX::XMFLOAT3 currentDir;
		DirectX::XMStoreFloat3(&currentDir, m_graphics->GetCamera().GetCurrentForward());

		m_graphics->GetCamera().OffsetCameraPosition(
			-currentDir.x * cameraSpeed,
			-currentDir.y * cameraSpeed,
			-currentDir.z * cameraSpeed);
		break;
	}
	case 'A': // left
	{
		DirectX::XMFLOAT3 currentDir;
		DirectX::XMStoreFloat3(&currentDir, m_graphics->GetCamera().GetCurrentLeft());

		m_graphics->GetCamera().OffsetCameraPosition(
			currentDir.x * cameraSpeed,
			currentDir.y * cameraSpeed,
			currentDir.z * cameraSpeed);
		break;
	}
	case 'D': // right
	{
		DirectX::XMFLOAT3 currentDir;
		DirectX::XMStoreFloat3(&currentDir, m_graphics->GetCamera().GetCurrentLeft());

		m_graphics->GetCamera().OffsetCameraPosition(
			-currentDir.x * cameraSpeed,
			-currentDir.y * cameraSpeed,
			-currentDir.z * cameraSpeed);
		break;
	}
	case VK_SPACE: // up
	{
		m_graphics->GetCamera().OffsetCameraPosition(0.0f, cameraSpeed, 0.0f);
		break;
	}
	case VK_CONTROL: // down
	{
		m_graphics->GetCamera().OffsetCameraPosition(0.0f, -cameraSpeed, 0.0f);
		break;
	}
	case '.': // dummy key for keyup message : no key pressed
	{
		break;
	}
	default:
	{
		break;
	}
	}
}

void CEngine::RenderFrame()
{
	m_graphics->ClearFrame(0.2f, 0.2f, 0.2f);
	m_graphics->Update();
	m_graphics->Render();
}


/*
	This function is defined in ImGui.
	we need to get this function as extern so that we can call it in our winProc function
	and can interact with imgui.
*/
extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

/*
	This winProc function will handle our keyboard and mouse calls.
	This function is redirected from the Handle.
	This contained handle for Imgui to interact.
*/
LRESULT CEngine::HandleMessages(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	LRESULT result = 0;

	if (ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam))
	{
		return result;
	}

	static int mouseX = 0.0f;
	static int mouseY = 0.0f;
	static bool rightMouseDown = false;

	switch (uMsg)
	{

	case WM_KEYDOWN:
	{

		unsigned char keycode = static_cast<unsigned char>(wParam);
		if (keycode == VK_ESCAPE) // escape key : close the window
		{
			DestroyWindow(hWnd);
			break;
		}

		// store currently pressed key
		m_keyPressed = keycode;
		break;
	}
	case WM_KEYUP:
	{
		m_keyPressed = '.';// dummy key for keyup message : no key pressed
		break;
	}
	case WM_RBUTTONDOWN:
	{
		rightMouseDown = true;
		break;
	}
	case WM_RBUTTONUP:
	{
		rightMouseDown = false;
		break;
	}
	case WM_MOUSEMOVE:
	{
		if (rightMouseDown)
		{
			m_graphics->GetCamera().OffsetCameraRotation(
				(float)(GET_Y_LPARAM(lParam) - mouseY) * g_cameraMouseSensitivity,
				(float)(GET_X_LPARAM(lParam) - mouseX) * g_cameraMouseSensitivity,
				0);

			mouseX = GET_X_LPARAM(lParam);
			mouseY = GET_Y_LPARAM(lParam);

		}
		else
		{
			mouseX = GET_X_LPARAM(lParam);
			mouseY = GET_Y_LPARAM(lParam);
		}
		break;
	}
	case WM_DESTROY:
	{
		PostQuitMessage(0);
		break;
	}
	default:
		result = DefWindowProcW(hWnd, uMsg, wParam, lParam);
	}

	return result;
}
