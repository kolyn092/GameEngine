#include "pch.h"
#include "CFont.h"
#include "CUtil.h"
#include "FontManager.h"

ComponentEngine::CFont::CFont()
	: m_FontName(""), m_FilePath(""), m_FontIndex(0), m_LineSpacing(14.0f), m_FontSize(10.0f)
{

}


ComponentEngine::CFont::CFont(std::string fontName)
	: m_FontName(fontName), m_FilePath(""), m_FontIndex(0), m_LineSpacing(14.0f), m_FontSize(10.0f)
{

}

ComponentEngine::CFont::~CFont()
{

}

void ComponentEngine::CFont::CreateFont()
{
	if (!m_FilePath.empty())
	{
		TCHAR* _char = StringToTCHAR(m_FilePath);
		m_FontIndex = DrawText_Debug::GetInstance()->CreateFont(_char, m_LineSpacing);
		FontManager::Ins()->AddFont(this);
		delete _char;
	}
}