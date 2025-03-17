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
	// ���� ���¿� ���� ���°� ���� ���� false ����
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
	// ���� ��ư ���°� ����
	m_PrevButtonState = m_NowButtonState;

	// ��ư ������Ʈ
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

				// ���콺 �Է� �޾��� �� �̺�Ʈ �Լ� ȣ��
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

				// ��ư ������ ���� �� 
				if (Input::Ins()->GetMouseButton(Input::eMouse::leftButton))
				{
					m_NowButtonState = eButtonState::Pressed;
				}

				// ��ư ���� ��
				if (Input::Ins()->GetMouseButtonUp(Input::eMouse::leftButton))
				{
					m_NowButtonState = eButtonState::Default;
				}
			}
			else
			{
				// ��ư ���� ������ ���콺�� ������ ��
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
