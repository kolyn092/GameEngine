#include "pch.h"
#include "MaterialManager.h"

ComponentEngine::MaterialManager::MaterialManager()
{

}

ComponentEngine::MaterialManager::~MaterialManager()
{
	
}

void ComponentEngine::MaterialManager::SetMaterial(CMaterial* material)
{
	if (material)
	{
		m_ModelMaterialList.push_back(material);
	}
}

void ComponentEngine::MaterialManager::CreateMaterialData(GRAPHICENGINE::IModelInterface* model)
{
	for (auto material : m_ModelMaterialList)
	{
		material->CreateMaterialData(model);
	}
}