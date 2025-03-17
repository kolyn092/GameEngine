#include "pch.h"
#include "AudioClip.h"
#include "DXAudioEngine.h"

ComponentEngine::AudioClip::AudioClip(const TCHAR* soundPath)
	: m_Sound(nullptr), m_SoundPath(soundPath)
{
	m_Sound = new SoundEffect(DXAudioEngine::Ins()->GetEngine(), m_SoundPath);
}

ComponentEngine::AudioClip::~AudioClip()
{
	if (m_Sound)
	{
		delete m_Sound;
		m_Sound = nullptr;
	}

	DXAudioEngine::Ins()->Finalize();
}

std::unique_ptr<DirectX::SoundEffectInstance> ComponentEngine::AudioClip::CreateSoundInstance()
{
	return m_Sound->CreateInstance(DirectX::SoundEffectInstance_Use3D | DirectX::SoundEffectInstance_ReverbUseFilters);
}

WORD ComponentEngine::AudioClip::GetChannelCount()
{
	return m_Sound->GetFormat()->nChannels;
}
