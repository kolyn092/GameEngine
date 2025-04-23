#pragma once
#include "Component.h"

namespace ComponentEngine
{
	class InputField : public ComponentImpl<InputField>
	{
	public:
		enum class eContentType
		{
			Standard, Password,
		};

		enum class eAlignmentType
		{
			Left, Center, Right,
		};

	public:
		eContentType m_ContentType;
		eAlignmentType m_AlignmentType;

	private:
		std::string m_Text;
		std::string m_TextYCheck;	// 문자열이 하나일 때 spritefont에서 사이즈를 잘못 가져오는 문제 때문에 생성
		float m_TextSize;
		DirectX::SimpleMath::Vector2 m_TextPos;
		DirectX::SimpleMath::Vector4 m_TextColor;
		unsigned int m_CharacterLimit;
		bool m_CanInputText;

		bool m_ActiveState;

	public:
		InputField();
		virtual ~InputField();

	private:
		void CollisionUpdate();
		void UpdateTextInput();
		void SettingTextAlignment(DirectX::SimpleMath::Vector2 textLength);

	public:
		void SetContentType(eContentType type) { m_ContentType = type; }
		void SetTextPosition(DirectX::SimpleMath::Vector2 pos) { m_TextPos = pos; }
		void SetTextColor(DirectX::SimpleMath::Vector4 col) { m_TextColor = col; }
		void SetCharacterLimit(unsigned int val) { m_CharacterLimit = val; }
		void SetInputTextActive(bool value) { m_CanInputText = value; }

		std::string GetText() const { return m_Text; }
		bool GetInputTextActive() const { return m_CanInputText; }

	public:
		virtual void Update() override;

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

