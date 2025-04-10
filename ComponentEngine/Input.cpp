#include "pch.h"
#include "Input.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

Input* Input::m_pInstance = nullptr;

Input::Input()
	: m_KeyboardDevice(nullptr), m_MouseDevice(nullptr),
	m_Keyboard(), m_Mouse(), m_NowKey(Keyboard::Keys::None),
	m_KeyboardTracker(nullptr), m_MouseTracker(nullptr),
	m_PrevMousePosX(0), m_PrevMousePosY(0),
	m_MouseDirection(Vector2::Zero), m_MouseDistance(0.f),
	m_isResetCursor(false), m_isWindowActive(false),
	m_View(Matrix::Identity), m_Proj(Matrix::Identity)
{
	m_KeyboardTracker = new Keyboard::KeyboardStateTracker();
	m_MouseTracker = new Mouse::ButtonStateTracker();
}

Input::~Input()
{

}

void Input::Initalize(HWND window)
{
	m_WindowHandle = window;

	m_KeyboardDevice = std::make_unique<Keyboard>();
	m_MouseDevice = std::make_unique<Mouse>();
	m_MouseDevice->SetWindow(window);

	///m_MouseDevice->SetMode(Mouse::MODE_RELATIVE);
}

void Input::Finalize()
{
	if (m_pInstance)
	{
		delete m_pInstance;
		m_pInstance = nullptr;
	}
}

void Input::Update()
{
	// 이전 값 저장
	m_PrevMousePosX = m_Mouse.x;
	m_PrevMousePosY = m_Mouse.y;

	if (m_KeyboardDevice->IsConnected())
	{
		m_Keyboard = m_KeyboardDevice->GetState();
		m_KeyboardTracker->Update(m_Keyboard);
	}

	if (m_MouseDevice->IsConnected())
	{
		m_Mouse = m_MouseDevice->GetState();
		m_MouseTracker->Update(m_Mouse);
	}

	UpdateKeyState();
	SetMouseDirection();

	//RECT _lpRect = {};
	//GetWindowRect(m_WindowHandle, &_lpRect);

	//if (m_isWindowActive)
	//{
	//	ClipCursor(&_lpRect);
	//}

	//if (m_isResetCursor)
	//{
	//	if (m_Mouse.x > 1910 || m_Mouse.x < 10
	//		|| m_Mouse.y > 1070 || m_Mouse.y < 10)
	//	{
	//		m_PrevMousePosX = 960;
	//		m_PrevMousePosY = 540;
	//		SetCursorPos(_lpRect.left + 960, _lpRect.top + 540);
	//		m_Mouse.x = m_PrevMousePosX;
	//		m_Mouse.y = m_PrevMousePosY;
	//	}
	//}

	RECT _rect = {};
	GetClientRect(m_WindowHandle, &_rect);

	POINT ul;
	ul.x = _rect.left;
	ul.y = _rect.top;

	POINT lr;
	lr.x = _rect.right;
	lr.y = _rect.bottom;

	MapWindowPoints(m_WindowHandle, nullptr, &ul, 1);
	MapWindowPoints(m_WindowHandle, nullptr, &lr, 1);

	_rect.left = ul.x;
	_rect.top = ul.y;

	_rect.right = lr.x;
	_rect.bottom = lr.y;

	if (m_isWindowActive)
	{
		ClipCursor(&_rect);
	}

	if (m_isResetCursor)
	{
		if (m_Mouse.x > 1910 || m_Mouse.x < 10 || m_Mouse.y > 1070 || m_Mouse.y < 10)
		{
			SetCursorPos((_rect.right - _rect.left) / 2 + _rect.left, (_rect.bottom - _rect.top) / 2 + _rect.top);
		}
	}


}

