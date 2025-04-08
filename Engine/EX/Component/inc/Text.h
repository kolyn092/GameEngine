#pragma once
#include "Component.h"

namespace ComponentEngine
{
	class CFont;
	class UI_Text : public Component
	{
	public:
		enum class eFontStyle
		{
			Normal,
			Bold,
			Italic,
			BoldAndItalic,
		};

		enum class eTextAnchor
		{
			UpperLeft,
			UpperCENTER,
			UpperRIGHT,
			MiddleLeft,
			MiddleCenter,
			LowerLeft,
			LowerCenter,
			LowerRight,
		};

	public:
		std::string m_Text;

	private:
		CFont* m_Font;
		DirectX::SimpleMath::Vector4 m_Color;

		DirectX::SimpleMath::Vector2 m_TextPos;

		bool m_ActiveState;

	public:
		UI_Text();
		virtual ~UI_Text();

	public:
		void SetColor(DirectX::SimpleMath::Vector4 color);
		void SetTextPosition(DirectX::SimpleMath::Vector2 pos) { m_TextPos = pos; }
		void SetFont(CFont* font);
		Vector2 GetFontSize();

	public:
		virtual void OnGUI() override;
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

