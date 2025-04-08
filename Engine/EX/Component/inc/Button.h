#pragma once
#include "Component.h"

namespace ComponentEngine
{
	class UI_Button : public Component
	{
	public:
		enum class eButtonState
		{
			Default, Hover, Pressed, Selected, Disabled,
		};

	private:
		// Interactable
		bool m_Interactable;

		// OnClickEvent
		std::function<void()> m_EventFunction;

		DirectX::SimpleMath::Vector4 m_NormalColor;			// Normal Color : Default 일반
		DirectX::SimpleMath::Vector4 m_HightlightedColor;	// Hightlighted Color : Hover 강조된 경우
		DirectX::SimpleMath::Vector4 m_PressedColor;		// Pressed Color : Pressed 눌렀을 경우
		DirectX::SimpleMath::Vector4 m_SelectedColor;		// Selected Color : Released, Not Selected Other object
		DirectX::SimpleMath::Vector4 m_DisabledColor;		// Disabled Color : 비활성화된 경우

		eButtonState m_PrevButtonState;
		eButtonState m_NowButtonState;

		bool m_ActiveState;

	public:
		UI_Button();
		virtual ~UI_Button();

	public:
		void SetOnClickEvent(std::function<void()> func);

		void SetNormalColor(DirectX::SimpleMath::Vector4 col) { m_NormalColor = col; }
		void SetHightlightColor(DirectX::SimpleMath::Vector4 col) { m_HightlightedColor = col; }
		void SetPressedColor(DirectX::SimpleMath::Vector4 col) { m_PressedColor = col; }
		void SetSelectedColor(DirectX::SimpleMath::Vector4 col) { m_SelectedColor = col; }
		void SetDisabledColor(DirectX::SimpleMath::Vector4 col) { m_DisabledColor = col; }

		DirectX::SimpleMath::Vector4 GetNowStateColor();
		bool IsStateChanged();
		bool IsPressed();

	public:
		virtual void Update() override;

		virtual void DebugTextRender() override;

		virtual void OnQuit() override;
		virtual void OnDisable() override;
		virtual void OnDestroy() override;

		virtual void SetActive(bool value) override;
		virtual bool isActive() override { return m_ActiveState; }

	public:
		virtual void Bind(GameObject* obj) override;
	};
}

