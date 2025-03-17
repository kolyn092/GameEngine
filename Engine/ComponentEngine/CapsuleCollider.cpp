#include "pch.h"
#include "CUtil.h"
#include "CapsuleCollider.h"
#include "Transform.h"

//////////////////////////////////////////////////////////////////////////
//
// Capsule Collider
//
//////////////////////////////////////////////////////////////////////////

ComponentEngine::CapsuleCollider::CapsuleCollider()
	: Collider("CapsuleCollider"), m_Center(Vector3::Zero), m_Radius(2.0f), m_HalfHeight(4.0f),
	m_Direction(eCapsuleDirection::YAxis), m_Rotate(Quaternion::Identity)
{
	SetRotateByDirection();

	m_ComponentName = "CapsuleCollider";
}

ComponentEngine::CapsuleCollider::~CapsuleCollider()
{

}

void ComponentEngine::CapsuleCollider::SetRadius(float radius)
{
	if (radius > 0)
	{
		m_Radius = radius;
		if (m_Shape)
		{
			SetGeometryData();
		}
	}
}

void ComponentEngine::CapsuleCollider::SetHalfHeight(float halfHeight)
{
	if (halfHeight >= 0)
	{
		m_HalfHeight = halfHeight;
		if (m_Shape)
		{
			SetGeometryData();
		}
	}
}

void ComponentEngine::CapsuleCollider::SetDirection(eCapsuleDirection dir)
{
	m_Direction = dir;
	SetRotateByDirection();
}

void ComponentEngine::CapsuleCollider::SetGeometryData()
{
	m_Shape->setGeometry(physx::PxCapsuleGeometry(m_Radius, m_HalfHeight));
}

void ComponentEngine::CapsuleCollider::SetRotateByDirection()
{
	Vector3 _eulers = Vector3::Zero;

	switch (m_Direction)
	{
		case eCapsuleDirection::YAxis:
		{
			_eulers.z = 90;
		}
		break;
		case eCapsuleDirection::ZAxis:
		{
			_eulers.y = 90;
		}
		break;
		default:
			break;
	}
	m_Rotate = Quaternion::CreateFromYawPitchRoll(DegreeToRadian(_eulers.y), DegreeToRadian(_eulers.x), DegreeToRadian(_eulers.z));
}

void ComponentEngine::CapsuleCollider::SetCenter(Vector3 pos /*= Vector3::Zero*/)
{
	m_Center.x = pos.x;
	m_Center.y = pos.y;
	m_Center.z = pos.z;

	if (m_GameObject)
	{
		Transform* _transform = m_GameObject->GetComponent<Transform>();
		Quaternion _rot = _transform->GetQuaternion() * m_Rotate;

		SetShapeLocal(m_Center, _rot);
	}
}

void ComponentEngine::CapsuleCollider::Awake()
{
	Collider::Awake();

	// Shape 생성 후 등록
	m_Shape = m_PhysXEngine->CreatePxShape(physx::PxCapsuleGeometry(m_Radius, m_HalfHeight), m_Material->GetPxMaterial(), true);
	///SetGeometryData();
	SetRadius(m_Radius);
	SetHalfHeight(m_HalfHeight);
	SetCenter(m_Center);
	SetDirection(m_Direction);
	SetTrigger(m_IsTrigger);
	m_GameObject->AttatchShape(m_Shape);
}

void ComponentEngine::CapsuleCollider::Bind(GameObject* obj)
{

}

