#include "pch.h"
#include "Button.h"
#include "Transform.h"
#include "Input.h"
#include "Image.h"
#include "CDebug.h"

using namespace DirectX::SimpleMath;

ComponentEngine::UI_Button::UI_Button()
	: Component("UI_Button"), m_Interactable(true), m_NormalColor(Vector4::One), m_HightlightedColor(Vector4(0.8f, 0.8f, 0.8f, 1)),
	m_PressedColor(Vector4(0.6f, 0.6f, 0.6f, 1)), m_SelectedColor(Vector4(0.8f, 0.8f, 0.8f, 1)),
	m_DisabledColor(Vector4(0.6f, 0.6f, 0.6f, 0.5f)), m_PrevButtonState(eButtonState::Default),
	m_NowButtonState(eButtonState::Default), m_ActiveState(true)
{

}

ComponentEngine::UI_Button::~UI_Button()
{

}

void ComponentEngine::UI_Button::SetOnClickEvent(std::function<void()> func)
{
	m_EventFunction = func;
}

DirectX::SimpleMath::Vector4 ComponentEngine::UI_Button::GetNowStateColor()
{
	switch (m_NowButtonState)
	{
		case ComponentEngine::UI_Button::eButtonState::Default:
			return m_NormalColor;
		case ComponentEngine::UI_Button::eButtonState::Hover:
			return m_HightlightedColor;
		case ComponentEngine::UI_Button::eButtonState::Pressed:
			return m_PressedColor;
		case ComponentEngine::UI_Button::eButtonState::Selected:
			return m_SelectedColor;
		case ComponentEngine::UI_Button::eButtonState::Disabled:
			return m_DisabledColor;
		default:
			return Vector4(1, 1, 1, 1);
	}
}

bool ComponentEngine::UI_Button::IsStateChanged()
{
	// 이전 상태와 현재 상태가 같을 경우는 false 리턴
	if (m_PrevButtonState == m_NowButtonState)
	{
		return false;
	}
	return true;
}

bool ComponentEngine::UI_Button::IsPressed()
{
	if (m_NowButtonState == eButtonState::Pressed)
	{
		return true;
	}
	return false;
}

void ComponentEngine::UI_Button::Update()
{
	// 이전 버튼 상태값 저장
	m_PrevButtonState = m_NowButtonState;

	// 버튼 업데이트
	if (m_GameObject)
	{
		Transform* _trans = m_GameObject->GetComponent<Transform>();
		UI_Image* _image = m_GameObject->GetComponent<UI_Image>();
		if (_image)
		{
			RECT rc = { static_cast<LONG>(_trans->GetPosition().x),
						static_cast<LONG>(_trans->GetPosition().y),
						static_cast<LONG>(_trans->GetPosition().x + _image->GetImageWidth()),
						static_cast<LONG>(_trans->GetPosition().y + _image->GetImageHeight()) };

			POINT pt = { static_cast<LONG>(Input::Ins()->GetMousePos().x), static_cast<LONG>(Input::Ins()->GetMousePos().y) };
			if (PtInRect(&rc, pt))
			{
				m_NowButtonState = eButtonState::Hover;

				// 마우스 입력 받았을 때 이벤트 함수 호출
				if (Input::Ins()->GetMouseButtonDown(Input::eMouse::leftButton))
				{
					if (m_ActiveState)
					{
						if (m_EventFunction)
						{
							m_EventFunction();
						}
					}
				}

				// 버튼 누르는 중일 때 
				if (Input::Ins()->GetMouseButton(Input::eMouse::leftButton))
				{
					m_NowButtonState = eButtonState::Pressed;
				}

				// 버튼 뗐을 때
				if (Input::Ins()->GetMouseButtonUp(Input::eMouse::leftButton))
				{
					m_NowButtonState = eButtonState::Default;
				}
			}
			else
			{
				// 버튼 범위 밖으로 마우스가 나갔을 때
				m_NowButtonState = eButtonState::Default;
			}
		}
	}
}

void ComponentEngine::UI_Button::DebugTextRender()
{
	//CDebug::Ins()->DebugDrawText(L"Button State : %d", m_NowButtonState);
}

void ComponentEngine::UI_Button::OnQuit()
{

}

void ComponentEngine::UI_Button::OnDisable()
{

}

void ComponentEngine::UI_Button::OnDestroy()
{

}

void ComponentEngine::UI_Button::SetActive(bool value)
{
	if (value == true && m_ActiveState != value)
	{
		Start();
	}
	m_ActiveState = value;
}

void ComponentEngine::UI_Button::Bind(GameObject* obj)
{
	if (obj)
	{
		obj->m_Button = this;
	}
}