void Input::UpdateKeyState()
{
	m_NowKey = Keyboard::Keys::None;

	if (m_Keyboard.Q)		m_NowKey = Keyboard::Keys::Q;
	if (m_Keyboard.W)		m_NowKey = Keyboard::Keys::W;
	if (m_Keyboard.E)		m_NowKey = Keyboard::Keys::E;
	if (m_Keyboard.R)		m_NowKey = Keyboard::Keys::R;
	if (m_Keyboard.T)		m_NowKey = Keyboard::Keys::T;
	if (m_Keyboard.Y)		m_NowKey = Keyboard::Keys::Y;
	if (m_Keyboard.U)		m_NowKey = Keyboard::Keys::U;
	if (m_Keyboard.I)		m_NowKey = Keyboard::Keys::I;
	if (m_Keyboard.O)		m_NowKey = Keyboard::Keys::O;
	if (m_Keyboard.P)		m_NowKey = Keyboard::Keys::P;
	if (m_Keyboard.A)		m_NowKey = Keyboard::Keys::A;
	if (m_Keyboard.S)		m_NowKey = Keyboard::Keys::S;
	if (m_Keyboard.D)		m_NowKey = Keyboard::Keys::D;
	if (m_Keyboard.F)		m_NowKey = Keyboard::Keys::F;
	if (m_Keyboard.G)		m_NowKey = Keyboard::Keys::G;
	if (m_Keyboard.H)		m_NowKey = Keyboard::Keys::H;
	if (m_Keyboard.J)		m_NowKey = Keyboard::Keys::J;
	if (m_Keyboard.K)		m_NowKey = Keyboard::Keys::K;
	if (m_Keyboard.L)		m_NowKey = Keyboard::Keys::L;
	if (m_Keyboard.Z)		m_NowKey = Keyboard::Keys::Z;
	if (m_Keyboard.X)		m_NowKey = Keyboard::Keys::X;
	if (m_Keyboard.C)		m_NowKey = Keyboard::Keys::C;
	if (m_Keyboard.V)		m_NowKey = Keyboard::Keys::V;
	if (m_Keyboard.B)		m_NowKey = Keyboard::Keys::B;
	if (m_Keyboard.N)		m_NowKey = Keyboard::Keys::N;
	if (m_Keyboard.M)		m_NowKey = Keyboard::Keys::M;
	if (m_Keyboard.D1)		m_NowKey = Keyboard::Keys::D1;
	if (m_Keyboard.D2)		m_NowKey = Keyboard::Keys::D2;
	if (m_Keyboard.D3)		m_NowKey = Keyboard::Keys::D3;
	if (m_Keyboard.D4)		m_NowKey = Keyboard::Keys::D4;
	if (m_Keyboard.D5)		m_NowKey = Keyboard::Keys::D5;
	if (m_Keyboard.D6)		m_NowKey = Keyboard::Keys::D6;
	if (m_Keyboard.D7)		m_NowKey = Keyboard::Keys::D7;
	if (m_Keyboard.D8)		m_NowKey = Keyboard::Keys::D8;
	if (m_Keyboard.D9)		m_NowKey = Keyboard::Keys::D9;
	if (m_Keyboard.D0)		m_NowKey = Keyboard::Keys::D0;
	if (m_Keyboard.Back)	m_NowKey = Keyboard::Keys::Back;
	if (m_Keyboard.Space)	m_NowKey = Keyboard::Keys::Space;
	if (m_Keyboard.Enter)	m_NowKey = Keyboard::Keys::Enter;
	//if (m_Keyboard.Tab)		m_NowKey = Keyboard::Keys::Tab;
}

// 누르는 도중 계속 호출
bool Input::GetKey(Keyboard::Keys key)
{
	if (m_Keyboard.IsKeyDown(key))
	{
		return true;
	}
	return false;
}

// 눌렀을 때 한번만
bool Input::GetKeyDown(Keyboard::Keys key)
{
	if (m_KeyboardTracker->IsKeyPressed(key))
	{
		return true;
	}
	return false;
}

// 눌렀다가 뗐을 때 한번만
bool Input::GetKeyUp(Keyboard::Keys key)
{
	if (m_KeyboardTracker->IsKeyReleased(key))
	{
		return true;
	}
	return false;
}

bool Input::GetMouseButton(eMouse mouse)
{
	switch (mouse)
	{
		case Input::eMouse::leftButton:
			return (m_MouseTracker->leftButton == Mouse::ButtonStateTracker::ButtonState::HELD);
			break;
		case Input::eMouse::middleButton:
			return (m_MouseTracker->middleButton == Mouse::ButtonStateTracker::ButtonState::HELD);
			break;
		case Input::eMouse::rightButton:
			return (m_MouseTracker->rightButton == Mouse::ButtonStateTracker::ButtonState::HELD);
			break;
		case Input::eMouse::xButton1:
			return (m_MouseTracker->xButton1 == Mouse::ButtonStateTracker::ButtonState::HELD);
			break;
		case Input::eMouse::xButton2:
			return (m_MouseTracker->xButton2 == Mouse::ButtonStateTracker::ButtonState::HELD);
			break;
		default:
			break;
	}
	return false;
}

bool Input::GetMouseButtonDown(eMouse mouse)
{
	switch (mouse)
	{
		case Input::eMouse::leftButton:
			return (m_MouseTracker->leftButton == Mouse::ButtonStateTracker::ButtonState::PRESSED);
			break;
		case Input::eMouse::middleButton:
			return (m_MouseTracker->middleButton == Mouse::ButtonStateTracker::ButtonState::PRESSED);
			break;
		case Input::eMouse::rightButton:
			return (m_MouseTracker->rightButton == Mouse::ButtonStateTracker::ButtonState::PRESSED);
			break;
		case Input::eMouse::xButton1:
			return (m_MouseTracker->xButton1 == Mouse::ButtonStateTracker::ButtonState::PRESSED);
			break;
		case Input::eMouse::xButton2:
			return (m_MouseTracker->xButton2 == Mouse::ButtonStateTracker::ButtonState::PRESSED);
			break;
		default:
			break;
	}
	return false;
}

