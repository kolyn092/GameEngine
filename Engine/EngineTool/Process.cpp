#include <windows.h>
#include <tchar.h>
#include "Process.h"
#include "Input.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"
#include "ImguiManager.h"

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

	//imgui 삭제
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}

HRESULT Process::Window_Initalize(HINSTANCE hInstance)
{
	HICON _icon = LoadIcon(hInstance, MAKEINTRESOURCE(101));

	// 윈도 클래스
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

	// 윈도 클래스 등록
	RegisterClass(&wndclass);

	// 모니터 해상도 받아오기
	//int _width = GetSystemMetrics(SM_CXSCREEN);
	//int _height = GetSystemMetrics(SM_CYSCREEN);

	int _x = 0 + (GetSystemMetrics(SM_CXSCREEN) * 0.5f) - (m_ScreenWidth * 0.5f);
	int _y = 0 + (GetSystemMetrics(SM_CYSCREEN) * 0.5f) - (m_ScreenHeight * 0.5f);
	int _width = m_ScreenWidth;
	int _height = m_ScreenHeight;

	// 윈도 생성
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

	// 생성된 윈도를 화면에 표시
	::ShowWindow(m_hWnd, SW_SHOWNORMAL);
	::UpdateWindow(m_hWnd);

	// Component 엔진 초기화
	m_ComponentEngine = ComponentEngine::CESystem::Ins();
	m_ComponentEngine->Initalize(m_hWnd);

	Input::Ins()->Initalize(m_hWnd);

	//imgui 초기화
	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

	// Setup Platform/Renderer backends
	ImGui_ImplWin32_Init(m_hWnd);
	ImGui_ImplDX11_Init(m_ComponentEngine->GetDevice(), m_ComponentEngine->GetDeviceContext());

	// 클라이언트 영역 크기 재조정
	ResizeWindow(m_hWnd, _width, _height);

	return S_OK;
}

void Process::Initalize()
{
	m_ComponentEngine->Start();
	ImguiManager::Ins()->Initalize();
}

bool show_demo_window = true;
bool show_another_window = false;
ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

bool isGUIInit = true;

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
			ImGui_ImplDX11_NewFrame();
			ImGui_ImplWin32_NewFrame();
			ImGui::NewFrame();

			m_ComponentEngine->Update();

			///*
			if (show_demo_window)
				ImGui::ShowDemoWindow(&show_demo_window);

			{
				static float f = 0.0f;
				static int counter = 0;

				ImGui::Begin("Hello World!");

				ImGui::Text("This is some useful text.");
				ImGui::Checkbox("Demo Window", &show_demo_window);
				ImGui::Checkbox("Another Window", &show_another_window);

				ImGui::SliderFloat("float", &f, 0.0f, 1.0f);
				ImGui::ColorEdit3("clear color", (float*)&clear_color);

				if (ImGui::Button("Button"))
					counter++;
				ImGui::SameLine();
				ImGui::Text("counter : % d", counter);

				ImGui::End();
			}

			if (show_another_window)
			{
				ImGui::Begin("Another Window", &show_another_window);
				ImGui::Text("Hello from another window!");
				if (ImGui::Button("Close Me"))
					show_another_window = false;
				ImGui::End();
			}
			//*/

			ImguiManager::Ins()->Update();

			//Draw
			m_ComponentEngine->BeginDraw();
			m_ComponentEngine->Draw();

			ImGui::Render();
			ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

			m_ComponentEngine->EndDraw();
		}
	}
}

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT CALLBACK Process::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	HDC			hdc;
	PAINTSTRUCT ps;

	if (ImGui_ImplWin32_WndProcHandler(hWnd, message, wParam, lParam))
		return true;

	switch (message)
	{
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		EndPaint(hWnd, &ps);
		break;
	case WM_DPICHANGED:
		if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_DpiEnableScaleViewports) // 도킹 깃허브 받은거 확인
		{
			//const int dpi = HIWORD(wParam);
			const RECT* suggested_rect = (RECT*)lParam;
			::SetWindowPos(hWnd, NULL, suggested_rect->left, suggested_rect->top,
				suggested_rect->right - suggested_rect->left, suggested_rect->bottom - suggested_rect->top, SWP_NOZORDER | SWP_NOACTIVATE);
		}
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
	// 현재 윈도우의 스타일 구하기
	DWORD style = ::GetWindowLong(hWnd, GWL_STYLE);
	DWORD exstyle = ::GetWindowLong(hWnd, GWL_EXSTYLE);

	// 현재 윈도우 전체 크기 얻기
	RECT oldRC;
	::GetWindowRect(hWnd, &oldRC);

	// 새로 생성할 윈도우의 클라 영역 크기 계산
	RECT newRC;
	newRC.left = 0;
	newRC.top = 0;
	newRC.right = width;
	newRC.bottom = height;

	// 새로운 클라 영역 크기 만큼의 윈도우 크기를 구하기
	::AdjustWindowRectEx(&newRC, style, NULL, exstyle);

	// 보정된 윈도우의 너비와 폭을 구하기
	int newWidth = newRC.right - newRC.left;
	int newHeight = newRC.bottom - newRC.top;

	// 새로운 크기로 설정
	::SetWindowPos(hWnd, HWND_NOTOPMOST, oldRC.left, oldRC.top, newWidth, newHeight, SWP_SHOWWINDOW);

}