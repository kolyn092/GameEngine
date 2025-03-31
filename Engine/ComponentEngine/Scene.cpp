#include "pch.h"
#include "Scene.h"
#include "MeshRenderer.h"
#include "Transform.h"
#include "CDebug.h"
#include "Rigidbody.h"
#include "BoxCollider.h"
#include "SphereCollider.h"
#include "CapsuleCollider.h"
#include "PhysX.h"
#include "Input.h"
#include "CCamera.h"
#include "CUtil.h"

#include "SceneManager.h"

using namespace std;

ComponentEngine::Scene::Scene()
	: m_SceneName(""), m_MainCamera(nullptr), m_AudioListener(nullptr),
	m_isInputMouseLock(false), m_ShowDebug(true),
	m_isLoadFbxComplete(false), m_isSetModelComplete(false),
	m_LoadFbxCompleteCount(0), m_ObjectCount(0),
	m_Handle(NULL)
{

}

ComponentEngine::Scene::Scene(std::string sceneName)
	: m_SceneName(sceneName), m_MainCamera(nullptr), m_AudioListener(nullptr),
	m_isInputMouseLock(false), m_ShowDebug(true),
	m_isLoadFbxComplete(false), m_isSetModelComplete(false),
	m_LoadFbxCompleteCount(0), m_ObjectCount(0),
	m_Handle(NULL) {

}

ComponentEngine::Scene::~Scene()
{
	OnDestroy();

	for (auto& obj : m_GameObjects)
	{
		if (obj)
		{
			delete obj;
			obj = nullptr;
		}
	}

	if (m_MainCamera)
	{
		m_MainCamera = nullptr;
	}

	if (m_AudioListener)
	{
		m_AudioListener = nullptr;
	}
}

void ComponentEngine::Scene::LoadFbxFile()
{
	GRAPHICENGINE::ClearRenderQueue_Model();
	GRAPHICENGINE::ClearRenderQueue_UI();
#ifdef _ADDLOADINGSCENE
	m_Handle = CreateEvent(NULL, NULL, FALSE, NULL);
#endif

	for (GameObject* obj : m_GameObjects)
	{
		MeshRenderer* _mesh = obj->GetComponent<MeshRenderer>();
		if (_mesh == nullptr)
		{
			continue;
		}

		_mesh->CreateModel();

		m_LoadFbxCompleteCount++;
	}

	m_isLoadFbxComplete = true;

#ifdef _ADDLOADINGSCENE
	SetEvent(m_Handle);
#endif
}

void ComponentEngine::Scene::SetFbxModel()
{
	GRAPHICENGINE::CreateModelMaterial_Texture();

	for (GameObject* obj : m_GameObjects)
	{
		MeshRenderer* _mesh = obj->GetComponent<MeshRenderer>();
		if (_mesh == nullptr)
		{
			continue;
		}

		_mesh->SetModel();
		_mesh->InitModel();
	}

	m_isSetModelComplete = true;
}

// 게임 오브젝트를 생성하는 함수
void ComponentEngine::Scene::AddGameObjectByName(string name)
{
	ComponentEngine::GameObject _obj = ComponentEngine::GameObject(name);
	m_GameObjects.push_back(&_obj);
}

void ComponentEngine::Scene::AddGameObject(ComponentEngine::GameObject* obj)
{
	m_GameObjects.push_back(obj);
	obj->SetScene(this);

	ComponentEngine::MeshRenderer* _mesh = obj->GetComponent<ComponentEngine::MeshRenderer>();
	if (_mesh)
	{
		m_ObjectCount++;
	}

	AddRootObject(obj);
}

void ComponentEngine::Scene::AddGameObject(GameObject* obj, GameObject* parent)
{
	if (parent)
	{
		parent->GetComponent<Transform>()->AttatchChild(obj->GetComponent<Transform>());
		parent->SetChildObject(obj);
	}
	else
	{
		m_RootObjects.emplace_back(obj);
	}
	m_GameObjects.emplace_back(obj);
}

void ComponentEngine::Scene::AddRootObject(GameObject* obj)
{
	m_RootObjects.push_back(obj);
	obj->SetScene(this);
}

ComponentEngine::GameObject* ComponentEngine::Scene::FindGameObjectWithName(std::string name)
{
	for (unsigned int i = 0; i < m_GameObjects.size(); i++)
	{
		if (m_GameObjects[i]->GetName() == name)
		{
			return m_GameObjects[i];
		}
	}
	return nullptr;
}

void ComponentEngine::Scene::DeleteGameObject(ComponentEngine::GameObject* obj)
{
	ComponentEngine::MeshRenderer* _mesh = obj->GetComponent<ComponentEngine::MeshRenderer>();
	if (_mesh)
	{
		m_ObjectCount--;
	}

	DeleteRootObject(obj);
	for (auto it = m_GameObjects.begin(); it != m_GameObjects.end(); ++it)
	{
		if ((*it) == obj)
		{
			m_GameObjects.erase(it);
			return;
		}
	}
}

