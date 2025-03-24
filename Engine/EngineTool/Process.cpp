#include <windows.h>
#include <tchar.h>
#include "Process.h"
#include "Input.h"

using namespace DirectX;

Process::Process()
	: m_hWnd(NULL), m_Msg()
{

}

Process::~Process()
{
	if (m_ComponentEngine)
	{
		m_ComponentEngine->Finalize();
		m_ComponentEngine = nullptr;
	}
}

HRESULT Process::Window_Initalize(HINSTANCE hInstance)
{
	HICON _icon = LoadIcon(hInstance, MAKEINTRESOURCE(101));

	// ���� Ŭ����
	TCHAR szAppName[] = _TEXT("TestClient");
	WNDCLASS wndclass;

	wndclass.style = CS_HREDRAW | CS_VREDRAW;
	wndclass.lpfnWndProc = Process::WndProc;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.hInstance = hInstance;
	wndclass.hIcon = _icon;
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndclass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wndclass.lpszMenuName = NULL;
	wndclass.lpszClassName = szAppName;

	// ���� Ŭ���� ���
	RegisterClass(&wndclass);

	// ����� �ػ� �޾ƿ���
	//int _width = GetSystemMetrics(SM_CXSCREEN);
	//int _height = GetSystemMetrics(SM_CYSCREEN);

	int _x = 0 + (GetSystemMetrics(SM_CXSCREEN) * 0.5f) - (m_ScreenWidth * 0.5f);
	int _y = 0 + (GetSystemMetrics(SM_CYSCREEN) * 0.5f) - (m_ScreenHeight * 0.5f);
	int _width = m_ScreenWidth;
	int _height = m_ScreenHeight;

	// ���� ����
	m_hWnd = CreateWindow(
		szAppName,
		szAppName,
		WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
		_x, _y, _width, _height,
		GetDesktopWindow(), NULL, hInstance, NULL);

	if (!m_hWnd)
	{
		return FALSE;
	}

	if (FAILED(CoInitializeEx(nullptr, COINIT_MULTITHREADED)))
	{
		return S_FALSE;
	}

	// ������ ������ ȭ�鿡 ǥ��
	::ShowWindow(m_hWnd, SW_SHOWNORMAL);
	::UpdateWindow(m_hWnd);

	// Ŭ���̾�Ʈ ���� ũ�� ������
	ResizeWindow(m_hWnd, _width, _height);

	return S_OK;
}

void Process::Initalize()
{
	m_ComponentEngine = ComponentEngine::CESystem::Ins();
	m_ComponentEngine->Initalize(m_hWnd);

	Input::Ins()->Initalize(m_hWnd);

	m_ComponentEngine->Start();
}

void Process::Loop()
{
	while (true)
	{
		if (::PeekMessage(&m_Msg, NULL, 0, 0, PM_REMOVE))
		{
			if (m_Msg.message == WM_QUIT)
			{
				break;
			}

			::TranslateMessage(&m_Msg);
			::DispatchMessage(&m_Msg);
		}
		else
		{
			Update();
			Render();
		}
	}
}

void Process::Update()
{
	m_ComponentEngine->Update();
}

void Process::Render()
{
	m_ComponentEngine->Draw();
}

LRESULT CALLBACK Process::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	HDC			hdc;
	PAINTSTRUCT ps;

	switch (message)
	{
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_ACTIVATE:
	{
		unsigned short _state = LOWORD(wParam);
		switch (_state)
		{
		case WA_ACTIVE:
		case WA_CLICKACTIVE:
		{
			Input::Ins()->SetWindowActive(true);
		}
		break;
		case WA_INACTIVE:
		{
			Input::Ins()->SetWindowActive(false);
		}
		break;
		default:
			break;
		}
	}
	break;
	case WM_INPUT:
	case WM_MOUSEMOVE:
	case WM_LBUTTONDOWN:
	case WM_LBUTTONUP:
	case WM_RBUTTONDOWN:
	case WM_RBUTTONUP:
	case WM_MBUTTONDOWN:
	case WM_MBUTTONUP:
	case WM_MOUSEWHEEL:
	case WM_XBUTTONDOWN:
	case WM_XBUTTONUP:
	case WM_MOUSEHOVER:
		Mouse::ProcessMessage(message, wParam, lParam);
		break;
	case WM_KEYDOWN:
	case WM_KEYUP:
	case WM_SYSKEYUP:
		Keyboard::ProcessMessage(message, wParam, lParam);
		break;
	case WM_SYSKEYDOWN:
		Keyboard::ProcessMessage(message, wParam, lParam);
		break;
	case WM_QUIT:
		CloseHandle(hWnd);
		break;
	}

	return DefWindowProc(hWnd, message, wParam, lParam);
}

void Process::ResizeWindow(HWND hWnd, UINT width, UINT height)
{
	// ���� �������� ��Ÿ�� ���ϱ�
	DWORD style = ::GetWindowLong(hWnd, GWL_STYLE);
	DWORD exstyle = ::GetWindowLong(hWnd, GWL_EXSTYLE);

	// ���� ������ ��ü ũ�� ���
	RECT oldRC;
	::GetWindowRect(hWnd, &oldRC);

	// ���� ������ �������� Ŭ�� ���� ũ�� ���
	RECT newRC;
	newRC.left = 0;
	newRC.top = 0;
	newRC.right = width;
	newRC.bottom = height;

	// ���ο� Ŭ�� ���� ũ�� ��ŭ�� ������ ũ�⸦ ���ϱ�
	::AdjustWindowRectEx(&newRC, style, NULL, exstyle);

	// ������ �������� �ʺ�� ���� ���ϱ�
	int newWidth = newRC.right - newRC.left;
	int newHeight = newRC.bottom - newRC.top;

	// ���ο� ũ��� ����
	::SetWindowPos(hWnd, HWND_NOTOPMOST, oldRC.left, oldRC.top, newWidth, newHeight, SWP_SHOWWINDOW);

}