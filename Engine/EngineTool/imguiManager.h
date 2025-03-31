#pragma once
#include "Scene.h"

class ImguiManager
{
private:
	ComponentEngine::Scene* m_DefaultScene;

#pragma region SINGLETON
private:
	static ImguiManager* m_pInstance;

public:
	static ImguiManager* Ins()
	{
		if (m_pInstance == nullptr)
		{
			m_pInstance = new ImguiManager();
		}
		return m_pInstance;
	}
#pragma endregion

private:
	ImguiManager();
	~ImguiManager();

public:
	void Initalize();
	void Update();
	void Finalize();

};

