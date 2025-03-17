#include "pch.h"
#include "AudioListener.h"
#include "Transform.h"
#include "CDebug.h"

ComponentEngine::AudioListener::AudioListener()
	: Component("AudioListener"), m_Volume(1.0f), m_isPause(false), m_ActiveState(true)
{

}

ComponentEngine::AudioListener::~AudioListener()
{

}

void ComponentEngine::AudioListener::Awake()
{
	if (m_GameObject)
	{
		Transform* _transfrom = m_GameObject->GetComponent<Transform>();
		SetPosition(_transfrom->GetPosition());
		
	}
}

void ComponentEngine::AudioListener::Update(float dTime)
{
	if (m_GameObject)
	{
		// Listener Position Update
		Transform* _transfrom = m_GameObject->GetComponent<Transform>();
		SetPosition(_transfrom->GetPosition());
	}
}

void ComponentEngine::AudioListener::DebugTextRender()
{

}

void ComponentEngine::AudioListener::OnQuit()
{

}

void ComponentEngine::AudioListener::OnDisable()
{

}

void ComponentEngine::AudioListener::OnDestroy()
{

}

void ComponentEngine::AudioListener::SetActive(bool value)
{
	if (value == true && m_ActiveState != value)
	{
		Start();
	}
	m_ActiveState = value;
}

void ComponentEngine::AudioListener::Bind(GameObject* obj)
{
	if (obj)
	{
		obj->m_AudioListener = this;
	}
}
