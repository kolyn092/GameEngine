#include "pch.h"
#include "CSkybox.h"

ComponentEngine::Skybox::Skybox()
	: Component("Skybox"), m_FilePath("")
{

}

ComponentEngine::Skybox::~Skybox()
{

}

void ComponentEngine::Skybox::Awake()
{	
	GRAPHICENGINE::CreateSkybox_Obj(&m_pSkybox, m_FilePath);
	m_pSkybox->InitSkybox();
}

void ComponentEngine::Skybox::Bind(GameObject* obj)
{

}
