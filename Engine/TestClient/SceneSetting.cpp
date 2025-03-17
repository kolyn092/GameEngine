#include "SceneSetting.h"
#include "Scene.h"
#include "SceneManager.h"
#include "CCamera.h"
#include "MeshRenderer.h"
#include "Transform.h"
#include "CSkybox.h"
#include "CTerrain.h"
#include "CLight.h"
#include "CFont.h"

#include "Player.h"

SceneSetting::SceneSetting()
{
	CreateScene();
}

SceneSetting::~SceneSetting()
{

}

void SceneSetting::CreateScene()
{
	/// [ font ���� ]
	ComponentEngine::CFont* _font0 = new ComponentEngine::CFont("DefaultFont");
	_font0->m_FilePath = "../EX/Font/RoundedTypeface.sfont";
	_font0->CreateFont();
	 
	/// [ Scene ���� ]
	ComponentEngine::Scene* _scene = new ComponentEngine::Scene();
	// ���콺 Ȱ��ȭ
	_scene->SetInputMouseLock(false);
	
	/// //////////////////////////////////////////////////////////////////////////
	
	/// [ Light ������Ʈ ���� ]
	ComponentEngine::GameObject* _lightObj = new ComponentEngine::GameObject("light");
	// Light ������Ʈ�� Transform ���� (ȸ����)
	_lightObj->GetComponent<ComponentEngine::Transform>()->SetRotate(Vector3(50, -30, 0));
	// Light Component �߰� �� �÷� ����
	ComponentEngine::CLight* _light = new ComponentEngine::CLight();
	_light->SetColor(Color(1, 1, 1, 1));
	_light->SetFogActive(false);
	_lightObj->AddComponent<ComponentEngine::CLight*>(_light);
	// Scene�� �ش� ������Ʈ ���
	_scene->AddGameObject(_lightObj);
	
	/// //////////////////////////////////////////////////////////////////////////
	
	/// [ Skybox ������Ʈ ���� ]
	ComponentEngine::GameObject* _skyboxObj = new ComponentEngine::GameObject("skybox");
	// Skybox ������Ʈ �߰� �� ���ҽ� ���� ��� ����
	ComponentEngine::Skybox* _skybox = new ComponentEngine::Skybox();
	_skybox->m_FilePath = "..\\Resource\\Skybox\\SkyBox3_1.dds";
	_skyboxObj->AddComponent<ComponentEngine::Skybox*>(_skybox);
	// Scene�� �ش� ������Ʈ ���
	_scene->AddGameObject(_skyboxObj);
	
	/// //////////////////////////////////////////////////////////////////////////
	
	/// [ Floor ������Ʈ ���� ]
	ComponentEngine::GameObject* _floorObj = new ComponentEngine::GameObject("floor");
	_floorObj->GetComponent<ComponentEngine::Transform>()->SetScale(Vector3(1, 1, 1));
	//// Terrain ������Ʈ �߰� �� ���ҽ� ���� ��� ����
	//ComponentEngine::CTerrain* _terrain = new ComponentEngine::CTerrain();
	//_terrain->m_FilePath = "../Resource/floor/floor.tif";
	//_floorObj->AddComponent<ComponentEngine::CTerrain*>(_terrain);
	// Scene�� �ش� ������Ʈ ���
	_scene->AddGameObject(_floorObj);
	
	/// //////////////////////////////////////////////////////////////////////////
	
	/// [ Player ������Ʈ ���� ]
	ComponentEngine::GameObject* _playerObj = new ComponentEngine::GameObject("Player");
	// �÷��̾� ������Ʈ Transform ���� (��ġ, ũ��)
	_playerObj->GetComponent<ComponentEngine::Transform>()->SetPosition(Vector3(0, 3, 0));
	_playerObj->GetComponent<ComponentEngine::Transform>()->SetScale(DirectX::SimpleMath::Vector3(3, 3, 3));
	// �÷��̾� ������Ʈ Tag ���� (�浹üũ��)
	_playerObj->SetTag("Player");
	// MeshRenderer ������Ʈ �߰� �� FBX ���� ��� ����
	// ����� �ִϸ��̼��� ��� FBX ���� ��θ� m_ModelPathList�� �߰����־�� �Ѵ�.
	ComponentEngine::MeshRenderer* _meshrenderer = new ComponentEngine::MeshRenderer();
	_meshrenderer->m_ModelPathList.push_back("..\\Resource\\Devil2\\Devil_Idle_Normal.FBX");
	_meshrenderer->m_ModelPathList.push_back("..\\Resource\\Devil2\\Devil_MoveFront.FBX");
	_meshrenderer->m_ModelPathList.push_back("..\\Resource\\Devil2\\Devil_MoveBack.FBX");
	_meshrenderer->m_ModelPathList.push_back("..\\Resource\\Devil2\\Devil_MoveRight.FBX");
	_meshrenderer->m_ModelPathList.push_back("..\\Resource\\Devil2\\Devil_MoveLeft.FBX");
	_meshrenderer->m_ModelPathList.push_back("..\\Resource\\Devil2\\Devil_Run.FBX");
	_meshrenderer->m_ModelPathList.push_back("..\\Resource\\Devil2\\Devil_PowerAttack1.FBX");
	_meshrenderer->m_ModelPathList.push_back("..\\Resource\\Devil2\\Devil_PowerAttack2.FBX");
	_playerObj->AddComponent<ComponentEngine::MeshRenderer*>(_meshrenderer);
	// Player ��ũ��Ʈ �߰�
	Player* _player = new Player();
	_playerObj->AddComponent<Player*>(_player);
	// Scene�� �ش� ������Ʈ ���
	_scene->AddGameObject(_playerObj);
	
	/// [ Player�� ���� ī�޶� Ÿ�ٿ� ������Ʈ ���� ]
	ComponentEngine::GameObject* _cameraTargetObj = new ComponentEngine::GameObject("CameraTargetObj");
	// Player ������Ʈ�� �ڽ����� ������Ʈ�� ���δ�.
	ComponentEngine::Transform* _transform = _cameraTargetObj->GetComponent<ComponentEngine::Transform>();
	_playerObj->GetComponent<ComponentEngine::Transform>()->AttatchChild(_transform);
	// �÷��̾��� ��ġ���� ���� �ٶ� �� �ְ� ��ġ�� �̵���Ų��.
	_transform->Translate(_transform->m_Forward * 5);
	// Scene�� �ش� ������Ʈ ���
	_scene->AddGameObject(_cameraTargetObj);

	/// //////////////////////////////////////////////////////////////////////////

	/// [ Camera ������Ʈ ���� ]
	ComponentEngine::GameObject* _cameraObj = new ComponentEngine::GameObject("camera");
	// Camera ������Ʈ Transform ���� (��ġ, ȸ��)
	_cameraObj->GetComponent<ComponentEngine::Transform>()->SetPosition(Vector3(0, 9, -9));
	_cameraObj->GetComponent<ComponentEngine::Transform>()->SetRotate(30, 0, 0);
	// Camera ������Ʈ �߰� �� ����
	ComponentEngine::CCamera* _camera = new ComponentEngine::CCamera();
	_camera->SetCameraMode(ComponentEngine::CCamera::eMode::FOLLOW);
	_cameraObj->AddComponent<ComponentEngine::CCamera*>(_camera);
	// Scene�� �ش� ������Ʈ ���
	_scene->AddGameObject(_cameraObj);
	// Scene�� ���� ī�޶�� ���
	_scene->SetMainCamera(_camera);

	///  //////////////////////////////////////////////////////////////////////////
	/// [ Scene ��� ]
	ComponentEngine::SceneManager::Ins()->AddScene(_scene);
}
