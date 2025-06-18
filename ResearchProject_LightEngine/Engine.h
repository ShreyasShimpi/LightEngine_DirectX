#pragma once

#include "Time.h"
#include "Dependencies.h"
#include <memory>

class CGraphics;
class CRenderWindow;

class CEngine
{
private:
	std::unique_ptr<CRenderWindow> m_renderWindow;
	std::unique_ptr<CGraphics> m_graphics;

	CTime m_time;

	unsigned char m_keyPressed;
public:

public:
	// constructor
	CEngine();

	// destructor
	~CEngine();

	// functions
	bool Initialize(HINSTANCE hInstance, std::wstring window_title, std::wstring window_class, int width, int height);
	bool ProcessMessages();
	void Update();
	void RenderFrame();

	LRESULT HandleMessages(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
};

