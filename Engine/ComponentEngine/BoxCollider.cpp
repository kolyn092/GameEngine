#include "pch.h"
#include "BoxCollider.h"

//////////////////////////////////////////////////////////////////////////
//
// Box Collider
//
//////////////////////////////////////////////////////////////////////////

ComponentEngine::BoxCollider::BoxCollider()
	: Collider("BoxCollider"), m_Center(Vector3::Zero), m_Size(Vector3(1, 1, 1))
{

}

ComponentEngine::BoxCollider::~BoxCollider()
{

}

void ComponentEngine::BoxCollider::SetCenter(Vector3 pos /*= Vector3::Zero*/)
{
	m_Center.x = pos.x;
	m_Center.y = pos.y;
	m_Center.z = pos.z;

	if (m_GameObject)
	{
		SetShapeLocal(m_Center);
	}
}

void ComponentEngine::BoxCollider::SetScale(Vector3 scale)
{
	if (scale.x > 0 && scale.y > 0 && scale.z > 0)
	{
		m_Size = scale;
		SetGeometryData();
	}
}

void ComponentEngine::BoxCollider::SetScaleX(float x)
{
	if (x > 0)
	{
		m_Size.x = x;
		SetGeometryData();
	}
}

void ComponentEngine::BoxCollider::SetScaleY(float y)
{
	if (y > 0)
	{
		m_Size.y = y;
		SetGeometryData();
	}
}

void ComponentEngine::BoxCollider::SetScaleZ(float z)
{
	if (z > 0)
	{
		m_Size.z = z;
		SetGeometryData();
	}
}

void ComponentEngine::BoxCollider::SetGeometryData()
{
	if (m_Shape)
	{
		m_Shape->setGeometry(physx::PxBoxGeometry(m_Size.x / 2, m_Size.y / 2, m_Size.z / 2));
	}
}

void ComponentEngine::BoxCollider::Awake()
{
	Collider::Awake();

	// Shape 생성 후 등록
	SetScale(m_Size);
	m_Shape = m_PhysXEngine->CreatePxShape(physx::PxBoxGeometry(m_Size.x / 2, m_Size.y / 2, m_Size.z / 2), m_Material->GetPxMaterial(), true);
	SetGeometryData();
	SetCenter(m_Center);
	SetTrigger(m_IsTrigger);
	m_GameObject->AttatchShape(m_Shape);
}

void ComponentEngine::BoxCollider::Bind(GameObject* obj)
{

}
