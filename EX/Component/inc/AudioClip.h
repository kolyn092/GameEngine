#pragma once
#include "Audio.h"

namespace ComponentEngine
{
	///
	///
	///  Audio Original Source
	///
	///  [12/9/2020 KoSuKyung]

	class AudioClip
	{

	private:
		DirectX::SoundEffect* m_Sound;
		const TCHAR* m_SoundPath;

		//float m_Length;
		//bool m_isReadyToPlay;
		//int m_Frequency;
		//int m_Channels;
		//bool m_Samples;
		//bool m_Ambisonic;
		//bool m_PreLoadAudioData;
		//bool m_LoadInBackground;

	public:
		AudioClip(const TCHAR* soundPath);
		~AudioClip();

	public:
		std::unique_ptr<DirectX::SoundEffectInstance> CreateSoundInstance();
		WORD GetChannelCount();

	public:
		const TCHAR* GetSoundPath() { return m_SoundPath; }
	};
}

