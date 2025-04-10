#pragma once
#include <vector>
#include "Scene.h"

namespace ComponentEngine
{
	class SceneManager
	{
	private:
		std::vector<Scene*> m_SceneList;
		int m_PreScene;
		int m_NowScene;

#pragma region SINGLETON
	private:
		static SceneManager* m_pInstance;

	public:
		static SceneManager* Ins()
		{
			if (m_pInstance == nullptr)
			{
				m_pInstance = new SceneManager();
			}
			return m_pInstance;
		}
#pragma endregion

	private:
		SceneManager();
		~SceneManager();

	public:
		void AddScene(Scene* scene);
		void DeleteScene(unsigned int num);
		void LoadScene(unsigned int num);
		void StartScene();
		void UpdateScene(float dTime);
		void DrawScene();
		void Finalize();

		CECamera* GetNowSceneCamera() const { return m_SceneList[m_NowScene]->GetMainCamera(); }

		int GetNowSceneIndex() const { return m_NowScene; }
		void SetNowSceneIndex(int index);

		Scene* GetScene(int index) { return m_SceneList[index]; }
		std::vector<Scene*> GetSceneList() { return m_SceneList; }
		int GetSceneListCount() { return m_SceneList.size(); }
	};
}
