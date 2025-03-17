#include "pch.h"
#include "AudioSource.h"
#include "AudioListener.h"
#include "Transform.h"
#include "CDebug.h"
#include "Scene.h"

static float EmitterAngle[] =
{
	5.8294f,		// Front Left (Degree 334)
	0.453786f,		// Front Right (Degree 26)
	0.f,			// Center (Degree 0)
	X3DAUDIO_2PI,	// Low-frequency effects (Degree 360)
	3.7960911f,		// Back Left (Degree 260)
	1.74533f,		// Back Right (Degree 100)
	4.53786f,		// Surround Left (Degree 217.5)
	2.4870942f,		// Surround Right (Degree 142.5)
};

ComponentEngine::AudioSource::AudioSource()
	: Component("AudioSource"), m_AudioClip(nullptr), m_isMute(false), m_isPlayOnAwake(true), m_isLoop(true), m_Volume(1.0f),
	m_AudioSource(nullptr), m_is3D(false), m_Spread(40.0f), m_MinDistance(1.0f), m_MaxDistance(500.0f),
	m_ActiveState(true)
{

}

ComponentEngine::AudioSource::~AudioSource()
{
	if (m_AudioClip)
	{
		m_AudioSource->Stop();
		m_AudioClip = nullptr;
	}

	if (m_AudioSource)
	{
		m_AudioSource.release();
	}
}

void ComponentEngine::AudioSource::Play()
{
	if (m_AudioSource)
	{
		m_AudioSource->Play(m_isLoop);
	}
}

void ComponentEngine::AudioSource::Pause()
{
	if (m_AudioSource)
	{
		m_AudioSource->Pause();
	}
}

void ComponentEngine::AudioSource::Stop()
{
	if (m_AudioSource)
	{
		m_AudioSource->Stop();
	}
}

void ComponentEngine::AudioSource::UnPause()
{
	if (m_AudioSource)
	{
		m_AudioSource->Resume();
	}
}

void ComponentEngine::AudioSource::Reset()
{
	m_AudioClip = nullptr;

	if (m_AudioSource)
	{
		m_AudioSource.reset();
	}

	m_Emitter = DirectX::AudioEmitter();
	m_isMute = false;
	m_isPlayOnAwake = true;
}

void ComponentEngine::AudioSource::SetAudioClip(AudioClip* clip)
{
	if (clip)
	{
		m_AudioClip = clip;
		m_AudioSource = m_AudioClip->CreateSoundInstance();
		m_Emitter.ChannelCount = m_AudioClip->GetChannelCount();
		m_Emitter.pChannelAzimuths = EmitterAngle;

		// 소리 들리는 제한 범위 설정
		m_Emitter.CurveDistanceScaler = m_Spread;
		m_Emitter.pVolumeCurve = const_cast<X3DAUDIO_DISTANCE_CURVE*>(&X3DAudioDefault_LinearCurve);
	}
}

void ComponentEngine::AudioSource::SetMute(bool isMute)
{
	m_isMute = isMute;
	if (m_isMute)
	{
		m_AudioSource->SetVolume(0);
	}
	else
	{
		m_AudioSource->SetVolume(m_Volume);
	}
}

void ComponentEngine::AudioSource::SetPlayOnAwake(bool value)
{
	m_isPlayOnAwake = value;
}

void ComponentEngine::AudioSource::SetLoop(bool isLoop)
{
	m_isLoop = isLoop;
}

void ComponentEngine::AudioSource::SetVolume(float value)
{
	if (m_AudioSource)
	{
		m_Volume = value;
		m_AudioSource->SetVolume(m_Volume);
	}
}

void ComponentEngine::AudioSource::SetRange(float range)
{
	if (m_AudioClip != nullptr && m_is3D == true)
	{
		// 소리 들리는 제한 범위 설정
		m_Emitter.CurveDistanceScaler = m_Spread;
		m_Emitter.pVolumeCurve = const_cast<X3DAUDIO_DISTANCE_CURVE*>(&X3DAudioDefault_LinearCurve);
	}
}

void ComponentEngine::AudioSource::Awake()
{
	if (m_GameObject)
	{
		Transform* _transfrom = m_GameObject->GetComponent<Transform>();
		m_Emitter.SetPosition(_transfrom->GetPosition());
	}
}

void ComponentEngine::AudioSource::Start()
{
	if (m_isPlayOnAwake)
	{
		Play();
	}
}

void ComponentEngine::AudioSource::Update()
{
	if (m_GameObject)
	{
		if (m_is3D && m_AudioSource)
		{
			// Listener Position Update ?

			// Emitter Position Update
			Transform* _transfrom = m_GameObject->GetComponent<Transform>();
			m_Emitter.SetPosition(_transfrom->GetPosition());

			AudioListener* _listener = m_GameObject->GetScene()->GetAudioListener();
			if (_listener)
			{
				m_AudioSource->Apply3D(*_listener, m_Emitter);
			}
		}
	}
}

void ComponentEngine::AudioSource::DebugTextRender()
{
	//CDebug::Ins()->DebugDrawText(L"%s : %s", L"L", L"AudioSource");
}

void ComponentEngine::AudioSource::OnQuit()
{
	if (m_AudioClip)
	{
		Stop();
		m_AudioClip = nullptr;
	}
}

void ComponentEngine::AudioSource::OnDisable()
{

}

void ComponentEngine::AudioSource::OnDestroy()
{

}

void ComponentEngine::AudioSource::SetActive(bool value)
{
	if (value == true && m_ActiveState != value)
	{
		Start();
	}
	m_ActiveState = value;
}

void ComponentEngine::AudioSource::Bind(GameObject* obj)
{

}
