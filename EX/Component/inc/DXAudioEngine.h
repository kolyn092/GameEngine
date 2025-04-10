#pragma once

namespace ComponentEngine
{
	class DXAudioEngine
	{

#pragma region SINGLETON
	private:
		static DXAudioEngine* m_pInstance;

	public:
		static DXAudioEngine* Ins()
		{
			if (m_pInstance == nullptr)
			{
				m_pInstance = new DXAudioEngine();
			}
			return m_pInstance;
		}
#pragma endregion

	private:
		DirectX::AudioEngine* m_AudioEngine;

	private:
		DXAudioEngine();
		DXAudioEngine(AUDIO_ENGINE_FLAGS flag);
		~DXAudioEngine();

	public:
		DirectX::AudioEngine* GetEngine() const { return m_AudioEngine; }

	public:
		void Finalize();

	};
}

