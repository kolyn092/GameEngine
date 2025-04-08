#include "ImguiManager.h"
#include "Scene.h"
#include "SceneManager.h"
#include "CEAudioListener.h"
#include "AudioSource.h"
#include "CCamera.h"
#include "CLight.h"
#include "MeshRenderer.h"
#include "BoxCollider.h"
#include "CapsuleCollider.h"
#include "SphereCollider.h"
#include "Rigidbody.h"
#include "Transform.h"
#include "CSkybox.h"
#include "CTerrain.h"
#include "CFont.h"
#include <imgui.h>

ImguiManager* ImguiManager::m_pInstance = nullptr;

ImguiManager::ImguiManager()
	: m_DefaultScene(nullptr), m_SelectedObject(nullptr)
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
	ComponentEngine::CELight* _light = new ComponentEngine::CELight();
	_light->SetColor(Color(1, 1, 1, 1));
	_light->SetFogActive(false);
	_lightObj->AddComponent<ComponentEngine::CELight*>(_light);
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
	ComponentEngine::CECamera* _camera = new ComponentEngine::CECamera();
	_camera->SetCameraMode(ComponentEngine::CECamera::eMode::FOLLOW);
	_cameraObj->AddComponent<ComponentEngine::CECamera*>(_camera);
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
	/// 
	ImGui::Begin("Hierarchy");
	auto rootGameObjects = m_DefaultScene->GetRootObjects();
	for (auto obj : rootGameObjects)
	{
		ShowGameObjectHierarchy(obj);
	}
	ImGui::End();
	ShowObjectDetails();
}

void ImguiManager::Finalize()
{
	if (m_pInstance)
	{
		delete m_pInstance;
		m_pInstance = nullptr;
	}
}

void ImguiManager::ShowGameObjectHierarchy(ComponentEngine::GameObject* obj)
{
	// 트리 노드 생성
	bool nodeOpen = ImGui::TreeNodeEx(obj->GetName().c_str(),
		(m_SelectedObject == obj) ? ImGuiTreeNodeFlags_Selected : 0);

	// 선택 상태 업데이트
	if (ImGui::IsItemClicked())
	{
		m_SelectedObject = obj;
	}

	// 우클릭 컨텍스트 메뉴 추가
	if (ImGui::BeginPopupContextItem())
	{
		if (ImGui::MenuItem("Rename"))
		{
			printf("Rename %s\n", obj->GetName().c_str());
		}

		if (ImGui::MenuItem("Delete"))
		{
			printf("Delete %s\n", obj->GetName().c_str());
		}

		ImGui::EndPopup();
	}

	// 드래그 드롭 추가
	if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_SourceAllowNullID))
	{
		ImGui::SetDragDropPayload("DND_NODE", obj->GetName().c_str(), obj->GetName().size() + 1);
		ImGui::Text("Dragging: %s", obj->GetName().c_str());
		ImGui::EndDragDropSource();
	}

	if (ImGui::BeginDragDropTarget())
	{
		if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("DND_NODE"))
		{
			const char* droppedName = (const char*)payload->Data;
			printf("Dropped %s onto %s\n", droppedName, obj->GetName().c_str());
		}
		ImGui::EndDragDropTarget();
	}

	// 자식 노드 표시
	if (nodeOpen)
	{
		auto trans = obj->GetComponent<ComponentEngine::Transform>();
		for (int i = 0; i < trans->GetChildCount(); i++)
		{
			auto childObject = trans->GetChild(i)->m_GameObject;
			ShowGameObjectHierarchy(childObject);
		}
		ImGui::TreePop();
	}
}

