#pragma once
#include "Scene.h"

class ImguiManager
{
private:
	ComponentEngine::Scene* m_NowScene;
	ComponentEngine::GameObject* m_SelectedObject;

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

private:
	void ShowGameObjectHierarchy(ComponentEngine::GameObject* obj);
	void ShowObjectDetails();
	void Update_AudioListener_Component();
	void Update_AudioSource_Component();
	void Update_Camera_Component();
	void Update_Light_Component();
	void Update_MeshRenderer_Component();
	void Update_BoxCollider_Component();
	void Update_CapsuleCollider_Component();
	void Update_SphereCollider_Component();
	void Update_Rigidbody_Component();
};

