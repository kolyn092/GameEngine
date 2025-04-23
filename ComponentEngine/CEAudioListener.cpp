#include "pch.h"
#include "CEAudioListener.h"
#include "Transform.h"
#include "CDebug.h"

ComponentEngine::CEAudioListener::CEAudioListener()
	: ComponentImpl("AudioListener"), m_Volume(1.0f), m_isPause(false), m_ActiveState(true)
{

}

ComponentEngine::CEAudioListener::~CEAudioListener()
{

}

void ComponentEngine::CEAudioListener::Awake()
{
	if (m_GameObject)
	{
		Transform* _transfrom = m_GameObject->GetComponent<Transform>();
		SetPosition(_transfrom->GetPosition());
		
	}
}

void ComponentEngine::CEAudioListener::Update(float dTime)
{
	if (m_GameObject)
	{
		// Listener Position Update
		Transform* _transfrom = m_GameObject->GetComponent<Transform>();
		SetPosition(_transfrom->GetPosition());
	}
}

void ComponentEngine::CEAudioListener::DebugTextRender()
{

}

void ComponentEngine::CEAudioListener::OnQuit()
{

}

void ComponentEngine::CEAudioListener::OnDisable()
{

}

void ComponentEngine::CEAudioListener::OnDestroy()
{

}

void ComponentEngine::CEAudioListener::SetActive(bool value)
{
	if (value == true && m_ActiveState != value)
	{
		Start();
	}
	m_ActiveState = value;
}

void ComponentEngine::CEAudioListener::Bind(GameObject* obj)
{
	if (obj)
	{
		obj->m_AudioListener = this;
	}
}
