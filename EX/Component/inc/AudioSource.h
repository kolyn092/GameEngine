#pragma once
#include "Component.h"
#include "AudioClip.h"

namespace ComponentEngine
{
	//////////////////////////////////////////////////////////////////////////
	// 
	// AudioClip 를 재생하려는 오브젝트에 넣을 컴포넌트
	// 
	//////////////////////////////////////////////////////////////////////////
	class AudioSource : public Component
	{

	private:
		AudioClip* m_AudioClip;
		//AudioMixerGroup m_AudioMixerGroup;
		bool m_isMute;
		float m_Volume;

		std::unique_ptr<DirectX::SoundEffectInstance> m_AudioSource;

		bool m_is3D;

		// 3D Sound Settings
		DirectX::AudioEmitter m_Emitter;
		float m_Spread;
		float m_MinDistance;
		float m_MaxDistance;

	public:
		bool m_isPlayOnAwake;
		bool m_isLoop;

	private:
		bool m_ActiveState;

	public:
		AudioSource();
		virtual ~AudioSource();

	public:
		void Play();
		void Pause();
		void Stop();
		void UnPause();
		void Reset();

	public:
		void SetAudioClip(AudioClip* clip);
		void SetMute(bool isMute);
		void SetVolume(float value);
		void SetRange(float range);
		void Set3D(bool value) { m_is3D = value; }

		bool IsMute() const { return m_isMute; }
		float GetVolume() const { return m_Volume; }

	public:
		AudioClip* GetAudioClip() { return m_AudioClip; }

	public:
		virtual void Awake() override;
		virtual void Start() override;

		virtual void Update() override;

		virtual void DebugTextRender() override;

		virtual void OnQuit() override;
		virtual void OnDisable() override;
		virtual void OnDestroy() override;

		virtual void SetActive(bool value) override;
		virtual bool isActive() override { return m_ActiveState; }

	public:
		virtual void Bind(GameObject* obj) override;
	};
}

