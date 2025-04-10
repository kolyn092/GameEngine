#pragma once

namespace ComponentEngine
{
	class CFont;
	class FontManager
	{

	private:
		std::vector<CFont*> m_FontList;

#pragma region SINGLETON
	private:
		static FontManager* m_pInstance;

	public:
		static FontManager* Ins()
		{
			if (m_pInstance == nullptr)
			{
				m_pInstance = new FontManager();
			}
			return m_pInstance;
		}
#pragma endregion

	private:
		FontManager();
		~FontManager();

	public:
		void AddFont(CFont* font);
		CFont* FindFontByName(std::string fontName);
		void Finalize();
	};
}