bool Input::GetMouseButtonUp(eMouse mouse)
{
	switch (mouse)
	{
		case Input::eMouse::leftButton:
			return (m_MouseTracker->leftButton == Mouse::ButtonStateTracker::ButtonState::RELEASED);
			break;
		case Input::eMouse::middleButton:
			return (m_MouseTracker->middleButton == Mouse::ButtonStateTracker::ButtonState::RELEASED);
			break;
		case Input::eMouse::rightButton:
			return (m_MouseTracker->rightButton == Mouse::ButtonStateTracker::ButtonState::RELEASED);
			break;
		case Input::eMouse::xButton1:
			return (m_MouseTracker->xButton1 == Mouse::ButtonStateTracker::ButtonState::RELEASED);
			break;
		case Input::eMouse::xButton2:
			return (m_MouseTracker->xButton2 == Mouse::ButtonStateTracker::ButtonState::RELEASED);
			break;
		default:
			break;
	}
	return false;
}

void Input::SetCameraMatrix(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj)
{
	m_View = view;
	m_Proj = proj;
}

Vector2 Input::GetMousePos()
{
	return DirectX::SimpleMath::Vector2(static_cast<float>(m_Mouse.x), static_cast<float>(m_Mouse.y));
}

Vector2 Input::GetPrevMousePos()
{
	return DirectX::SimpleMath::Vector2(static_cast<float>(m_PrevMousePosX), static_cast<float>(m_PrevMousePosY));
}

void Input::SetMouseDirection()
{
	// 도착점 - 시작점
	Vector2 _MousePos = Vector2(static_cast<float>(m_Mouse.x), static_cast<float>(m_Mouse.y));
	Vector2 _PrevMousePos = Vector2(static_cast<float>(m_PrevMousePosX), static_cast<float>(m_PrevMousePosY));
	m_MouseDirection = _MousePos - _PrevMousePos;
	m_MouseDirection.Normalize();
	m_MouseDistance = Vector2::Distance(_MousePos, _PrevMousePos);
}

Vector4 Input::ViewPortToWorld()
{
	//Matrix proj, view;
	//Viewport vp;
	//Vector3 _vPos1 = vp.Unproject(Vector3(m_Mouse.x, m_Mouse.y, 1), proj, view, Matrix::Identity);
	//Vector4 _vPos2 = Vector4(0, 0, 0, 1);
	//Matrix _invView = view.Invert();
	//_vPos2 = DirectX::SimpleMath::Vector4::Transform(_vPos2, _invView);
	//
	//Plane vPlane = Plane(Vector3(0, 0, 0), Vector3(0, 1, 0));
	//Vector4 Result = intersect(vPlane, _vPos1, _vPos2);
	//
	//Result = Vector4(Result.x, Result.y, Result.z, 1);

	/// TODO : 해상도 임시 상수값
	XMVECTOR _vPos1 = XMVector3Unproject(XMVectorSet(static_cast<float>(m_Mouse.x), static_cast<float>(m_Mouse.y), 1, 0), 0, 0,
		1920, 1080, 0, 1, m_Proj, m_View, XMMatrixIdentity());
	XMVECTOR _vPos2 = XMVectorSet(0, 0, 0, 1);
	XMMATRIX _invView = XMMatrixInverse(NULL, m_View);
	_vPos2 = XMVector3TransformCoord(_vPos2, _invView);

	XMVECTOR _vPlane = XMPlaneFromPointNormal(XMVectorSet(0, 0, 0, 1), XMVectorSet(0, 1, 0, 0));
	XMVECTOR _Result = XMPlaneIntersectLine(_vPlane, _vPos1, _vPos2);

	_Result = XMVectorSetW(_Result, 1);

	return _Result;
}

Vector4 Input::ViewPortToWorld(float x, float y)
{
	/// TODO : 해상도 임시 상수값
	XMVECTOR _vPos1 = XMVector3Unproject(XMVectorSet(x, y, 1, 0), 0, 0,
		1920, 1080, 0, 1, m_Proj, m_View, XMMatrixIdentity());
	XMVECTOR _vPos2 = XMVectorSet(0, 0, 0, 1);
	XMMATRIX _invView = XMMatrixInverse(NULL, m_View);
	_vPos2 = XMVector3TransformCoord(_vPos2, _invView);

	XMVECTOR _vPlane = XMPlaneFromPointNormal(XMVectorSet(0, 0, 0, 1), XMVectorSet(0, 1, 0, 0));
	XMVECTOR _Result = XMPlaneIntersectLine(_vPlane, _vPos1, _vPos2);

	_Result = XMVectorSetW(_Result, 1);

	return _Result;
}