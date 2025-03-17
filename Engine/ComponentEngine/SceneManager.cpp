#include "pch.h"
#include "SceneManager.h"

ComponentEngine::SceneManager* ComponentEngine::SceneManager::m_pInstance = nullptr;

ComponentEngine::SceneManager::SceneManager()
	: m_NowScene(0)
{

}

ComponentEngine::SceneManager::~SceneManager()
{

}

void ComponentEngine::SceneManager::AddScene(Scene* scene)
{
	m_SceneList.push_back(scene);
}

void ComponentEngine::SceneManager::DeleteScene(unsigned int num)
{
	std::vector<Scene*>::iterator it = m_SceneList.begin();
	if (num < m_SceneList.size())
	{
		it += num;
		//for (int i = 0; i < num + 1; i++, ++it)
		//{
		//
		//}

		delete m_SceneList[num];
		m_SceneList.erase(it);
	}
}

void ComponentEngine::SceneManager::LoadScene(unsigned int num)
{
	if (m_SceneList.size() > num && 0 <= num)
	{
		m_PreScene = m_NowScene;
		if (m_NowScene != 0)
		{
			m_SceneList[m_PreScene]->OnDestroy();
		}
		m_NowScene = num;
		m_SceneList[m_NowScene]->LoadFbxFile();
		m_SceneList[m_NowScene]->SetFbxModel();
		m_SceneList[m_NowScene]->Awake();
		m_SceneList[m_NowScene]->OnEnable();
		m_SceneList[m_NowScene]->Start();
	}
}

void ComponentEngine::SceneManager::StartScene()
{
	if (m_SceneList.size() > 0)
	{
		if (m_SceneList[m_NowScene]->GetLoadComplete())
		{
			m_SceneList[m_NowScene]->Start();
		}
	}
}

void ComponentEngine::SceneManager::UpdateScene(float dTime)
{
	if (m_SceneList.size() > 0)
	{
		if (m_SceneList[m_NowScene]->GetLoadComplete())
		{
			m_SceneList[m_NowScene]->Update(dTime);
		}
	}
}

void ComponentEngine::SceneManager::DrawScene()
{
	if (m_SceneList.size() > 0)
	{
		if (m_SceneList[m_NowScene]->GetLoadComplete())
		{
			m_SceneList[m_NowScene]->Render();
		}
	}
}

void ComponentEngine::SceneManager::Finalize()
{
	for (auto& scene : m_SceneList)
	{
		if (scene)
		{
			delete scene;
			scene = nullptr;
		}
	}

	if (m_pInstance)
	{
		delete m_pInstance;
		m_pInstance = nullptr;
	}
}

void ComponentEngine::SceneManager::SetNowSceneIndex(int index)
{
	m_PreScene = m_NowScene;

	m_NowScene = index;
}
