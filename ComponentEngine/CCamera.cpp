#include "pch.h"
#include "CCamera.h"
#include "Transform.h"
#include "CUtil.h"
#include "CDebug.h"
#include "Input.h"

using namespace DirectX::SimpleMath;

ComponentEngine::CECamera::CECamera()
	: Component("Camera"), m_ViewPortWidth(1920), m_ViewPortHeight(1080), m_FOV(0),
	m_AspectRatio(0), m_Near(1.0f), m_Far(1000.0f), m_BackGroundColor(Color(1, 1, 1, 1)),
	m_TargetObj(nullptr), m_View(Matrix::Identity), m_Proj(Matrix::Identity),
	m_Mode(eMode::DEFAULT), m_ClearFlags(eClearFlags::SOLIDCOLOR), m_ViewMode(eProjection::PERSPECTIVE),
	m_ActiveState(true)
{
	m_FOV = XMConvertToRadians(60);
	m_AspectRatio = static_cast<float>(m_ViewPortWidth) / static_cast<float>(m_ViewPortHeight);
}

ComponentEngine::CECamera::~CECamera()
{
	if (m_TargetObj)
	{
		m_TargetObj = nullptr;
	}
}

void ComponentEngine::CECamera::SetViewPort(int width, int height)
{
	m_ViewPortWidth = width;
	m_ViewPortHeight = height;

	m_AspectRatio = static_cast<float>(m_ViewPortWidth) / static_cast<float>(m_ViewPortHeight);
}

void ComponentEngine::CECamera::SetFOV(float fovDegree)
{
	m_FOV = XMConvertToRadians(fovDegree);
}

void ComponentEngine::CECamera::SetAspectRatio(float aspectRatio)
{
	m_AspectRatio = aspectRatio;
}

void ComponentEngine::CECamera::SetClippingPlanes(float camnear, float camfar)
{
	m_Near = camnear;
	m_Far = camfar;
}

void ComponentEngine::CECamera::SetClippingPlanes_Near(float camnear)
{
	m_Near = camnear;
}

void ComponentEngine::CECamera::SetClippingPlanes_Far(float camfar)
{
	m_Far = camfar;
}

void ComponentEngine::CECamera::SetBackgroundColor(Color col)
{
	m_BackGroundColor = col;
}

void ComponentEngine::CECamera::SetFollowTarget(GameObject* obj)
{
	m_TargetObj = obj;
}

void ComponentEngine::CECamera::SetFollowTarget(DirectX::SimpleMath::Vector3 pos)
{
	m_FollowTargetPos = pos;
}

void ComponentEngine::CECamera::SetCameraMode(eMode mode /*= eMode::DEFAULT*/, Vector3 pos /*= Vector3::Zero*/)
{
	m_Mode = mode;
	m_FollowTargetPos = pos;
}

void ComponentEngine::CECamera::SetClearFlags(eClearFlags flag /*= eClearFlags::SOLIDCOLOR*/)
{
	m_ClearFlags = flag;
}

void ComponentEngine::CECamera::SetViewMode(eProjection viewMode)
{
	m_ViewMode = viewMode;
}

void ComponentEngine::CECamera::ConvertViewMode(int viewMode)
{
	m_ViewMode = static_cast<eProjection>(viewMode);
}

void ComponentEngine::CECamera::FixedUpdate()
{
	// Input에서 월드상에 피킹된 좌표로 변환해주는걸 하기 위해서는 카메라 view, proj 세팅이 필요하다.
	Input::Ins()->SetCameraMatrix(m_View, m_Proj);
}

void ComponentEngine::CECamera::LateUpdate(float dTime)
{
	Transform* _targetTransform = nullptr;

	if (m_TargetObj)
	{
		_targetTransform = m_TargetObj->GetComponent<Transform>();
	}

	Transform* _transform = m_GameObject->GetComponent<Transform>();
	if (_transform)
	{
		switch (m_Mode)
		{
			case eMode::DEFAULT:
			{
				m_View = Matrix::Identity
					* Matrix::CreateFromQuaternion(_transform->GetQuaternion())
					* Matrix::CreateTranslation(_transform->GetPosition());
				m_View = m_View.Invert();
			}
			break;
			case eMode::FOLLOW:
			{
				DirectX::SimpleMath::Vector3 _pos = _transform->GetPosition();
				m_View = DirectX::XMMatrixLookAtLH(_pos, m_FollowTargetPos, _transform->m_Up);
			}
			break;
			default:
				break;
		}

		switch (m_ViewMode)
		{
			case ComponentEngine::CECamera::eProjection::PERSPECTIVE:
				m_Proj = DirectX::XMMatrixPerspectiveFovLH(m_FOV, m_AspectRatio, m_Near, m_Far);
				break;
			case ComponentEngine::CECamera::eProjection::ORTHOGRAPHIC:
				m_Proj = DirectX::XMMatrixOrthographicLH(static_cast<float>(m_ViewPortWidth), static_cast<float>(m_ViewPortHeight), m_Near, m_Far);
				break;
			default:
				break;
		}
	}
}

void ComponentEngine::CECamera::DebugTextRender()
{

}

void ComponentEngine::CECamera::OnQuit()
{

}

void ComponentEngine::CECamera::OnDisable()
{

}

void ComponentEngine::CECamera::OnDestroy()
{

}

void ComponentEngine::CECamera::SetActive(bool value)
{
	if (value == true && m_ActiveState != value)
	{
		Start();
	}
	m_ActiveState = value;
}

void ComponentEngine::CECamera::Bind(GameObject* obj)
{

}

