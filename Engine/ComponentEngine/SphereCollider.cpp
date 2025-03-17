#include "pch.h"
#include "SphereCollider.h"

//////////////////////////////////////////////////////////////////////////
//
// Sphere Collider
//
//////////////////////////////////////////////////////////////////////////

ComponentEngine::SphereCollider::SphereCollider()
	: Collider("SphereCollider"), m_Center(Vector3::Zero), m_Radius(10.0f)
{

}

ComponentEngine::SphereCollider::~SphereCollider()
{

}

void ComponentEngine::SphereCollider::SetRadius(float radius)
{
	if (radius > 0)
	{
		m_Radius = radius;
		SetGeometryData();
	}
}

void ComponentEngine::SphereCollider::SetGeometryData()
{
	if (m_Shape)
	{
		m_Shape->setGeometry(physx::PxSphereGeometry(m_Radius));
	}
}

void ComponentEngine::SphereCollider::SetCenter(Vector3 pos /*= Vector3::Zero*/)
{
	m_Center.x = pos.x;
	m_Center.y = pos.y;
	m_Center.z = pos.z;

	if (m_GameObject)
	{
		SetShapeLocal(m_Center);
	}
}

void ComponentEngine::SphereCollider::Awake()
{
	Collider::Awake();

	// Shape 생성 후 등록
	m_Shape = m_PhysXEngine->CreatePxShape(physx::PxSphereGeometry(m_Radius), m_Material->GetPxMaterial(), true);
	SetGeometryData();
	SetTrigger(m_IsTrigger);
	SetCenter(m_Center);
	m_GameObject->AttatchShape(m_Shape);
}

void ComponentEngine::SphereCollider::Bind(GameObject* obj)
{

}