void ImguiManager::ShowObjectDetails()
{
	if (m_SelectedObject != nullptr)  // 오브젝트가 선택된 경우에만 창을 띄움
	{
		ImGui::Begin("Inspector", nullptr, ImGuiWindowFlags_AlwaysAutoResize);

		bool gameObjectActive = m_SelectedObject->isActive();
		ImGui::Checkbox("##objectActive", &gameObjectActive);
		m_SelectedObject->SetActive(gameObjectActive);
		ImGui::SameLine();

		std::string strName = m_SelectedObject->GetName();
		std::vector<char> nameBuffer(strName.begin(), strName.end());
		nameBuffer.push_back('\0');

		if (ImGui::InputText("##name", nameBuffer.data(), nameBuffer.size()))
		{
			m_SelectedObject->SetName(std::string(nameBuffer.data()));
		}

		ImGui::Separator(); // 구분선 추가

		ImGui::CollapsingHeader("Transform");
		{
			auto trans = m_SelectedObject->m_Transform;
			float pos3f[3] = { trans->GetPosition().x, trans->GetPosition().y, trans->GetPosition().z };
			float rot3f[3] = { trans->GetRotation().x, trans->GetRotation().y, trans->GetRotation().z };
			float scl3f[3] = { trans->GetScale().x, trans->GetScale().y, trans->GetScale().z };
			if (trans->GetParent() == nullptr)
			{
				ImGui::Text("Position");
				ImGui::DragFloat3("##position", pos3f, 0.1f);

				ImGui::Text("Rotation");
				ImGui::DragFloat3("##rotation", rot3f, 0.1f);

				ImGui::Text("Scale");
				ImGui::DragFloat3("##scale", scl3f, 0.1f, 0.01f, 1000.0f);

				trans->SetPosition(DirectX::SimpleMath::Vector3(pos3f[0], pos3f[1], pos3f[2]));
				trans->SetRotate(DirectX::SimpleMath::Vector3(rot3f[0], rot3f[1], rot3f[2]));
				trans->SetScale(DirectX::SimpleMath::Vector3(scl3f[0], scl3f[1], scl3f[2]));
			}
		}

		ImGui::Separator(); // 구분선 추가

		// TODO : 같은 이름의 컴포넌트가 여러개가 있을 경우
		if (m_SelectedObject->HasComponent<ComponentEngine::CEAudioListener>())
			Update_AudioListener_Component();
		else if (m_SelectedObject->HasComponent<ComponentEngine::AudioSource>())
			Update_AudioSource_Component();
		else if (m_SelectedObject->HasComponent<ComponentEngine::CECamera>())
			Update_Camera_Component();
		else if (m_SelectedObject->HasComponent<ComponentEngine::CELight>())
			Update_Light_Component();
		else if (m_SelectedObject->HasComponent<ComponentEngine::MeshRenderer>())
			Update_MeshRenderer_Component();
		else if (m_SelectedObject->HasComponent<ComponentEngine::BoxCollider>())
			Update_BoxCollider_Component();
		else if (m_SelectedObject->HasComponent<ComponentEngine::CapsuleCollider>())
			Update_CapsuleCollider_Component();
		else if (m_SelectedObject->HasComponent<ComponentEngine::SphereCollider>())
			Update_SphereCollider_Component();
		else if (m_SelectedObject->HasComponent<ComponentEngine::Rigidbody>())
			Update_Rigidbody_Component();

		if (ImGui::Button("Close"))  // 닫기 버튼 추가
		{
			m_SelectedObject = nullptr;  // 창 닫기
		}

		ImGui::End();
	}
}

void ImguiManager::Update_AudioListener_Component()
{
	auto audioListener = m_SelectedObject->GetComponent<ComponentEngine::CEAudioListener>();
	bool componentActive = audioListener->isActive();
	ImGui::Checkbox("##componentActive", &componentActive);
	audioListener->SetActive(componentActive);
	ImGui::SameLine();

	ImGui::CollapsingHeader("AudioListener");
	{

	}
	ImGui::Separator(); // 구분선 추가
}

