#include "pch.h"
#include "Text.h"
#include "Transform.h"
#include "CUtil.h"
#include "CFont.h"

using namespace DirectX::SimpleMath;

ComponentEngine::UI_Text::UI_Text()
	: ComponentImpl("UI_Text"), m_Text(""), m_Font(), m_Color(Vector4(1, 1, 1, 1)), m_TextPos(Vector2(0, 0)), m_ActiveState(true)
{

}

ComponentEngine::UI_Text::~UI_Text()
{

}

void ComponentEngine::UI_Text::SetColor(Vector4 color)
{
	m_Color = color;
}

void ComponentEngine::UI_Text::SetFont(CFont* font)
{
	if (font)
	{
		m_Font = font;
	}
}

Vector2 ComponentEngine::UI_Text::GetFontSize()
{
	Vector2 _result = Vector2::Zero;
	if (m_Font)
	{
		_result = DrawText_Debug::GetInstance()->GetSpriteFontSize(m_Font->m_FontIndex, StringToTCHAR(m_Text), m_Font->m_FontSize);
	}
	return _result;
}

void ComponentEngine::UI_Text::OnGUI()
{
	float _fontSize = 10.0f;
	int _fontIndex = 0;
	if (m_Font != nullptr)
	{
		_fontSize = m_Font->m_FontSize;
		_fontIndex = m_Font->m_FontIndex;
	}

	Transform* _trans = m_GameObject->GetComponent<Transform>();
	Vector3 _pos = _trans->GetPosition();
	va_list vl;
	TCHAR* _ch = StringToTCHAR(m_Text);
	va_start(vl, _ch);
	DrawText_Debug::GetInstance()->DrawTextEx(_fontIndex, static_cast<int>(_pos.x + m_TextPos.x), static_cast<int>(_pos.y + m_TextPos.y), m_Color, _fontSize, _ch, vl);
	va_end(vl);
	delete _ch;
}

void ComponentEngine::UI_Text::DebugTextRender()
{

}

void ComponentEngine::UI_Text::OnQuit()
{

}

void ComponentEngine::UI_Text::OnDisable()
{

}

void ComponentEngine::UI_Text::OnDestroy()
{
	if (m_Font)
	{
		m_Font = nullptr;
	}
}

void ComponentEngine::UI_Text::SetActive(bool value)
{
	if (value == true && m_ActiveState != value)
	{
		Start();
	}
	m_ActiveState = value;
}

void ComponentEngine::UI_Text::Bind(GameObject* obj)
{

}
