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
	/// [ font 생성 ]
	ComponentEngine::CFont* _font0 = new ComponentEngine::CFont("DefaultFont");
	_font0->m_FilePath = "../EX/Font/RoundedTypeface.sfont";
	_font0->CreateFont();
	 
	/// [ Scene 생성 ]
	ComponentEngine::Scene* _scene = new ComponentEngine::Scene();
	// 마우스 활성화
	_scene->SetInputMouseLock(false);
	
	/// //////////////////////////////////////////////////////////////////////////
	
	/// [ Light 오브젝트 생성 ]
	ComponentEngine::GameObject* _lightObj = new ComponentEngine::GameObject("light");
	// Light 오브젝트의 Transform 설정 (회전값)
	_lightObj->GetComponent<ComponentEngine::Transform>()->SetRotate(Vector3(50, -30, 0));
	// Light Component 추가 및 컬러 세팅
	ComponentEngine::CLight* _light = new ComponentEngine::CLight();
	_light->SetColor(Color(1, 1, 1, 1));
	_light->SetFogActive(false);
	_lightObj->AddComponent<ComponentEngine::CLight*>(_light);
	// Scene에 해당 오브젝트 등록
	_scene->AddGameObject(_lightObj);
	
	/// //////////////////////////////////////////////////////////////////////////
	
	/// [ Skybox 오브젝트 생성 ]
	ComponentEngine::GameObject* _skyboxObj = new ComponentEngine::GameObject("skybox");
	// Skybox 컴포넌트 추가 및 리소스 파일 경로 설정
	ComponentEngine::Skybox* _skybox = new ComponentEngine::Skybox();
	_skybox->m_FilePath = "..\\Resource\\Skybox\\SkyBox3_1.dds";
	_skyboxObj->AddComponent<ComponentEngine::Skybox*>(_skybox);
	// Scene에 해당 오브젝트 등록
	_scene->AddGameObject(_skyboxObj);
	
	/// //////////////////////////////////////////////////////////////////////////
	
	/// [ Floor 오브젝트 생성 ]
	ComponentEngine::GameObject* _floorObj = new ComponentEngine::GameObject("floor");
	_floorObj->GetComponent<ComponentEngine::Transform>()->SetScale(Vector3(1, 1, 1));
	//// Terrain 컴포넌트 추가 및 리소스 파일 경로 설정
	//ComponentEngine::CTerrain* _terrain = new ComponentEngine::CTerrain();
	//_terrain->m_FilePath = "../Resource/floor/floor.tif";
	//_floorObj->AddComponent<ComponentEngine::CTerrain*>(_terrain);
	// Scene에 해당 오브젝트 등록
	_scene->AddGameObject(_floorObj);
	
	/// //////////////////////////////////////////////////////////////////////////
	
	/// [ Player 오브젝트 생성 ]
	ComponentEngine::GameObject* _playerObj = new ComponentEngine::GameObject("Player");
	// 플레이어 오브젝트 Transform 설정 (위치, 크기)
	_playerObj->GetComponent<ComponentEngine::Transform>()->SetPosition(Vector3(0, 3, 0));
	_playerObj->GetComponent<ComponentEngine::Transform>()->SetScale(DirectX::SimpleMath::Vector3(3, 3, 3));
	// 플레이어 오브젝트 Tag 설정 (충돌체크용)
	_playerObj->SetTag("Player");
	// MeshRenderer 컴포넌트 추가 및 FBX 파일 경로 설정
	// 사용할 애니메이션의 모든 FBX 파일 경로를 m_ModelPathList에 추가해주어야 한다.
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
	// Player 스크립트 추가
	Player* _player = new Player();
	_playerObj->AddComponent<Player*>(_player);
	// Scene에 해당 오브젝트 등록
	_scene->AddGameObject(_playerObj);
	
	/// [ Player를 위한 카메라 타겟용 오브젝트 생성 ]
	ComponentEngine::GameObject* _cameraTargetObj = new ComponentEngine::GameObject("CameraTargetObj");
	// Player 오브젝트의 자식으로 오브젝트를 붙인다.
	ComponentEngine::Transform* _transform = _cameraTargetObj->GetComponent<ComponentEngine::Transform>();
	_playerObj->GetComponent<ComponentEngine::Transform>()->AttatchChild(_transform);
	// 플레이어의 위치보다 앞을 바라볼 수 있게 위치를 이동시킨다.
	_transform->Translate(_transform->m_Forward * 5);
	// Scene에 해당 오브젝트 등록
	_scene->AddGameObject(_cameraTargetObj);

	/// //////////////////////////////////////////////////////////////////////////

	/// [ Camera 오브젝트 생성 ]
	ComponentEngine::GameObject* _cameraObj = new ComponentEngine::GameObject("camera");
	// Camera 오브젝트 Transform 설정 (위치, 회전)
	_cameraObj->GetComponent<ComponentEngine::Transform>()->SetPosition(Vector3(0, 9, -9));
	_cameraObj->GetComponent<ComponentEngine::Transform>()->SetRotate(30, 0, 0);
	// Camera 컴포넌트 추가 및 설정
	ComponentEngine::CCamera* _camera = new ComponentEngine::CCamera();
	_camera->SetCameraMode(ComponentEngine::CCamera::eMode::FOLLOW);
	_cameraObj->AddComponent<ComponentEngine::CCamera*>(_camera);
	// Scene에 해당 오브젝트 등록
	_scene->AddGameObject(_cameraObj);
	// Scene의 메인 카메라로 등록
	_scene->SetMainCamera(_camera);

	///  //////////////////////////////////////////////////////////////////////////
	/// [ Scene 등록 ]
	ComponentEngine::SceneManager::Ins()->AddScene(_scene);
}
