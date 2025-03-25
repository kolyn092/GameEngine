#pragma once
#include "CESystem.h"

class Process
{

private:
	ComponentEngine::CESystem* m_ComponentEngine;

private:
	HWND m_hWnd;
	MSG m_Msg;

	const int m_ScreenWidth = 1600;
	const int m_ScreenHeight = 900;

public:
	Process();
	~Process();

public:
	HRESULT Window_Initalize(HINSTANCE hInstance);
	void Initalize();
	void Loop();
	void Update();
	void Render();
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	void ResizeWindow(HWND hWnd, UINT width, UINT height);
};

