#pragma once
#include "Gamepad.h"
#include "Keyboard.h"
#include "Mouse.h"

class Input
{

public:
	enum class eMouse
	{
		leftButton,		// 왼쪽 버튼
		middleButton,	// 휠 버튼
		rightButton,	// 오른쪽 버튼
		xButton1,		// 사이드 위 버튼
		xButton2,		// 사이드 아래 버튼
	};

	enum class eMouseMode
	{
		None, 
		WindowLock,			// 화면 밖으로 못나가게 제어
		CenterLock,			// 마우스 중앙 고정
	};

private:
	// Input devices.
	std::unique_ptr<DirectX::Keyboard>			m_KeyboardDevice;
	std::unique_ptr<DirectX::Mouse>				m_MouseDevice;

private:
	HWND m_WindowHandle;

	DirectX::Keyboard::State					m_Keyboard;
	DirectX::Mouse::State						m_Mouse;
	DirectX::Keyboard::Keys						m_NowKey;

	DirectX::Keyboard::KeyboardStateTracker*	m_KeyboardTracker;
	DirectX::Mouse::ButtonStateTracker*			m_MouseTracker;

	int m_PrevMousePosX;
	int m_PrevMousePosY;

	DirectX::SimpleMath::Vector2 m_MouseDirection;
	float m_MouseDistance;

	bool m_isResetCursor;
	bool m_isWindowActive;

private:
	DirectX::SimpleMath::Matrix m_View;
	DirectX::SimpleMath::Matrix m_Proj;

#pragma region SINGLETON
private:
	static Input* m_pInstance;

public:
	static Input* Ins()
	{
		if (m_pInstance == nullptr)
		{
			m_pInstance = new Input();
		}
		return m_pInstance;
	}
#pragma endregion

private:
	Input();
	~Input();

private:
	void UpdateKeyState();

public:
	void Initalize(HWND window);
	void Finalize();
	void Update();
	bool GetKey(DirectX::Keyboard::Keys key);
	bool GetKeyDown(DirectX::Keyboard::Keys key);
	bool GetKeyUp(DirectX::Keyboard::Keys key);
	bool GetMouseButton(eMouse mouse);
	bool GetMouseButtonDown(eMouse mouse);
	bool GetMouseButtonUp(eMouse mouse);

	DirectX::Keyboard::Keys GetNowKey() const { return m_NowKey; }

	void SetCameraMatrix(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj);

	DirectX::SimpleMath::Vector4 ViewPortToWorld();
	DirectX::SimpleMath::Vector4 ViewPortToWorld(float x, float y);

	DirectX::SimpleMath::Vector2 GetMousePos();
	DirectX::SimpleMath::Vector2 GetPrevMousePos();
	DirectX::SimpleMath::Vector2 GetMouseDirection() const { return m_MouseDirection; }
	float GetMouseDistance() const { return m_MouseDistance; }
	DirectX::Keyboard::KeyboardStateTracker* GetKeyboardTracker() { return m_KeyboardTracker; }
	DirectX::Mouse::ButtonStateTracker* GetMouseTracker() { return m_MouseTracker; }

	void SetMouseDirection();
	void SetWindowActive(bool state) { m_isWindowActive = state; }
	void SetMouseLock(bool state) { m_isResetCursor = state; }


};
