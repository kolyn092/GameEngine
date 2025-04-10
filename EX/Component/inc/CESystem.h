#pragma once
#include "SceneManager.h"
#include "CTimer.h"
#include <d3d11.h>
#include "EngineInterface.h"

namespace ComponentEngine
{
	////////////////////////////////////////////////////////////////////////////
	// 
	// 컴포넌트 엔진을 전반적으로 관리하는 시스템 클래스
	// 	- 물리를 서버에서만 돌려도 물리엔진은 돌아가고 있어야 한다.
	// 
	////////////////////////////////////////////////////////////////////////////

	class CESystem
	{
	private:
		// 컴포넌트 씬을 관리해주기 위한 매니저
		SceneManager* m_SceneManager;

		// 컴포넌트 내부에서 돌리는 타이머
		ComponentTimer* m_Timer;
		float m_dTime;

#pragma region SINGLETON
	private:
		static CESystem* m_pInstance;

	public:
		static CESystem* Ins()
		{
			if (m_pInstance == nullptr)
			{
				m_pInstance = new CESystem();
			}
			return m_pInstance;
		}
#pragma endregion

	private:
		CESystem();
		~CESystem();
		
	public:
		void Initalize(HWND hWnd);
		void Start();
		void Update();
		void BeginDraw();
		void Draw();
		void EndDraw();
		void Finalize();

	public:
		ID3D11Device* GetDevice() { return GRAPHICENGINE::GetD11Device(); }
		ID3D11DeviceContext* GetDeviceContext() { return GRAPHICENGINE::GetD11DC(); }
	};
}