void ImguiManager::Update_AudioSource_Component()
{
	auto audioSource = m_SelectedObject->GetComponent<ComponentEngine::AudioSource>();
	bool componentActive = audioSource->isActive();
	ImGui::Checkbox("##componentActive", &componentActive);
	audioSource->SetActive(componentActive);
	ImGui::SameLine();

	ImGui::CollapsingHeader("AudioSource");
	{
		auto audioClip = audioSource->GetAudioClip();
		auto audioClipPath = audioClip->GetSoundPath();
		int len = WideCharToMultiByte(CP_UTF8, 0, audioClipPath, -1, NULL, 0, NULL, NULL);
		char* charStr = new char[len];
		WideCharToMultiByte(CP_UTF8, 0, audioClipPath, -1, charStr, len, NULL, NULL);
		ImGui::Text("Now Path ");
		ImGui::SameLine();
		ImGui::Text(charStr);

		bool muteActive = audioSource->IsMute();
		ImGui::Text("Mute ");
		ImGui::SameLine();
		ImGui::Checkbox("##muteActive", &muteActive);
		audioSource->SetMute(muteActive);

		bool playonawakeActive = audioSource->m_isPlayOnAwake;
		ImGui::Text("Play On Awake ");
		ImGui::SameLine();
		ImGui::Checkbox("##playonawakeActive", &playonawakeActive);

		bool loopActive = audioSource->m_isLoop;
		ImGui::Text("Loop ");
		ImGui::SameLine();
		ImGui::Checkbox("##loopActive", &loopActive);

		float volume = audioSource->GetVolume();
		ImGui::Text("Volume ");
		ImGui::SameLine();
		ImGui::SliderFloat("##volume", &volume, 0.0f, 1.0f);
		audioSource->SetVolume(volume);
	}
	ImGui::Separator(); // 구분선 추가
}

void ImguiManager::Update_Camera_Component()
{
	auto camera = m_SelectedObject->GetComponent<ComponentEngine::CECamera>();
	if (ImGui::CollapsingHeader("Camera"))
	{
		// background color 
		// projection view mode combobox
		// fov SliderFloat
		// viewport rect textbox 4
		// clipping planes near textbox / far textbox
		// etc
		ImGui::Text("Background Color");
		ImGui::ColorEdit4("##BackgroundColor", (float*)&(camera->m_BackGroundColor));

	}
	ImGui::Separator(); // 구분선 추가
}

void ImguiManager::Update_Light_Component()
{
	ImGui::CollapsingHeader("Light");
	{
		// light type combobox
		// range textbox
		// color 
		// intensity
		// IndirectMultiplier
		// shadowtype combobox
	}
	ImGui::Separator(); // 구분선 추가
}

void ImguiManager::Update_MeshRenderer_Component()
{
	ImGui::CollapsingHeader("MeshRenderer");
	{
		auto meshComponent = m_SelectedObject->m_MeshRenderer;
		ImGui::Text("Single Path : ", meshComponent->m_ModelPath);
		if (meshComponent->m_ModelPathList.size() > 0);
		for (int i = 0; i < meshComponent->m_ModelPathList.size(); i++)
		{
			ImGui::Text("Multi Path : ", meshComponent->m_ModelPathList[i]);
		}
	}
	ImGui::Separator(); // 구분선 추가
}

void ImguiManager::Update_BoxCollider_Component()
{
	ImGui::CollapsingHeader("BoxCollider");
	{
		// istrigger checkbox
		// center textbox 3
		// size textbox 3
	}
	ImGui::Separator(); // 구분선 추가
}

void ImguiManager::Update_CapsuleCollider_Component()
{
	ImGui::CollapsingHeader("CapsuleCollider");
	{
		// istrigger checkbox
		// center textbox 3
		// radius textbox
		// height textbox
		// direction combobox
	}
	ImGui::Separator(); // 구분선 추가
}

void ImguiManager::Update_SphereCollider_Component()
{
	ImGui::CollapsingHeader("SphereCollider");
	{
		// istrigger checkbox
		// center textbox 3
		// radius textbox
	}
	ImGui::Separator(); // 구분선 추가
}

void ImguiManager::Update_Rigidbody_Component()
{
	ImGui::CollapsingHeader("Rigidbody");
	{
		// mass
		// drag
		// angular drag
		// usegravity
		// iskinematic
		// interpolate
		// collisiondetection
		// constraints (freeze pos, freeze rot)
	}
	ImGui::Separator(); // 구분선 추가
}
