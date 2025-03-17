#include "pch.h"
#include "FontManager.h"
#include "CFont.h"

ComponentEngine::FontManager* ComponentEngine::FontManager::m_pInstance = nullptr;

ComponentEngine::FontManager::FontManager()
{

}

ComponentEngine::FontManager::~FontManager()
{
	for (auto font : m_FontList)
	{
		delete font;
		font = nullptr;
	}

}

void ComponentEngine::FontManager::AddFont(CFont* font)
{
	if (font)
	{
		m_FontList.push_back(font);
	}
}

ComponentEngine::CFont* ComponentEngine::FontManager::FindFontByName(std::string fontName)
{
	for (auto font : m_FontList)
	{
		if (font->m_FontName == fontName)
		{
			return font;
		}
	}
	return nullptr;
}

void ComponentEngine::FontManager::Finalize()
{
	if (m_pInstance)
	{
		delete m_pInstance;
		m_pInstance = nullptr;
	}
}
