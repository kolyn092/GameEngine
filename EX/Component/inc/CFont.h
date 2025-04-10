#pragma once

namespace ComponentEngine
{
	class CFont
	{
	public:
		std::string m_FontName;
		std::string m_FilePath;
		int m_FontIndex;
		float m_LineSpacing;
		float m_FontSize;

	public:
		CFont();
		CFont(std::string fontName);
		~CFont();

	public:
		void CreateFont();
	};
}

