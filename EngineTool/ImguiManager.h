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
	void Update_Transform_Component(int index);
	void Update_AudioListener_Component(int index);
	void Update_AudioSource_Component(int index);
	void Update_Camera_Component(int index);
	void Update_Light_Component(int index);
	void Update_MeshRenderer_Component(int index);
	void Update_BoxCollider_Component(int index);
	void Update_CapsuleCollider_Component(int index);
	void Update_SphereCollider_Component(int index);
	void Update_Rigidbody_Component(int index);
};

