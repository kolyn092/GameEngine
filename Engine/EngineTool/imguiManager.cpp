#include "ImguiManager.h"
#include "Scene.h"
#include "SceneManager.h"
#include "CCamera.h"
#include "MeshRenderer.h"
#include "Transform.h"
#include "CSkybox.h"
#include "CTerrain.h"
#include "CLight.h"
#include "CFont.h"
#include <imgui.h>

ImguiManager* ImguiManager::m_pInstance = nullptr;

ImguiManager::ImguiManager()
	: m_DefaultScene(nullptr)
{

}

ImguiManager::~ImguiManager()
{
	if (m_DefaultScene)
	{
		delete m_DefaultScene;
		m_DefaultScene = nullptr;
	}
}

void ImguiManager::Initalize()
{
	/// //////////////////////////////////////////////////////////////////////////
	/// 컴포넌트 엔진 설정 (기본 Scene 세팅)
	/// //////////////////////////////////////////////////////////////////////////

	/// [ Default Font Setting ]
	ComponentEngine::CFont* _defaultFont = new ComponentEngine::CFont("DefaultFont");
	_defaultFont->m_FilePath = "../EX/Font/RoundedTypeface.sfont";
	_defaultFont->CreateFont();

	/// [ Create Default Scene ]
	m_DefaultScene = new ComponentEngine::Scene();
	m_DefaultScene->SetInputMouseLock(false);

	/// [ Default Light Setting ] 
	ComponentEngine::GameObject* _lightObj = new ComponentEngine::GameObject("light");
	// Light 오브젝트의 Transform 설정 (회전값)
	_lightObj->GetComponent<ComponentEngine::Transform>()->SetRotate(Vector3(50, -30, 0));
	// Light Component 추가 및 컬러 세팅
	ComponentEngine::CLight* _light = new ComponentEngine::CLight();
	_light->SetColor(Color(1, 1, 1, 1));
	_light->SetFogActive(false);
	_lightObj->AddComponent<ComponentEngine::CLight*>(_light);
	// Scene에 해당 오브젝트 등록
	m_DefaultScene->AddGameObject(_lightObj);

	/// [ Default Skybox Object Setting ]
	ComponentEngine::GameObject* _skyboxObj = new ComponentEngine::GameObject("skybox");
	// Skybox 컴포넌트 추가 및 리소스 파일 경로 설정
	ComponentEngine::Skybox* _skybox = new ComponentEngine::Skybox();
	_skybox->m_FilePath = "..\\Resource\\Skybox\\SkyBox3_1.dds";
	_skyboxObj->AddComponent<ComponentEngine::Skybox*>(_skybox);
	// Scene에 해당 오브젝트 등록
	m_DefaultScene->AddGameObject(_skyboxObj);

	/// [ Default Camera Object Setting ]
	ComponentEngine::GameObject* _cameraObj = new ComponentEngine::GameObject("camera");
	// Camera 오브젝트 Transform 설정 (위치, 회전)
	_cameraObj->GetComponent<ComponentEngine::Transform>()->SetPosition(Vector3(0, 9, -9));
	_cameraObj->GetComponent<ComponentEngine::Transform>()->SetRotate(30, 0, 0);
	// Camera 컴포넌트 추가 및 설정
	ComponentEngine::CCamera* _camera = new ComponentEngine::CCamera();
	_camera->SetCameraMode(ComponentEngine::CCamera::eMode::FOLLOW);
	_cameraObj->AddComponent<ComponentEngine::CCamera*>(_camera);
	// Scene에 해당 오브젝트 등록
	m_DefaultScene->AddGameObject(_cameraObj);
	// Scene의 메인 카메라로 등록
	m_DefaultScene->SetMainCamera(_camera);

	/// [ Add Default Scene ]
	ComponentEngine::SceneManager::Ins()->AddScene(m_DefaultScene);
}

void ImguiManager::Update()
{
	/// //////////////////////////////////////////////////////////////////////////
	/// ImGUI Setting
	/// //////////////////////////////////////////////////////////////////////////
	
	ImGui::Begin("Hierarchy");
	auto gameObjects = m_DefaultScene->GetRootObjects();
	for (auto obj : gameObjects)
	{
		auto objActiveState = obj->isActive();
		if (ImGui::Button(obj->GetName().c_str(), ImVec2(100,0)))
		{
			// 자식 오브젝트 표시
		}
		//mGui::Begin(obj->GetName().c_str());
		//auto components = obj->GetAllComponents();
		//for (auto com : components)
		//{
		//	ImGui::Text(com->GetComponentName().c_str());
		//	//ImGui::Checkbox("Active", &show_demo_window);
		//}
		//ImGui::End();

		/// Detach 관련해서 Root 오브젝트와 Child 오브젝트 추가/삭제 수정
	}
	ImGui::End();
}

void ImguiManager::Finalize()
{
	if (m_pInstance)
	{
		delete m_pInstance;
		m_pInstance = nullptr;
	}
}
