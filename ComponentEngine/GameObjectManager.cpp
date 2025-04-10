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

// ���� ������Ʈ�� �����ϴ� �Լ�
void ComponentEngine::GameObjectManager::AddGameObjectByName(std::string name)
{
	// �ӽ� ��ü�� ������ �ʰ� ���Ϳ� ����ֱ� ���� emplace_back ���
	// ���Ϳ� �ִ� ��ü�� �����ͷ� ������ �ȵȴ�.
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
