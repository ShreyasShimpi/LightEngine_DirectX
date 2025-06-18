#include "Engine.h"
#include "Data.h"

int CALLBACK wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow)
{
	CEngine engine;
	if (engine.Initialize(hInstance, L"Title", L"MyWindowClass", WIDTH, HEIGHT))
	{
		while (engine.ProcessMessages() == true)
		{
			engine.Update();
			engine.RenderFrame();
		}
	}
	return 0;
}
