#include "pch.h"
#include "CMaterial.h"

ComponentEngine::CMaterial::CMaterial()
	: m_isEmission(false), m_Tiling(Vector2(1, 1)),
	m_Offset(Vector2(0, 0))
{
	GRAPHICENGINE::CreateMaterial_Mesh(&m_Material);
}

ComponentEngine::CMaterial::~CMaterial()
{

}

void ComponentEngine::CMaterial::SetNodeName(std::string nodeName)
{
	if (!m_AlbedoPath.empty())		m_Material->SetTexturePath(m_AlbedoPath, GRAPHICENGINE::IMaterialInterface::TextureSort::ALBEDO);
	if (!m_NormalMapPath.empty())	m_Material->SetTexturePath(m_NormalMapPath, GRAPHICENGINE::IMaterialInterface::TextureSort::NORMAL);
	if (!m_ORMPath.empty())			m_Material->SetTexturePath(m_ORMPath, GRAPHICENGINE::IMaterialInterface::TextureSort::ORM);
	if (!m_EmissionPath.empty())	m_Material->SetTexturePath(m_EmissionPath, GRAPHICENGINE::IMaterialInterface::TextureSort::EMISSIVE);
	if (!m_DetailMaskPath.empty())	m_Material->SetTexturePath(m_DetailMaskPath, GRAPHICENGINE::IMaterialInterface::TextureSort::MASKMAP);

	m_NodeName = nodeName;
	m_Material->SetLinkNodename(m_NodeName);
}

void ComponentEngine::CMaterial::CreateMaterialData(GRAPHICENGINE::IModelInterface* model)
{
	if (!m_AlbedoPath.empty())		model->SetMaterialData(m_Material, m_NodeName, GRAPHICENGINE::IMaterialInterface::TextureSort::ALBEDO);
	if (!m_NormalMapPath.empty())	model->SetMaterialData(m_Material, m_NodeName, GRAPHICENGINE::IMaterialInterface::TextureSort::NORMAL);
	if (!m_ORMPath.empty())			model->SetMaterialData(m_Material, m_NodeName, GRAPHICENGINE::IMaterialInterface::TextureSort::ORM);
	if (!m_EmissionPath.empty())	model->SetMaterialData(m_Material, m_NodeName, GRAPHICENGINE::IMaterialInterface::TextureSort::EMISSIVE);
	if (!m_DetailMaskPath.empty())	model->SetMaterialData(m_Material, m_NodeName, GRAPHICENGINE::IMaterialInterface::TextureSort::MASKMAP);
}