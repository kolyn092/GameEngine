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

ImguiManager* ImguiManager::m_pInstance = nullptr;

ImguiManager::ImguiManager()
{

}

ImguiManager::~ImguiManager()
{
	if (m_defaultScene)
	{
		delete m_defaultScene;
		m_defaultScene = nullptr;
	}
}

void ImguiManager::Initalize()
{
	/// //////////////////////////////////////////////////////////////////////////
	/// ������Ʈ ���� ���� (�⺻ Scene ����)
	/// //////////////////////////////////////////////////////////////////////////
	
	/// [ Default Font Setting ]
	ComponentEngine::CFont* _defaultFont = new ComponentEngine::CFont("DefaultFont");
	_defaultFont->m_FilePath = "../EX/Font/RoundedTypeface.sfont";
	_defaultFont->CreateFont();

	/// [ Create Default Scene ]
	m_defaultScene = new ComponentEngine::Scene();
	m_defaultScene->SetInputMouseLock(false);

	/// [ Default Light Setting ] 
	ComponentEngine::GameObject* _lightObj = new ComponentEngine::GameObject("light");
	// Light ������Ʈ�� Transform ���� (ȸ����)
	_lightObj->GetComponent<ComponentEngine::Transform>()->SetRotate(Vector3(50, -30, 0));
	// Light Component �߰� �� �÷� ����
	ComponentEngine::CLight* _light = new ComponentEngine::CLight();
	_light->SetColor(Color(1, 1, 1, 1));
	_light->SetFogActive(false);
	_lightObj->AddComponent<ComponentEngine::CLight*>(_light);
	// Scene�� �ش� ������Ʈ ���
	m_defaultScene->AddGameObject(_lightObj);

	/// [ Default Skybox Object Setting ]
	ComponentEngine::GameObject* _skyboxObj = new ComponentEngine::GameObject("skybox");
	// Skybox ������Ʈ �߰� �� ���ҽ� ���� ��� ����
	ComponentEngine::Skybox* _skybox = new ComponentEngine::Skybox();
	_skybox->m_FilePath = "..\\Resource\\Skybox\\SkyBox3_1.dds";
	_skyboxObj->AddComponent<ComponentEngine::Skybox*>(_skybox);
	// Scene�� �ش� ������Ʈ ���
	m_defaultScene->AddGameObject(_skyboxObj);

	/// [ Default Camera Object Setting ]
	ComponentEngine::GameObject* _cameraObj = new ComponentEngine::GameObject("camera");
	// Camera ������Ʈ Transform ���� (��ġ, ȸ��)
	_cameraObj->GetComponent<ComponentEngine::Transform>()->SetPosition(Vector3(0, 9, -9));
	_cameraObj->GetComponent<ComponentEngine::Transform>()->SetRotate(30, 0, 0);
	// Camera ������Ʈ �߰� �� ����
	ComponentEngine::CCamera* _camera = new ComponentEngine::CCamera();
	_camera->SetCameraMode(ComponentEngine::CCamera::eMode::FOLLOW);
	_cameraObj->AddComponent<ComponentEngine::CCamera*>(_camera);
	// Scene�� �ش� ������Ʈ ���
	m_defaultScene->AddGameObject(_cameraObj);
	// Scene�� ���� ī�޶�� ���
	m_defaultScene->SetMainCamera(_camera);

	/// [ Add Default Scene ]
	ComponentEngine::SceneManager::Ins()->AddScene(m_defaultScene);

	/// //////////////////////////////////////////////////////////////////////////
	/// ImGUI Setting
	/// //////////////////////////////////////////////////////////////////////////
	

}

void ImguiManager::Finalize()
{
	if (m_pInstance)
	{
		delete m_pInstance;
		m_pInstance = nullptr;
	}
}
