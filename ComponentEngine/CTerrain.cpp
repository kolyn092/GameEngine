#include "pch.h"
#include "CTerrain.h"

ComponentEngine::CTerrain::CTerrain()
	: Component("Terrain"), m_FilePath(""), m_Terrain(nullptr)
{

}

ComponentEngine::CTerrain::~CTerrain()
{
	if (m_Terrain)
	{
		delete m_Terrain;
		m_Terrain = nullptr;
	}
}

void ComponentEngine::CTerrain::Awake()
{
	if (!m_FilePath.empty())
	{
		GRAPHICENGINE::CreateTerrain_Obj(&m_Terrain, m_FilePath, 200, 200, 50);
		if (m_Terrain)
		{
			m_Terrain->InitTerrain();
		}
	}
}

void ComponentEngine::CTerrain::Update()
{
	m_Terrain->Update();
}

void ComponentEngine::CTerrain::DebugTextRender()
{

}

void ComponentEngine::CTerrain::OnQuit()
{

}

void ComponentEngine::CTerrain::OnDisable()
{

}

void ComponentEngine::CTerrain::OnDestroy()
{

}

void ComponentEngine::CTerrain::SetActive(bool value)
{

}

void ComponentEngine::CTerrain::Bind(GameObject* obj)
{

}
