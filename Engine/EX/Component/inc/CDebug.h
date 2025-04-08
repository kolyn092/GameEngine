#pragma once

namespace ComponentEngine
{
	class CDebug
	{

	private:
		int m_DebugPosX;
		int m_DebugPosY;

		int m_DebugPosX_Default = 10;
		int m_DebugPosY_Default = 10;

		int m_MaxDebugPosY = 1000;

		DirectX::SimpleMath::Vector4 m_Color;

		bool m_isActive;

#pragma region SINGLETON
	private:
		static CDebug* m_pInstance;

	public:
		static CDebug* Ins()
		{
			if (m_pInstance == nullptr)
			{
				m_pInstance = new CDebug();
			}
			return m_pInstance;
		}
#pragma endregion

	private:
		CDebug();
		~CDebug();

	public:
		void Reset();
		void Spacing();
		void DebugDrawText(int x, int y, const TCHAR* text, ...);
		void DebugDrawText(const TCHAR* text, ...);
		void Finalize();

		void SetColor(DirectX::SimpleMath::Vector4 color);
		void SetActive(bool value);
	};
}

