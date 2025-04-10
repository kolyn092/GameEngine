#include "pch.h"
#include "ProgressBar.h"
#include "Image.h"
#include "CDebug.h"

ComponentEngine::ProgressBar::ProgressBar()
	: Component("ProgressBar"), m_MaxValue(5.0f), m_UserValue(5.0f), m_NowValue(5.0f), m_DecreaseValue(1.0f),
	m_ProgressState(eProgressState::Decrease), m_ProgressDirection(eProgressDirection::Horizontal),
	m_isSlowProgress(false), m_ActiveState(true)
{
	// ���ο��� �ڵ����� ���̰ų� �ϴ°� �Ұ���. �׻� Active �Ǿ� �־�� �ϴ� ������Ʈ�� �ֱ� ����.
	// Progress Bar ���ο����� ���Ұ� ���� �� ������ �ٰ� �ϸ� �ɵ�. (�̰͵� ���û������� �ּ� ������ ������ �� �ְ�?)
}

ComponentEngine::ProgressBar::~ProgressBar()
{

}

void ComponentEngine::ProgressBar::SetMaxValue(float val)
{
	if (val >= 0)
	{
		m_MaxValue = val;
	}
}

void ComponentEngine::ProgressBar::SetUserValue(float val)
{
	if (0 <= val && val <= m_MaxValue)
	{
		m_UserValue = val;
	}

	if (0 > m_UserValue)
	{
		m_UserValue = 0;
	}

	if (m_UserValue > m_MaxValue)
	{
		m_UserValue = m_MaxValue;
	}
}

void ComponentEngine::ProgressBar::SetDecreaseValue(float val)
{
	if (val >= 0)
	{
		m_DecreaseValue = val;
	}
}

bool ComponentEngine::ProgressBar::IsProgressing()
{
	if (m_NowValue == m_UserValue)
	{
		return true;
	}
	return false;
}

void ComponentEngine::ProgressBar::Awake()
{
	m_NowValue = m_UserValue;
}

void ComponentEngine::ProgressBar::Update(float dTime)
{
	if (m_isSlowProgress)
	{
		// ������ ����
		switch (m_ProgressState)
		{
			case ComponentEngine::ProgressBar::eProgressState::Increase:
			{
				// ������ ������ ������ ���� ���
				if (m_NowValue < m_UserValue)
				{
					m_NowValue += m_DecreaseValue * dTime;
				}
				else
				{
					m_NowValue = m_UserValue;
				}
			}
			break;
			case ComponentEngine::ProgressBar::eProgressState::Decrease:
			{
				// ������ ������ ������ Ŭ ���
				if (m_NowValue > m_UserValue)
				{
					m_NowValue -= m_DecreaseValue * dTime;
				}
				else
				{
					m_NowValue = m_UserValue;
				}
			}
			break;
			default:
				break;
		}
	}
	else
	{
		// �׳� ����
		m_NowValue = m_UserValue;
	}

	ComponentEngine::UI_Image* _image = m_GameObject->GetComponent<ComponentEngine::UI_Image>();
	_image->m_FillAmount = (1.0f / m_MaxValue) * m_NowValue;
}

void ComponentEngine::ProgressBar::DebugTextRender()
{
	//CDebug::Ins()->DebugDrawText(L"%s %s", L"L", L"UI_ProgresBar");
	//CDebug::Ins()->DebugDrawText(L"Max Value : %.f", m_MaxValue);
	//CDebug::Ins()->DebugDrawText(L"User Value : %.f", m_UserValue);
	//CDebug::Ins()->DebugDrawText(L"Now Value : %.f", m_NowValue);
}

void ComponentEngine::ProgressBar::OnQuit()
{

}

void ComponentEngine::ProgressBar::OnDisable()
{

}

void ComponentEngine::ProgressBar::OnDestroy()
{

}

void ComponentEngine::ProgressBar::SetActive(bool value)
{

}

void ComponentEngine::ProgressBar::Bind(GameObject* obj)
{
	if (obj)
	{
		obj->m_ProgressBar = this;
	}
}

