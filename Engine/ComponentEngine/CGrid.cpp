#include "pch.h"
#include "CGrid.h"
#include "CCamera.h"
#include "CDebug.h"

ComponentEngine::CGrid::CGrid()
	: m_Grid(nullptr), m_ActiveState(true)
{
	GRAPHICENGINE::CreateGird_Debug(&m_Grid);
	m_Grid->InitializeGrid(10, 10, 1);
}

ComponentEngine::CGrid::~CGrid()
{
	if (m_Grid != nullptr)
	{
		delete m_Grid;
		m_Grid = nullptr;
	}
}

void ComponentEngine::CGrid::Update()
{
	if (m_ActiveState)
	{
		m_Grid->UpdateGrid();
	}
}

void ComponentEngine::CGrid::LateUpdate(float dTime)
{

}

void ComponentEngine::CGrid::Update(float dTime)
{

}

void ComponentEngine::CGrid::OnPreRender()
{

}

void ComponentEngine::CGrid::OnRender()
{
	if (m_ActiveState)
	{
		m_Grid->DrawGrid();
	}
}

void ComponentEngine::CGrid::OnPostRender()
{

}

void ComponentEngine::CGrid::OnDrawGizmos()
{

}

void ComponentEngine::CGrid::DebugTextRender()
{
	CDebug::Ins()->DebugDrawText(L"%s %s", L"L", L"CGrid");
}

void ComponentEngine::CGrid::OnQuit()
{

}

void ComponentEngine::CGrid::OnDisable()
{

}

void ComponentEngine::CGrid::OnDestroy()
{

}

void ComponentEngine::CGrid::SetActive(bool value)
{
	if (value == true && m_ActiveState != value)
	{
		Start();
	}
	m_ActiveState = value;
}