void ComponentEngine::Scene::DeleteRootObject(GameObject* obj)
{
	for (auto it = m_RootObjects.begin(); it != m_RootObjects.end(); ++it)
	{
		if ((*it) == obj)
		{
			m_RootObjects.erase(it);
			return;
		}
	}
}

void ComponentEngine::Scene::Awake()
{
	for (ComponentEngine::GameObject* _pGO : m_RootObjects)
	{
		_pGO->Awake();
	}
}

void ComponentEngine::Scene::OnEnable()
{
	for (ComponentEngine::GameObject* _pGO : m_RootObjects)
	{
		_pGO->OnEnable();
	}
}

void ComponentEngine::Scene::Start()
{
	for (ComponentEngine::GameObject* _pGO : m_RootObjects)
	{
		_pGO->Start();
	}
}

void ComponentEngine::Scene::Update(float dTime)
{
	for (ComponentEngine::GameObject* _pGO : m_RootObjects)
	{
		_pGO->FixedUpdate(dTime);
	}

	PhysXEngine::PhysX::Ins()->Update();

	for (ComponentEngine::GameObject* _pGO : m_RootObjects)
	{
		_pGO->PhysicsUpdate(dTime);
	}

	Input::Ins()->Update();
	Input::Ins()->SetMouseLock(m_isInputMouseLock);

	for (ComponentEngine::GameObject* _pGO : m_RootObjects)
	{
		_pGO->Update(dTime);
	}

	for (ComponentEngine::GameObject* _pGO : m_RootObjects)
	{
		_pGO->LateUpdate(dTime);
	}

	if (m_MainCamera)
	{
		GRAPHICENGINE::GetCameraMatrixOnce(m_MainCamera->GetViewMatrix(), m_MainCamera->GetProjMatrix());
	}

}

void ComponentEngine::Scene::Render()
{
	if (m_isLoadFbxComplete == false)
		return;

	CDebug::Ins()->Reset();
	for (ComponentEngine::GameObject* _pGO : m_GameObjects)
	{
		_pGO->OnPreRender();
	}

#ifdef _ADDLOADINGSCENE
	if (SceneManager::Ins()->GetNowSceneIndex() != 2)
	{
		GRAPHICENGINE::DrawAll(0);
	}
#else
	GRAPHICENGINE::DrawAll(0);
#endif

	for (ComponentEngine::GameObject* _pGO : m_GameObjects)
	{
		_pGO->OnRender();
	}

	for (ComponentEngine::GameObject* _pGO : m_GameObjects)
	{
		_pGO->OnPostRender();
	}

	for (ComponentEngine::GameObject* _pGO : m_GameObjects)
	{
		_pGO->OnDrawGizmos();
	}

	for (ComponentEngine::GameObject* _pGO : m_RootObjects)
	{
		_pGO->OnGUI();
	}

#ifdef _DEBUG
	for (ComponentEngine::GameObject* _pGO : m_GameObjects)
	{
		_pGO->DebugTextRender();
	}
#else
#endif
	CDebug::Ins()->DebugDrawText(L"FPS : %.f", GRAPHICENGINE::GetFramePerSecond());
}

void ComponentEngine::Scene::OnDestroy()
{
	for (ComponentEngine::GameObject* _pGO : m_RootObjects)
	{
		_pGO->OnQuit();

		Transform* _trans = _pGO->GetComponent<Transform>();
		if (_trans->GetChildCount() > 0)
		{
			for (int i = 0; i < _trans->GetChildCount(); i++)
			{
				_trans->GetChild(i)->m_GameObject->OnQuit();
			}
		}
	}

	for (ComponentEngine::GameObject* _pGO : m_RootObjects)
	{
		_pGO->OnDisable();

		Transform* _trans = _pGO->GetComponent<Transform>();
		if (_trans->GetChildCount() > 0)
		{
			for (int i = 0; i < _trans->GetChildCount(); i++)
			{
				_trans->GetChild(i)->m_GameObject->OnDisable();
			}
		}
	}

	for (ComponentEngine::GameObject* _pGO : m_RootObjects)
	{
		_pGO->OnDestroy();

		Transform* _trans = _pGO->GetComponent<Transform>();
		if (_trans->GetChildCount() > 0)
		{
			for (int i = 0; i < _trans->GetChildCount(); i++)
			{
				_trans->GetChild(i)->m_GameObject->OnDestroy();
			}
		}
	}
}

void ComponentEngine::Scene::SetInputMouseLock(bool state)
{
	if (m_isInputMouseLock != state)
	{
		m_isInputMouseLock = state;

		if (m_isInputMouseLock)
		{
			ShowCursor(false);
		}
		else
		{
			ShowCursor(true);
		}
	}
}
