#pragma once
#include "SceneManager.h"
#include "CTimer.h"
#include <d3d11.h>
#include "EngineInterface.h"

namespace ComponentEngine
{
	////////////////////////////////////////////////////////////////////////////
	// 
	// ������Ʈ ������ ���������� �����ϴ� �ý��� Ŭ����
	// 	- ������ ���������� ������ ���������� ���ư��� �־�� �Ѵ�.
	// 
	////////////////////////////////////////////////////////////////////////////

	class CESystem
	{
	private:
		// ������Ʈ ���� �������ֱ� ���� �Ŵ���
		SceneManager* m_SceneManager;

		// ������Ʈ ���ο��� ������ Ÿ�̸�
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

