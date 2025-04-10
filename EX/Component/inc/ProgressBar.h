#pragma once
#include "Component.h"

namespace ComponentEngine
{
	class ProgressBar : public Component
	{
	public:
		enum class eProgressState
		{
			Increase, Decrease,
		};

		enum class eProgressDirection
		{
			Horizontal, Vertical,
		};

	private:
		float m_MaxValue;							// 초기 Max 값 (게임 시작할 때 한 번만 설정)
		float m_UserValue;							// 유저가 세팅할 값 (어디까지 감소하고, 어디까지 증가할 것인지)
		float m_NowValue;							// 프로그래스 바 내부 현재 값

		float m_DecreaseValue;						// 감소 값 (default 1)

		eProgressState m_ProgressState;				// 증가, 감소 설정
		eProgressDirection m_ProgressDirection;		// 방향 설정 (상하 , 좌우)

		bool m_isSlowProgress;						// 서서히 감소하는 프로그래스바인지 여부 설정

		bool m_ActiveState;

	public:
		ProgressBar();
		virtual ~ProgressBar();

	public:
		void SetMaxValue(float val);
		void SetUserValue(float val);
		void SetDecreaseValue(float val);
		void SetProgressState(eProgressState state) { m_ProgressState = state; }
		void SetProgressDirection(eProgressDirection dir) { m_ProgressDirection = dir; }
		void SetSlowProgress(bool val) { m_isSlowProgress = val; }

		float GetMaxValue() const { return m_MaxValue; }
		float GetUserValue() const { return m_UserValue; }
		float GetNowValue() const { return m_NowValue; }
		eProgressDirection GetDirection() const { return m_ProgressDirection; }
		bool IsProgressing();

	public:
		virtual void Awake() override;

		virtual void Update(float dTime) override;

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
