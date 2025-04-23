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
				Update_Transform_Component(i);
			else if (compList[i]->GetComponentName() == "AudioListener")
				Update_AudioListener_Component(i);
			else if (compList[i]->GetComponentName() == "AudioSource")
				Update_AudioSource_Component(i);
			else if (compList[i]->GetComponentName() == "Camera")
				Update_Camera_Component(i);
			else if (compList[i]->GetComponentName() == "Light")
				Update_Light_Component(i);
			else if (compList[i]->GetComponentName() == "MeshRenderer")
				Update_MeshRenderer_Component(i);
			else if (compList[i]->GetComponentName() == "BoxCollider")
				Update_BoxCollider_Component(i);
			else if (compList[i]->GetComponentName() == "CapsuleCollider")
				Update_CapsuleCollider_Component(i);
			else if (compList[i]->GetComponentName() == "SphereCollider")
				Update_SphereCollider_Component(i);
			else if (compList[i]->GetComponentName() == "Rigidbody")
				Update_Rigidbody_Component(i);
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

void ImguiManager::Update_Transform_Component(int index)
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

void ImguiManager::Update_AudioListener_Component(int index)
{
	auto audioListener = m_SelectedObject->GetComponent<ComponentEngine::CEAudioListener>();
	bool componentActive = audioListener->isActive();
	std::string label = "##componentActive" + std::to_string(index);
	ImGui::Checkbox(label.c_str(), & componentActive);
	audioListener->SetActive(componentActive);
	ImGui::SameLine();
	ImGui::CollapsingHeader("AudioListener");
	ImGui::Separator();
}

void ImguiManager::Update_AudioSource_Component(int index)
{
	auto audioSource = m_SelectedObject->GetComponent<ComponentEngine::AudioSource>();
	bool componentActive = audioSource->isActive();
	std::string label = "##componentActive" + std::to_string(index);
	ImGui::Checkbox(label.c_str(), &componentActive);
	audioSource->SetActive(componentActive);
	ImGui::SameLine();

	if(ImGui::CollapsingHeader("AudioSource"))
	{
		std::string componentIndex = std::to_string(audioSource->GetTypeID());
		std::string label;

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
		label = "##muteActive" + componentIndex + "_" + std::to_string(audioSource->m_TypeLocalIndex);
		ImGui::Checkbox(label.c_str(), &muteActive);
		audioSource->SetMute(muteActive);

		bool playonawakeActive = audioSource->m_isPlayOnAwake;
		ImGui::Text("Play On Awake ");
		ImGui::SameLine();
		label = "##playonawakeActive" + componentIndex + "_" + std::to_string(audioSource->m_TypeLocalIndex);
		ImGui::Checkbox(label.c_str(), &playonawakeActive);

		bool loopActive = audioSource->m_isLoop;
		ImGui::Text("Loop ");
		ImGui::SameLine();
		label = "##loopActive" + componentIndex + "_" + std::to_string(audioSource->m_TypeLocalIndex);
		ImGui::Checkbox(label.c_str(), &loopActive);

		float volume = audioSource->GetVolume();
		ImGui::Text("Volume ");
		ImGui::SameLine();
		label = "##volume" + componentIndex + "_" + std::to_string(audioSource->m_TypeLocalIndex);
		ImGui::SliderFloat(label.c_str(), &volume, 0.0f, 1.0f);
		audioSource->SetVolume(volume);
	}
	ImGui::Separator();
}

