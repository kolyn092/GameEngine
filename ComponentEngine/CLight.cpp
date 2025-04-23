#include "pch.h"
#include "Scene.h"
#include "CLight.h"
#include "CCamera.h"
#include "CDebug.h"
#include "Transform.h"

using namespace DirectX::SimpleMath;

ComponentEngine::CELight::CELight()
	: ComponentImpl("Light"), m_Light(nullptr), m_FogColor(Color(1, 1, 1, 1)), m_FogDensity(0.03f), m_FogActive(true), m_LightType(eLightType::DIRECTIONAL), m_Range(10),
	m_Color(Color(1, 1, 1, 1)), m_Intensity(0.5f), m_IndirectMultiplier(0.2f),
	m_ShadowType(eShadowType::NONE), m_ActiveState(true)
{

}

ComponentEngine::CELight::CELight(eLightType lightType)
	: ComponentImpl("Light"), m_Light(nullptr), m_FogColor(Color(1, 1, 1, 1)), m_FogDensity(0.03f), m_FogActive(true), m_LightType(lightType), m_Range(10),
	m_Color(Color(1, 1, 1, 1)), m_Intensity(1), m_IndirectMultiplier(1),
	m_ShadowType(eShadowType::NONE), m_ActiveState(true)
{

}

ComponentEngine::CELight::~CELight()
{
	if (m_Light)
	{
		delete m_Light;
		m_Light = nullptr;
	}
}

void ComponentEngine::CELight::SetFogActive(bool state)
{
	m_FogActive = state;
	GRAPHICENGINE::SetFogOnOff(m_FogActive);
}

void ComponentEngine::CELight::SetFogColor(Color col)
{
	m_FogColor = col;
	GRAPHICENGINE::SetFogColor(m_FogColor);
}

void ComponentEngine::CELight::SetFogDensity(float value)
{
	m_FogDensity = value;
	GRAPHICENGINE::SetFogDensity(m_FogDensity);
}

void ComponentEngine::CELight::ConvertLightType(int lightType)
{
	m_LightType = static_cast<eLightType>(lightType);
}

void ComponentEngine::CELight::Awake()
{
	/// TODO : 이 시점에서 하면 Transform 초기 설정이 안먹음.
	///m_GameObject->GetComponent<Transform>()->SetRotate(Vector3(50, -30, 0));

	Quaternion _quat = m_GameObject->GetComponent<Transform>()->GetQuaternion();
	Matrix _mRot = Matrix::CreateFromQuaternion(_quat);
	Vector3 _dirVec3 = Vector3::TransformNormal(Vector3(1, -3, 2), _mRot);

	Vector4 _dir = Vector4(_dirVec3.x, _dirVec3.y, _dirVec3.z, 1);

	Vector4 _diff = m_Color;
	Vector4 _amb = m_Color * m_IndirectMultiplier;
	_amb.w = 1;

	GRAPHICENGINE::CreateDirectionLight(&m_Light);
	m_Light->InitializeDirectrionLight(_dir, _diff, _amb);
	GRAPHICENGINE::GetDirectionLightData(_dir, _diff, _amb);

	GRAPHICENGINE::SetFogOnOff(m_FogActive);
	GRAPHICENGINE::SetFogColor(m_FogColor);
	GRAPHICENGINE::SetFogDensity(m_FogDensity);
}

void ComponentEngine::CELight::LateUpdate(float dTime)
{
	/// 그림자 갱신
	CECamera* _mainCam = m_GameObject->GetScene()->GetMainCamera();
	if (_mainCam)
	{
		CECamera::eMode _cameraMode = _mainCam->GetCameraMode();
		Vector3 _pos = Vector3::Zero;
		switch (_cameraMode)
		{
			case ComponentEngine::CECamera::eMode::DEFAULT:
				_pos = _mainCam->m_GameObject->GetComponent<Transform>()->GetPosition();
				break;
			case ComponentEngine::CECamera::eMode::FOLLOW:
				_pos = _mainCam->GetFollowTargetPos();
				break;
			default:
				break;
		}
		GRAPHICENGINE::SetShadow(true, _pos, 100.f);
	}

	/// 라이트 갱신 (Direction Light)
	Quaternion _quat = m_GameObject->GetComponent<Transform>()->GetQuaternion();
	Matrix _mRot = Matrix::CreateFromQuaternion(_quat);
	Vector3 _dirVec3 = Vector3::TransformNormal(Vector3(0, 0, 1), _mRot);

	Vector4 _dir = Vector4(_dirVec3.x, _dirVec3.y, _dirVec3.z, 1);

	Vector4 _diff = m_Color;
	Vector4 _amb = m_Color * m_IndirectMultiplier;
	_amb.w = 1;

	m_Light->InitializeDirectrionLight(_dir, _diff, _amb);
	GRAPHICENGINE::GetDirectionLightData(_dir, _diff, _amb);

}

void ComponentEngine::CELight::DebugTextRender()
{
	//CDebug::Ins()->DebugDrawText(L"%s %s", L"L", L"CLight");
}

void ComponentEngine::CELight::OnQuit()
{

}

void ComponentEngine::CELight::OnDisable()
{

}

void ComponentEngine::CELight::OnDestroy()
{

}

void ComponentEngine::CELight::SetActive(bool value)
{
	if (value == true && m_ActiveState != value)
	{
		Start();
	}
	m_ActiveState = value;
}

void ComponentEngine::CELight::Bind(GameObject* obj)
{

}
