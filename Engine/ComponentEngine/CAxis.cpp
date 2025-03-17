#include "pch.h"
#include "CAxis.h"
#include "CCamera.h"
#include "CDebug.h"

ComponentEngine::CAxis::CAxis()
	: m_Axis(nullptr), m_ActiveState(true)
{
	m_Axis = new Axis();
	m_Axis->InitAxis();
}

ComponentEngine::CAxis::~CAxis()
{
	if(m_Axis != nullptr)
	{
		delete m_Axis;
		m_Axis = nullptr;
	}
}

void ComponentEngine::CAxis::OnEnable()
{

}

void ComponentEngine::CAxis::Update()
{
	if (m_ActiveState)
	{
		// TODO : Camera 관련 수정
		///CCamera* _cam = m_GameObject->GetComponent<CCamera>();
		///m_Axis->Update(_cam->GetViewMatrix(), _cam->GetProjMatrix());
	}
}

void ComponentEngine::CAxis::LateUpdate(float dTime)
{

}

void ComponentEngine::CAxis::Update(float dTime)
{

}

void ComponentEngine::CAxis::OnPreRender()
{

}

void ComponentEngine::CAxis::OnRender()
{
	if (m_ActiveState)
	{
		m_Axis->Render();
	}
}

void ComponentEngine::CAxis::OnPostRender()
{

}

void ComponentEngine::CAxis::OnDrawGizmos()
{

}

void ComponentEngine::CAxis::DebugTextRender()
{
	CDebug::Ins()->DebugDrawText(L"%s %s", L"L", L"CAxis");
}

void ComponentEngine::CAxis::OnQuit()
{

}

void ComponentEngine::CAxis::OnDisable()
{

}

void ComponentEngine::CAxis::OnDestroy()
{

}

void ComponentEngine::CAxis::SetActive(bool value)
{
	if (value == true && m_ActiveState != value)
	{
		Start();
	}
	m_ActiveState = value;
}
