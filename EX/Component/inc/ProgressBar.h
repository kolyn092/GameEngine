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
		float m_MaxValue;							// �ʱ� Max �� (���� ������ �� �� ���� ����)
		float m_UserValue;							// ������ ������ �� (������ �����ϰ�, ������ ������ ������)
		float m_NowValue;							// ���α׷��� �� ���� ���� ��

		float m_DecreaseValue;						// ���� �� (default 1)

		eProgressState m_ProgressState;				// ����, ���� ����
		eProgressDirection m_ProgressDirection;		// ���� ���� (���� , �¿�)

		bool m_isSlowProgress;						// ������ �����ϴ� ���α׷��������� ���� ����

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
