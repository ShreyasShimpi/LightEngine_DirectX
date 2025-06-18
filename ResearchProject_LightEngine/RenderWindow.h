#pragma once
// internal includes
#include "Dependencies.h"

class CEngine;

class CRenderWindow
{
private:
	// variables
	HWND m_windowHandle; //Handle to this window
	HINSTANCE m_appInstanceHandle; //Handle to application instance

	std::wstring m_windowTitleW; //Wide string to store window title
	std::wstring m_windowClassNameW; //Wide string to store window class name
	
	int m_width;
	int m_height;

	// function
	void RegisterWindowClass();
public:
	// constructor
	CRenderWindow();

	// destructor
	~CRenderWindow();

	// getter
	HWND GetWindowHandle() const { return m_windowHandle; };
	HINSTANCE GetAppInstanceHandle() const { return m_appInstanceHandle; };
	
	// functions
	bool Initialize(CEngine* engine, int width, int height, HINSTANCE hInstance, std::wstring windowTitle, std::wstring windowClassName);
	bool ProcessMessages();
	void SetTitleWindow(const std::string& title);
};