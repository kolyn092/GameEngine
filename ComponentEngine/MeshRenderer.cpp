#include "pch.h"
#include "MeshRenderer.h"
#include "CCamera.h"
#include "CDebug.h"
#include "Scene.h"

ComponentEngine::MeshRenderer::MeshRenderer()
	: Component("MeshRenderer"), m_isAnimLoop(true), m_ActiveState(true), m_ModelPath(""), m_Model(nullptr)
{
	m_ModelPathList.reserve(10);
	m_MaterialManager = new MaterialManager();
}

ComponentEngine::MeshRenderer::~MeshRenderer()
{
	// TODO : The block at 0x097A1998 was not allocated by _aligned routines, use free()
	if (m_Model)
	{
		//delete m_Model;
		m_Model = nullptr;
	}
}

void ComponentEngine::MeshRenderer::CreateModel()
{
	// ¸ğµ¨ ÇÏ³ª
	if (!(m_ModelPath.empty()))
	{
		GRAPHICENGINE::CreateModelObject(m_ModelPath);
	}

	// ¸ğµ¨ ¸®½ºÆ®
	if (!(m_ModelPathList.empty()))
	{
		GRAPHICENGINE::CreateModelObject_List(m_ModelPathList);
	}
}

void ComponentEngine::MeshRenderer::SetModel()
{
	// ¸ğµ¨ ÇÏ³ª
	if (!(m_ModelPath.empty()))
	{
		GRAPHICENGINE::SetModelObject(&m_Model, m_ModelPath);
	}

	// ¸ğµ¨ ¸®½ºÆ®
	if (!(m_ModelPathList.empty()))
	{
		GRAPHICENGINE::SetModelObject(&m_Model, m_ModelPathList[0]);
	}
}

void ComponentEngine::MeshRenderer::InitModel()
{
	if (m_Model)
	{
		m_Model->InitFbxModel();
	}
}

void ComponentEngine::MeshRenderer::ChangeAnimation(unsigned int index, bool value)
{
	if (m_Model)
	{
		if (m_ModelPathList.size() > index)
		{
			m_Model->SetAnimationLoop(value);
			m_Model->ChangeAnimation(index);
			IsAnimLoop(value);
		}
	}
}

void ComponentEngine::MeshRenderer::IsAnimLoop(bool value)
{
	if (m_isAnimLoop != value)
	{
		m_isAnimLoop = value;
	}
}

bool ComponentEngine::MeshRenderer::GetAnimationEnd()
{
	if (m_Model)
	{
		return m_Model->GetAniEndTimeing();
	}
	return false;
}

void ComponentEngine::MeshRenderer::SetAnimON()
{
	if (m_Model)
	{
		m_Model->SetAniOn();
	}
}

void ComponentEngine::MeshRenderer::SetAnimOFF()
{
	if (m_Model)
	{
		m_Model->SetAniOff();
	}
}

void ComponentEngine::MeshRenderer::SetUserMatrix(DirectX::SimpleMath::Matrix matrix)
{
	if (m_Model)
	{
		m_Model->SetUserMatrix(matrix);
	}
}

void ComponentEngine::MeshRenderer::CreateMaterialData()
{
	if (m_MaterialManager && m_Model)
	{
		m_MaterialManager->CreateMaterialData(m_Model);
	}
}

void ComponentEngine::MeshRenderer::SetMaterial(CMaterial* material)
{
	if (m_MaterialManager && material)
	{
		m_MaterialManager->SetMaterial(material);
	}
}

void ComponentEngine::MeshRenderer::InitModelTexture()
{
	if (m_Model)
	{
		m_Model->InitModelTexture();
	}
}

void ComponentEngine::MeshRenderer::SetEmissiveActive(bool state)
{
	if (m_Model)
	{
		m_Model->SetEmissiveMapOnOff(state);
	}
}

void ComponentEngine::MeshRenderer::AddEmissiveMaterial(std::string path)
{
	if (m_Model)
	{
		m_Model->AddEmissiveMap(path);
	}
}

void ComponentEngine::MeshRenderer::SetEmissiveMaterial(int index)
{
	if (m_Model)
	{
		m_Model->ChangeApplyEmissiveMap(index);
	}
}

void ComponentEngine::MeshRenderer::Awake()
{
	if (m_Model)
	{
		m_Model->SetAnimationLoop(true);
		m_Model->SetAniOn();
	}
}

void ComponentEngine::MeshRenderer::SetKey(int key)
{
	if (m_Model)
	{
		m_Model->SetNowKeyCount(key);
	}
}

int ComponentEngine::MeshRenderer::GetNowKey() const
{
	return m_Model->GetAniNowKeyCount();
}

int ComponentEngine::MeshRenderer::GetTotalKey() const
{
	return m_Model->GetAniTotalKeyCount();
}


void ComponentEngine::MeshRenderer::SetAnimationSpeed(float speed)
{
	m_Model->SetAnimationSpeed(speed);
}

DirectX::SimpleMath::Matrix ComponentEngine::MeshRenderer::GetNodeMatrix(std::string nodeName)
{
	if (m_Model)
	{
		return m_Model->GetHiearchyMatrixByName(nodeName);
	}
	return Matrix::Identity;
}

void ComponentEngine::MeshRenderer::Update(float dTime)
{
	if (m_Model)
	{
		m_Model->FbxModelUpdate(dTime);
	}
}

void ComponentEngine::MeshRenderer::DebugTextRender()
{
	//CDebug::Ins()->DebugDrawText(L"%s %s", L"L", L"MeshRenderer");
}

void ComponentEngine::MeshRenderer::OnQuit()
{

}

void ComponentEngine::MeshRenderer::OnDisable()
{

}

void ComponentEngine::MeshRenderer::OnDestroy()
{

}

void ComponentEngine::MeshRenderer::SetActive(bool value)
{
	if (m_Model)
	{
		m_Model->SetDrawScreen(value);
	}

	if (value == true && m_ActiveState != value)
	{
		Start();
	}
	m_ActiveState = value;
}

void ComponentEngine::MeshRenderer::Bind(GameObject* obj)
{
	if (obj)
	{
		obj->m_MeshRenderer = this;
	}
}