void ImguiManager::Update_Camera_Component(int index)
{
	auto camera = m_SelectedObject->GetComponent<ComponentEngine::CECamera>();
	bool componentActive = camera->isActive();
	std::string label = "##componentActive" + std::to_string(index);
	ImGui::Checkbox(label.c_str(), &componentActive);
	camera->SetActive(componentActive);
	ImGui::SameLine();

	if (ImGui::CollapsingHeader("Camera"))
	{
		std::string componentIndex = std::to_string(camera->GetTypeID());
		std::string label;

		int viewmode = static_cast<int>(camera->m_ViewMode);
		ImGui::Text("Background Color");
		label = "##BackgroundColor" + componentIndex + "_" + std::to_string(camera->m_TypeLocalIndex);
		ImGui::ColorEdit4(label.c_str(), (float*)&(camera->m_BackGroundColor));

		ImGui::Text("Projection");
		label = "##Projection" + componentIndex + "_" + std::to_string(camera->m_TypeLocalIndex);
		ImGui::Combo(label.c_str(), &viewmode, "Perspective\0Orthographic\0");
		camera->ConvertViewMode(viewmode);

		if (viewmode == 0)
		{
			ImGui::Text("FOV");
			ImGui::SliderFloat("##FOV", &camera->m_FOV, 1.0f, 179.0f);
		}

		float viewport[4] = { 0, 0, camera->m_ViewPortWidth, camera->m_ViewPortHeight };
		ImGui::Text("Viewport Rect");
		label = "##ViewportRect" + componentIndex + "_" + std::to_string(camera->m_TypeLocalIndex);
		ImGui::InputFloat4(label.c_str(), (float*)&viewport);

		ImGui::Text("Near");
		label = "##Near" + componentIndex + "_" + std::to_string(camera->m_TypeLocalIndex);
		ImGui::SliderFloat(label.c_str(), &camera->m_Near, 0.01f, camera->m_Far - 0.1f);

		ImGui::Text("Far");
		label = "##Far" + componentIndex + "_" + std::to_string(camera->m_TypeLocalIndex);
		ImGui::SliderFloat(label.c_str(), &camera->m_Far, camera->m_Near + 0.1f, 10000.0f);
	}
	ImGui::Separator();
}

void ImguiManager::Update_Light_Component(int index)
{
	auto light = m_SelectedObject->GetComponent<ComponentEngine::CELight>();
	bool componentActive = light->isActive();
	std::string label = "##componentActive" + std::to_string(index);
	ImGui::Checkbox(label.c_str(), &componentActive);
	light->SetActive(componentActive);
	ImGui::SameLine();

	if (ImGui::CollapsingHeader("Light"))
	{
		std::string componentIndex = std::to_string(light->GetTypeID());
		std::string label;

		int lightType = static_cast<int>(light->m_LightType);
		ImGui::Text("Light Type");
		label = "##LightType" + componentIndex + "_" + std::to_string(light->m_TypeLocalIndex);
		ImGui::Combo(label.c_str(), &lightType, "Spot\0Directional\0Point\0");
		light->ConvertLightType(lightType);

		ImGui::Text("Range");
		label = "##Range" + componentIndex + "_" + std::to_string(light->m_TypeLocalIndex);
		ImGui::InputFloat(label.c_str(), (float*)&(light->m_Range));

		ImGui::Text("Color");
		label = "##Color" + componentIndex + "_" + std::to_string(light->m_TypeLocalIndex);
		ImGui::ColorEdit4(label.c_str(), (float*)&(light->m_Color));

		ImGui::Text("Intensity");
		label = "##Intensity" + componentIndex + "_" + std::to_string(light->m_TypeLocalIndex);
		ImGui::InputFloat(label.c_str(), (float*)&(light->m_Intensity));

		ImGui::Text("IndirectMultiplier");
		label = "##IndirectMultiplier" + componentIndex + "_" + std::to_string(light->m_TypeLocalIndex);
		ImGui::InputFloat(label.c_str(), (float*)&(light->m_IndirectMultiplier));

		int shadowtype = static_cast<int>(light->m_ShadowType);
		ImGui::Text("Shadow Type");
		label = "##ShadowType" + componentIndex + "_" + std::to_string(light->m_TypeLocalIndex);
		ImGui::Combo(label.c_str(), &shadowtype, "Soft\0Hard\0");
	}
	ImGui::Separator();
}

void ImguiManager::Update_MeshRenderer_Component(int index)
{
	auto meshComponent = m_SelectedObject->m_MeshRenderer;
	bool componentActive = meshComponent->isActive();
	std::string label = "##componentActive" + std::to_string(index);
	ImGui::Checkbox(label.c_str(), &componentActive);
	meshComponent->SetActive(componentActive);
	ImGui::SameLine();

	if(ImGui::CollapsingHeader("MeshRenderer"))
	{
		ImGui::Text("Single Path : ", meshComponent->m_ModelPath);
		for (int i = 0; i < meshComponent->m_ModelPathList.size(); i++)
		{
			ImGui::Text("Multi Path : ", meshComponent->m_ModelPathList[i]);
		}
	}
	ImGui::Separator();
}

