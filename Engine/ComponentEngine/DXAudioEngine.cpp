#include "pch.h"
#include "DXAudioEngine.h"

ComponentEngine::DXAudioEngine* ComponentEngine::DXAudioEngine::m_pInstance = nullptr;

ComponentEngine::DXAudioEngine::DXAudioEngine()
{
	//	AUDIO_ENGINE_FLAGS _flag = AudioEngine_EnvironmentalReverb | AudioEngine_ReverbUseFilters;
	//#if defined(_DEBUG)||defined(DEBUG)
	//	_flag |= DirectX::AudioEngine_Debug;
	//#endif
	//	m_AudioEngine = new DirectX::AudioEngine(_flag);

	m_AudioEngine = new DirectX::AudioEngine();
}

ComponentEngine::DXAudioEngine::DXAudioEngine(AUDIO_ENGINE_FLAGS flag)
{
	m_AudioEngine = new DirectX::AudioEngine(flag);
}

ComponentEngine::DXAudioEngine::~DXAudioEngine()
{
	if (m_AudioEngine)
	{
		delete m_AudioEngine;
		m_AudioEngine = nullptr;
	}
}

void ComponentEngine::DXAudioEngine::Finalize()
{
	if (m_pInstance)
	{
		delete m_pInstance;
		m_pInstance = nullptr;
	}
}
