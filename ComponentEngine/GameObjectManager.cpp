#include "pch.h"
#include "GameObjectManager.h"
#include "CCamera.h"
#include "CGrid.h"
#include "CAxis.h"
#include "MeshRenderer.h"

ComponentEngine::GameObjectManager::GameObjectManager()
{
	GameObject _obj("debugObject");

	//// Camera
	//CCamera* _camera = new CCamera();
	//_obj.AddComponent<CCamera*>(_camera);

	// Grid
	CGrid* _grid = new CGrid();
	_obj.AddComponent<CGrid*>(_grid);

	// Axis
	CAxis* _axis = new CAxis();
	_obj.AddComponent<CAxis*>(_axis);

	AddGameObject(&_obj);
}

ComponentEngine::GameObjectManager::~GameObjectManager()
{

}

void ComponentEngine::GameObjectManager::LoadModelData()
{
	FbxLoader _fbxLoader;
	_fbxLoader.InitializeFbxLoader();
	for (GameObject obj : m_GameObjects)
	{
		MeshRenderer* _mesh = obj.GetComponent<MeshRenderer>();
		if (_mesh == nullptr)
		{
			continue;
		}

		if (!(_mesh->m_ModelPath.empty()))
		{
			_mesh->m_Model = new FbxModel();
			_fbxLoader.LoadModel(_mesh->m_ModelPath, _mesh->m_Model);
			_mesh->m_Model->InitFbxModel();
		}
	}
	_fbxLoader.ReleaseFbxLoader();
}

// 게임 오브젝트를 생성하는 함수
void ComponentEngine::GameObjectManager::AddGameObjectByName(std::string name)
{
	// 임시 객체를 만들지 않고 벡터에 집어넣기 위해 emplace_back 사용
	// 벡터에 넣는 객체를 포인터로 넣으면 안된다.
	m_GameObjects.emplace_back(name);
}

void ComponentEngine::GameObjectManager::AddGameObject(ComponentEngine::GameObject* obj)
{
	m_GameObjects.push_back(*obj);
}

void ComponentEngine::GameObjectManager::Update(float dTime)
{
	for (ComponentEngine::GameObject _pGO : m_GameObjects)
	{
		_pGO.Update(dTime);
	}
}

void ComponentEngine::GameObjectManager::Render()
{
	for (ComponentEngine::GameObject _pGO : m_GameObjects)
	{
		_pGO.Render();
	}
}