void ImguiManager::Update_BoxCollider_Component(int index)
{
	auto collider = m_SelectedObject->GetComponent<ComponentEngine::BoxCollider>();
	bool componentActive = collider->isActive();
	std::string label = "##componentActive" + std::to_string(index);
	ImGui::Checkbox(label.c_str(), &componentActive);
	collider->SetActive(componentActive);
	ImGui::SameLine();

	if(ImGui::CollapsingHeader("BoxCollider"))
	{
		std::string componentIndex = std::to_string(collider->GetTypeID());
		std::string label;

		bool isTrigger = collider->IsTrigger();
		ImGui::Text("IsTrigger ");
		ImGui::SameLine();
		label = "##IsTrigger" + componentIndex + "_" + std::to_string(collider->m_TypeLocalIndex);
		ImGui::Checkbox(label.c_str(), &isTrigger);
		collider->SetTrigger(isTrigger);

		ImGui::Text("Center ");
		ImGui::SameLine();
		label = "##Center" + componentIndex + "_" + std::to_string(collider->m_TypeLocalIndex);
		ImGui::InputFloat3(label.c_str(), (float*)&(collider->m_Center));

		ImGui::Text("Size ");
		ImGui::SameLine();
		label = "##Size" + componentIndex + "_" + std::to_string(collider->m_TypeLocalIndex);
		ImGui::InputFloat3(label.c_str(), (float*)&(collider->m_Size));
	}
	ImGui::Separator();
}

void ImguiManager::Update_CapsuleCollider_Component(int index)
{
	auto collider = m_SelectedObject->GetComponent<ComponentEngine::CapsuleCollider>();
	bool componentActive = collider->isActive();
	std::string label = "##componentActive" + std::to_string(index);
	ImGui::Checkbox(label.c_str(), &componentActive);
	collider->SetActive(componentActive);
	ImGui::SameLine();

	if(ImGui::CollapsingHeader("CapsuleCollider"))
	{
		std::string componentIndex = std::to_string(collider->GetTypeID());
		std::string label;

		bool isTrigger = collider->IsTrigger();
		ImGui::Text("IsTrigger ");
		ImGui::SameLine();
		label = "##IsTrigger" + componentIndex + "_" + std::to_string(collider->m_TypeLocalIndex);
		ImGui::Checkbox(label.c_str(), &isTrigger);
		collider->SetTrigger(isTrigger);

		ImGui::Text("Center ");
		ImGui::SameLine();
		label = "##Center" + componentIndex + "_" + std::to_string(collider->m_TypeLocalIndex);
		ImGui::InputFloat3(label.c_str(), (float*)&(collider->m_Center));

		ImGui::Text("Radius ");
		ImGui::SameLine();
		label = "##Radius" + componentIndex + "_" + std::to_string(collider->m_TypeLocalIndex);
		ImGui::InputFloat(label.c_str(), (float*)&(collider->m_Radius));

		ImGui::Text("Height ");
		ImGui::SameLine();
		label = "##Height" + componentIndex + "_" + std::to_string(collider->m_TypeLocalIndex);
		ImGui::InputFloat(label.c_str(), (float*)&(collider->m_HalfHeight));

		int direction = static_cast<int>(collider->m_Direction);
		ImGui::Text("Direction");
		label = "##Direction" + componentIndex + "_" + std::to_string(collider->m_TypeLocalIndex);
		ImGui::Combo(label.c_str(), &direction, "XAxis\0YAxis\0ZAxis\0");
		collider->m_Direction = static_cast<ComponentEngine::CapsuleCollider::eCapsuleDirection>(direction);
	}
	ImGui::Separator();
}

void ImguiManager::Update_SphereCollider_Component(int index)
{
	auto collider = m_SelectedObject->GetComponent<ComponentEngine::SphereCollider>();
	bool componentActive = collider->isActive();
	std::string label = "##componentActive" + std::to_string(index);
	ImGui::Checkbox(label.c_str(), &componentActive);
	collider->SetActive(componentActive);
	ImGui::SameLine();

	if(ImGui::CollapsingHeader("SphereCollider"))
	{
		std::string componentIndex = std::to_string(collider->GetTypeID());
		std::string label;

		bool isTrigger = collider->IsTrigger();
		ImGui::Text("IsTrigger ");
		ImGui::SameLine();
		label = "##IsTrigger" + componentIndex + "_" + std::to_string(collider->m_TypeLocalIndex);
		ImGui::Checkbox(label.c_str(), &isTrigger);
		collider->SetTrigger(isTrigger);

		ImGui::Text("Center ");
		ImGui::SameLine();
		label = "##Center" + componentIndex + "_" + std::to_string(collider->m_TypeLocalIndex);
		ImGui::InputFloat3(label.c_str(), (float*)&(collider->m_Center));

		ImGui::Text("Radius ");
		ImGui::SameLine();
		label = "##Radius" + componentIndex + "_" + std::to_string(collider->m_TypeLocalIndex);
		ImGui::InputFloat(label.c_str(), (float*)&(collider->m_Radius));
	}
	ImGui::Separator();
}

