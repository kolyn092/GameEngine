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
#include "Script.h"
#include <imgui.h>

ImguiManager* ImguiManager::m_pInstance = nullptr;

ImguiManager::ImguiManager()
	: m_NowScene(nullptr), m_SelectedObject(nullptr)
{

}

ImguiManager::~ImguiManager()
{
	if (m_NowScene)
	{
		delete m_NowScene;
		m_NowScene = nullptr;
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
	m_NowScene = new ComponentEngine::Scene();
	m_NowScene->SetInputMouseLock(false);

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
	m_NowScene->AddGameObject(_lightObj);

	/// [ Default Skybox Object Setting ]
	ComponentEngine::GameObject* _skyboxObj = new ComponentEngine::GameObject("skybox");
	// Skybox 컴포넌트 추가 및 리소스 파일 경로 설정
	ComponentEngine::Skybox* _skybox = new ComponentEngine::Skybox();
	_skybox->m_FilePath = "..\\Resource\\Skybox\\SkyBox3_1.dds";
	_skyboxObj->AddComponent<ComponentEngine::Skybox*>(_skybox);
	// Scene에 해당 오브젝트 등록
	m_NowScene->AddGameObject(_skyboxObj);

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
	m_NowScene->AddGameObject(_cameraObj);
	// Scene의 메인 카메라로 등록
	m_NowScene->SetMainCamera(_camera);

	/// [ Add Default Scene ]
	ComponentEngine::SceneManager::Ins()->AddScene(m_NowScene);
}

void ImguiManager::Update()
{
	/// //////////////////////////////////////////////////////////////////////////
	/// ImGUI Setting
	/// //////////////////////////////////////////////////////////////////////////

	ImGui::Begin("Hierarchy");

	// 우클릭 컨텍스트 메뉴 추가
	if (ImGui::IsWindowHovered() &&
		ImGui::IsMouseReleased(ImGuiMouseButton_Right) && !ImGui::IsAnyItemHovered())
	{
		ImGui::OpenPopup("HierarchyContext");
	}

	if (ImGui::BeginPopup("HierarchyContext"))
	{
		if (ImGui::MenuItem("Add GameObject"))
		{
			ComponentEngine::GameObject* _newObj = new ComponentEngine::GameObject();
			m_NowScene->AddGameObject(_newObj);
		}
		ImGui::EndPopup();
	}

	auto rootGameObjects = m_NowScene->GetRootObjects();
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

	// GameObject Delete 기능
	if (m_SelectedObject != nullptr && ImGui::IsKeyPressed(ImGuiKey::ImGuiKey_Delete))
	{
		m_NowScene->DeleteGameObject(m_SelectedObject);
		m_SelectedObject = nullptr;
	}
}

// Inspector 창
void ImguiManager::ShowObjectDetails()
{
	if (m_SelectedObject != nullptr)  // 오브젝트가 선택된 경우에만 창을 띄움
	{
		ImGui::Begin("Inspector", nullptr, ImGuiWindowFlags_AlwaysAutoResize);

		// 우클릭으로 전체 창에서 팝업 열기
		if (ImGui::BeginPopupContextWindow("InspectorContext", ImGuiPopupFlags_MouseButtonRight))
		{
			if (ImGui::MenuItem("Delete"))
			{
				// 컴포넌트 index 지정해서 해당 index 컴포넌트를 삭제하도록 할까 싶긴한데 고민중
			}
			ImGui::EndPopup();
		}

		bool gameObjectActive = m_SelectedObject->isActive();
		ImGui::Checkbox("##objectActive", &gameObjectActive);
		m_SelectedObject->SetActive(gameObjectActive);
		ImGui::SameLine();

		std::string strName = m_SelectedObject->GetName();
		std::vector<char> nameBuffer(128);
		std::copy(strName.begin(), strName.end(), nameBuffer.begin());
		nameBuffer[strName.size()] = '\0';

		if (ImGui::InputText("##name", nameBuffer.data(), nameBuffer.size()))
		{
			//m_SelectedObject->SetName(std::string(nameBuffer.data()));
			std::string newName = nameBuffer.data();
			if (!newName.empty() && newName != m_SelectedObject->GetName())
			{
				m_SelectedObject->SetName(newName);
			}
		}

		ImGui::Separator(); // 구분선 추가

		auto& compList = m_SelectedObject->GetAllComponents();
		for (int i = 0; i < m_SelectedObject->GetComponentSize(); i++)
		{
			// TODO : 같은 이름의 컴포넌트가 여러개가 있을 경우
			if (compList[i]->GetComponentName() == "Transform")
				Update_Transform_Component();
			else if (compList[i]->GetComponentName() == "AudioListener")
				Update_AudioListener_Component();
			else if (compList[i]->GetComponentName() == "AudioSource")
				Update_AudioSource_Component();
			else if (compList[i]->GetComponentName() == "Camera")
				Update_Camera_Component();
			else if (compList[i]->GetComponentName() == "Light")
				Update_Light_Component();
			else if (compList[i]->GetComponentName() == "MeshRenderer")
				Update_MeshRenderer_Component();
			else if (compList[i]->GetComponentName() == "_BoxCollider")
				Update_BoxCollider_Component();
			else if (compList[i]->GetComponentName() == "CapsuleCollider")
				Update_CapsuleCollider_Component();
			else if (compList[i]->GetComponentName() == "SphereCollider")
				Update_SphereCollider_Component();
			else if (compList[i]->GetComponentName() == "Rigidbody")
				Update_Rigidbody_Component();
		}

		if (ImGui::Button("Add Component"))
		{
			ImGui::OpenPopup("AddComponentPopup");
		}

		if (ImGui::BeginPopup("AddComponentPopup"))
		{
			if (ImGui::MenuItem("AudioListener"))
			{
				ComponentEngine::CEAudioListener* _comp = new ComponentEngine::CEAudioListener();
				m_SelectedObject->AddComponent<ComponentEngine::CEAudioListener*>(_comp);
			}
			if (ImGui::MenuItem("AudioSource"))
			{
				ComponentEngine::AudioSource* _comp = new ComponentEngine::AudioSource();
				m_SelectedObject->AddComponent<ComponentEngine::AudioSource*>(_comp);
			}
			if (ImGui::MenuItem("Camera"))
			{
				ComponentEngine::CECamera* _comp = new ComponentEngine::CECamera();
				m_SelectedObject->AddComponent<ComponentEngine::CECamera*>(_comp);
			}
			if (ImGui::MenuItem("Light"))
			{
				ComponentEngine::CELight* _comp = new ComponentEngine::CELight();
				m_SelectedObject->AddComponent<ComponentEngine::CELight*>(_comp);
			}
			if (ImGui::MenuItem("BoxCollider"))
			{
				ComponentEngine::BoxCollider* _comp = new ComponentEngine::BoxCollider();
				m_SelectedObject->AddComponent<ComponentEngine::BoxCollider*>(_comp);
			}
			if (ImGui::MenuItem("CapsuleCollider"))
			{
				ComponentEngine::CapsuleCollider* _comp = new ComponentEngine::CapsuleCollider();
				m_SelectedObject->AddComponent<ComponentEngine::CapsuleCollider*>(_comp);
			}
			if (ImGui::MenuItem("SphereCollider"))
			{
				ComponentEngine::SphereCollider* _comp = new ComponentEngine::SphereCollider();
				m_SelectedObject->AddComponent<ComponentEngine::SphereCollider*>(_comp);
			}
			if (ImGui::MenuItem("Rigidbody"))
			{
				ComponentEngine::Rigidbody* _comp = new ComponentEngine::Rigidbody();
				m_SelectedObject->AddComponent<ComponentEngine::Rigidbody*>(_comp);
			}
			if (ImGui::MenuItem("Script"))
			{
				ComponentEngine::Script* _comp = new ComponentEngine::Script();
				m_SelectedObject->AddComponent<ComponentEngine::Script*>(_comp);
			}
			ImGui::EndPopup();
		}

		ImGui::End();
	}
}

void ImguiManager::Update_Transform_Component()
{
	if (ImGui::CollapsingHeader("Transform"))
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
}

void ImguiManager::Update_AudioListener_Component()
{
	auto audioListener = m_SelectedObject->GetComponent<ComponentEngine::CEAudioListener>();
	bool componentActive = audioListener->isActive();
	ImGui::Checkbox("##componentActive", &componentActive);
	audioListener->SetActive(componentActive);
	ImGui::SameLine();
	ImGui::CollapsingHeader("AudioListener");
	ImGui::Separator();
}

void ImguiManager::Update_AudioSource_Component()
{
	auto audioSource = m_SelectedObject->GetComponent<ComponentEngine::AudioSource>();
	bool componentActive = audioSource->isActive();
	ImGui::Checkbox("##componentActive", &componentActive);
	audioSource->SetActive(componentActive);
	ImGui::SameLine();

	if(ImGui::CollapsingHeader("AudioSource"))
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
	ImGui::Separator();
}

void ImguiManager::Update_Camera_Component()
{
	auto camera = m_SelectedObject->GetComponent<ComponentEngine::CECamera>();
	bool componentActive = camera->isActive();
	ImGui::Checkbox("##componentActive", &componentActive);
	camera->SetActive(componentActive);
	ImGui::SameLine();

	if (ImGui::CollapsingHeader("Camera"))
	{
		int viewmode = static_cast<int>(camera->m_ViewMode);
		ImGui::Text("Background Color");
		ImGui::ColorEdit4("##BackgroundColor", (float*)&(camera->m_BackGroundColor));
		ImGui::Text("Projection");
		ImGui::Combo("##Projection", &viewmode, "Perspective\0Orthographic\0");
		camera->ConvertViewMode(viewmode);

		if (viewmode == 0)
		{
			ImGui::Text("FOV");
			ImGui::SliderFloat("##FOV", &camera->m_FOV, 1.0f, 179.0f);
		}

		float viewport[4] = { 0, 0, camera->m_ViewPortWidth, camera->m_ViewPortHeight };
		ImGui::Text("Viewport Rect");
		ImGui::InputFloat4("##ViewportRect", (float*)&viewport);
		ImGui::Text("Near");
		ImGui::SliderFloat("##Near", &camera->m_Near, 0.01f, camera->m_Far - 0.1f);
		ImGui::Text("Far");
		ImGui::SliderFloat("##Far", &camera->m_Far, camera->m_Near + 0.1f, 10000.0f);
	}
	ImGui::Separator();
}

void ImguiManager::Update_Light_Component()
{
	auto light = m_SelectedObject->GetComponent<ComponentEngine::CELight>();
	bool componentActive = light->isActive();
	ImGui::Checkbox("##componentActive", &componentActive);
	light->SetActive(componentActive);
	ImGui::SameLine();

	if (ImGui::CollapsingHeader("Light"))
	{
		int lightType = static_cast<int>(light->m_LightType);
		ImGui::Text("Light Type");
		ImGui::Combo("##LightType", &lightType, "Spot\0Directional\0Point\0");
		light->ConvertLightType(lightType);
		ImGui::Text("Range");
		ImGui::InputFloat("##Range", (float*)&(light->m_Range));
		ImGui::Text("Color");
		ImGui::ColorEdit4("##Color", (float*)&(light->m_Color));
		ImGui::Text("Intensity");
		ImGui::InputFloat("##Intensity", (float*)&(light->m_Intensity));
		ImGui::Text("IndirectMultiplier");
		ImGui::InputFloat("##IndirectMultiplier", (float*)&(light->m_IndirectMultiplier));
		int shadowtype = static_cast<int>(light->m_ShadowType);
		ImGui::Text("Shadow Type");
		ImGui::Combo("##ShadowType", &shadowtype, "Soft\0Hard\0");
	}
	ImGui::Separator();
}

void ImguiManager::Update_MeshRenderer_Component()
{
	if(ImGui::CollapsingHeader("MeshRenderer"))
	{
		auto meshComponent = m_SelectedObject->m_MeshRenderer;
		ImGui::Text("Single Path : ", meshComponent->m_ModelPath);
		if (meshComponent->m_ModelPathList.size() > 0);
		for (int i = 0; i < meshComponent->m_ModelPathList.size(); i++)
		{
			ImGui::Text("Multi Path : ", meshComponent->m_ModelPathList[i]);
		}
	}
	ImGui::Separator();
}

void ImguiManager::Update_BoxCollider_Component()
{
	auto collider = m_SelectedObject->GetComponent<ComponentEngine::BoxCollider>();
	bool componentActive = collider->isActive();
	ImGui::Checkbox("##componentActive", &componentActive);
	collider->SetActive(componentActive);
	ImGui::SameLine();

	if(ImGui::CollapsingHeader("BoxCollider"))
	{
		bool isTrigger = collider->IsTrigger();
		ImGui::Text("IsTrigger ");
		ImGui::SameLine();
		ImGui::Checkbox("##IsTrigger", &isTrigger);
		collider->SetTrigger(isTrigger);

		ImGui::Text("Center ");
		ImGui::SameLine();
		ImGui::InputFloat3("##Center", (float*)&(collider->m_Center));

		ImGui::Text("Size ");
		ImGui::SameLine();
		ImGui::InputFloat3("##Size", (float*)&(collider->m_Size));
	}
	ImGui::Separator();
}

void ImguiManager::Update_CapsuleCollider_Component()
{
	auto collider = m_SelectedObject->GetComponent<ComponentEngine::CapsuleCollider>();
	bool componentActive = collider->isActive();
	ImGui::Checkbox("##componentActive", &componentActive);
	collider->SetActive(componentActive);
	ImGui::SameLine();

	if(ImGui::CollapsingHeader("CapsuleCollider"))
	{
		bool isTrigger = collider->IsTrigger();
		ImGui::Text("IsTrigger ");
		ImGui::SameLine();
		ImGui::Checkbox("##IsTrigger", &isTrigger);
		collider->SetTrigger(isTrigger);

		ImGui::Text("Center ");
		ImGui::SameLine();
		ImGui::InputFloat3("##Center", (float*)&(collider->m_Center));

		ImGui::Text("Radius ");
		ImGui::SameLine();
		ImGui::InputFloat("##Radius", (float*)&(collider->m_Radius));

		ImGui::Text("Height ");
		ImGui::SameLine();
		ImGui::InputFloat("##Height", (float*)&(collider->m_HalfHeight));

		int direction = static_cast<int>(collider->m_Direction);
		ImGui::Text("Direction");
		ImGui::Combo("##Direction", &direction, "XAxis\0YAxis\0ZAxis\0");
		collider->m_Direction = static_cast<ComponentEngine::CapsuleCollider::eCapsuleDirection>(direction);
	}
	ImGui::Separator();
}

void ImguiManager::Update_SphereCollider_Component()
{
	auto collider = m_SelectedObject->GetComponent<ComponentEngine::SphereCollider>();
	bool componentActive = collider->isActive();
	ImGui::Checkbox("##componentActive", &componentActive);
	collider->SetActive(componentActive);
	ImGui::SameLine();

	if(ImGui::CollapsingHeader("SphereCollider"))
	{
		bool isTrigger = collider->IsTrigger();
		ImGui::Text("IsTrigger ");
		ImGui::SameLine();
		ImGui::Checkbox("##IsTrigger", &isTrigger);
		collider->SetTrigger(isTrigger);

		ImGui::Text("Center ");
		ImGui::SameLine();
		ImGui::InputFloat3("##Center", (float*)&(collider->m_Center));

		ImGui::Text("Radius ");
		ImGui::SameLine();
		ImGui::InputFloat("##Radius", (float*)&(collider->m_Radius));
	}
	ImGui::Separator();
}

void ImguiManager::Update_Rigidbody_Component()
{
	auto rigidbody = m_SelectedObject->GetComponent<ComponentEngine::Rigidbody>();
	bool componentActive = rigidbody->isActive();
	ImGui::Checkbox("##componentActive", &componentActive);
	rigidbody->SetActive(componentActive);
	ImGui::SameLine();

	if(ImGui::CollapsingHeader("Rigidbody"))
	{
		ImGui::Text("Mass ");
		ImGui::SameLine();
		ImGui::InputFloat("##Mass", (float*)&(rigidbody->m_Mass));

		ImGui::Text("Drag ");
		ImGui::SameLine();
		ImGui::InputFloat("##Drag", (float*)&(rigidbody->m_Drag));

		ImGui::Text("Angular Drag ");
		ImGui::SameLine();
		ImGui::InputFloat("##AngularDrag", (float*)&(rigidbody->m_AngularDrag));

		ImGui::Text("UseGravity ");
		ImGui::SameLine();
		ImGui::Checkbox("##IsTrigger", &(rigidbody->m_UseGravity));

		int interpolate = static_cast<int>(rigidbody->m_Interpolate);
		ImGui::Text("Interpolate");
		ImGui::Combo("##Interpolate", &interpolate, "Interpolate\0Extrapolate\0");
		rigidbody->m_Interpolate = static_cast<ComponentEngine::Rigidbody::eInterpolate>(interpolate);

		int collisiondetection = static_cast<int>(rigidbody->m_CollisionDetection);
		ImGui::Text("Collision Detection");
		ImGui::Combo("##CollisionDetection", &interpolate, "Discrete\0Continuous\0Continuous_Dynamic\0Continuous_Speculative\0");
		rigidbody->m_CollisionDetection = static_cast<ComponentEngine::Rigidbody::eCollisionDetection>(collisiondetection);

		if (ImGui::CollapsingHeader("constraints"))
		{
			ImGui::Text("Freeze Position");
			ImGui::SameLine();
			ImGui::Checkbox("##FreezePosX", &(rigidbody->m_FreezePosition.x));
			ImGui::SameLine();
			ImGui::Checkbox("##FreezePosY", &(rigidbody->m_FreezePosition.y));
			ImGui::SameLine();
			ImGui::Checkbox("##FreezePosZ", &(rigidbody->m_FreezePosition.z));

			ImGui::Text("Freeze Rotation");
			ImGui::SameLine();
			ImGui::Checkbox("##FreezeRotX", &(rigidbody->m_FreezeRotation.x));
			ImGui::SameLine();
			ImGui::Checkbox("##FreezeRotY", &(rigidbody->m_FreezeRotation.y));
			ImGui::SameLine();
			ImGui::Checkbox("##FreezeRotZ", &(rigidbody->m_FreezeRotation.z));
		}
	}
	ImGui::Separator();
}