void ImguiManager::Update_Rigidbody_Component(int index)
{
	auto rigidbody = m_SelectedObject->GetComponent<ComponentEngine::Rigidbody>();
	bool componentActive = rigidbody->isActive();
	std::string label = "##componentActive" + std::to_string(index);
	ImGui::Checkbox(label.c_str(), &componentActive);
	rigidbody->SetActive(componentActive);
	ImGui::SameLine();

	if(ImGui::CollapsingHeader("Rigidbody"))
	{
		std::string componentIndex = std::to_string(rigidbody->GetTypeID());
		std::string label;

		ImGui::Text("Mass ");
		ImGui::SameLine();
		label = "##Radius" + componentIndex + "_" + std::to_string(rigidbody->m_TypeLocalIndex);
		ImGui::InputFloat(label.c_str(), (float*)&(rigidbody->m_Mass));

		ImGui::Text("Drag ");
		ImGui::SameLine();
		label = "##Drag" + componentIndex + "_" + std::to_string(rigidbody->m_TypeLocalIndex);
		ImGui::InputFloat(label.c_str(), (float*)&(rigidbody->m_Drag));

		ImGui::Text("Angular Drag ");
		ImGui::SameLine();
		label = "##AngularDrag" + componentIndex + "_" + std::to_string(rigidbody->m_TypeLocalIndex);
		ImGui::InputFloat(label.c_str(), (float*)&(rigidbody->m_AngularDrag));

		ImGui::Text("UseGravity ");
		ImGui::SameLine();
		label = "##UseGravity" + componentIndex + "_" + std::to_string(rigidbody->m_TypeLocalIndex);
		ImGui::Checkbox(label.c_str(), &(rigidbody->m_UseGravity));

		int interpolate = static_cast<int>(rigidbody->m_Interpolate);
		ImGui::Text("Interpolate");
		label = "##Interpolate" + componentIndex + "_" + std::to_string(rigidbody->m_TypeLocalIndex);
		ImGui::Combo(label.c_str(), &interpolate, "Interpolate\0Extrapolate\0");
		rigidbody->m_Interpolate = static_cast<ComponentEngine::Rigidbody::eInterpolate>(interpolate);

		int collisiondetection = static_cast<int>(rigidbody->m_CollisionDetection);
		ImGui::Text("Collision Detection");
		label = "##CollisionDetection" + componentIndex + "_" + std::to_string(rigidbody->m_TypeLocalIndex);
		ImGui::Combo(label.c_str(), &interpolate, "Discrete\0Continuous\0Continuous_Dynamic\0Continuous_Speculative\0");
		rigidbody->m_CollisionDetection = static_cast<ComponentEngine::Rigidbody::eCollisionDetection>(collisiondetection);

		if (ImGui::CollapsingHeader("constraints"))
		{
			ImGui::Text("Freeze Position");
			ImGui::SameLine();
			label = "##FreezePosX" + componentIndex + "_" + std::to_string(rigidbody->m_TypeLocalIndex);
			ImGui::Checkbox(label.c_str(), &(rigidbody->m_FreezePosition.x));
			ImGui::SameLine();
			label = "##FreezePosY" + componentIndex + "_" + std::to_string(rigidbody->m_TypeLocalIndex);
			ImGui::Checkbox(label.c_str(), &(rigidbody->m_FreezePosition.y));
			ImGui::SameLine();
			label = "##FreezePosZ" + componentIndex + "_" + std::to_string(rigidbody->m_TypeLocalIndex);
			ImGui::Checkbox(label.c_str(), &(rigidbody->m_FreezePosition.z));

			ImGui::Text("Freeze Rotation");
			ImGui::SameLine();
			label = "##FreezeRotX" + componentIndex + "_" + std::to_string(rigidbody->m_TypeLocalIndex);
			ImGui::Checkbox(label.c_str(), &(rigidbody->m_FreezeRotation.x));
			ImGui::SameLine();
			label = "##FreezeRotY" + componentIndex + "_" + std::to_string(rigidbody->m_TypeLocalIndex);
			ImGui::Checkbox(label.c_str(), &(rigidbody->m_FreezeRotation.y));
			ImGui::SameLine();
			label = "##FreezeRotZ" + componentIndex + "_" + std::to_string(rigidbody->m_TypeLocalIndex);
			ImGui::Checkbox(label.c_str(), &(rigidbody->m_FreezeRotation.z));
		}
	}
	ImGui::Separator();
}
