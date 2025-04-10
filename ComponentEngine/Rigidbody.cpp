#include "pch.h"
#include "Rigidbody.h"
#include "PhysX.h"
#include "Transform.h"
#include "CDebug.h"

ComponentEngine::Rigidbody::Rigidbody()
	: Component("Rigidbody"), m_PhysXEngine(PhysXEngine::PhysX::Ins()), m_Actor(nullptr),
	m_Mass(1.0f), m_Drag(0.0f), m_AngularDrag(0.05f), m_UseGravity(true),
	m_IsKinematic(false), m_Interpolate(eInterpolate::NONE), m_CollisionDetection(eCollisionDetection::DISCRETE),
	m_FreezePosition(FreezeTransform{ false, false, false }),
	m_FreezeRotation(FreezeTransform{ false, false, false }),
	m_ForceMode(eForceMode::FORCE)
{

}

ComponentEngine::Rigidbody::~Rigidbody()
{

}

void ComponentEngine::Rigidbody::SetMass(float mass)
{
	m_Mass = mass;
	if ((0 <= mass && mass <= PX_MAX_F32) && m_Actor)
	{
		m_Actor->setMass(m_Mass);
	}
}

void ComponentEngine::Rigidbody::SetDrag(float drag)
{
	m_Drag = drag;
	if ((0 <= drag && drag <= PX_MAX_F32) && m_Actor)
	{
		m_Actor->setLinearDamping(m_Drag);
	}
}

void ComponentEngine::Rigidbody::SetAngularDrag(float angularDrag)
{
	m_AngularDrag = angularDrag;
	if ((0 <= angularDrag && angularDrag <= PX_MAX_F32) && m_Actor)
	{
		m_Actor->setAngularDamping(m_AngularDrag);
	}
}

void ComponentEngine::Rigidbody::SetUseGravity(bool value)
{
	m_UseGravity = value;
	if (m_Actor)
	{
		// TODO : 검증 필요
		m_Actor->setActorFlag(physx::PxActorFlag::eDISABLE_GRAVITY, !m_UseGravity);
	}
}

void ComponentEngine::Rigidbody::SetKinematic(bool value)
{
	m_IsKinematic = value;
	if (m_Actor)
	{
		// TODO : 검증 필요
		m_Actor->setRigidBodyFlag(physx::PxRigidBodyFlag::eKINEMATIC, m_IsKinematic);
	}
}

void ComponentEngine::Rigidbody::SetFreezePosition(FreezeTransform freezeState)
{
	m_FreezePosition = freezeState;
	if (m_Actor)
	{
		m_Actor->setRigidDynamicLockFlag(physx::PxRigidDynamicLockFlag::eLOCK_LINEAR_X, m_FreezePosition.x);
		m_Actor->setRigidDynamicLockFlag(physx::PxRigidDynamicLockFlag::eLOCK_LINEAR_Y, m_FreezePosition.y);
		m_Actor->setRigidDynamicLockFlag(physx::PxRigidDynamicLockFlag::eLOCK_LINEAR_Z, m_FreezePosition.z);
	}
}

void ComponentEngine::Rigidbody::SetFreezeRotation(FreezeTransform freezeState)
{
	m_FreezeRotation = freezeState;
	if (m_Actor)
	{
		m_Actor->setRigidDynamicLockFlag(physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_X, m_FreezeRotation.x);
		m_Actor->setRigidDynamicLockFlag(physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_Y, m_FreezeRotation.y);
		m_Actor->setRigidDynamicLockFlag(physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_Z, m_FreezeRotation.z);
	}
}

void ComponentEngine::Rigidbody::ClearForce()
{
	if (m_Actor)
	{
		m_Actor->setLinearVelocity(physx::PxVec3(0, 0, 0));
		m_Actor->clearForce(static_cast<physx::PxForceMode::Enum>(m_ForceMode));
	}
}

void ComponentEngine::Rigidbody::ClearTorque()
{
	if (m_Actor)
	{
		m_Actor->setAngularVelocity(physx::PxVec3(0, 0, 0));
		m_Actor->clearTorque(static_cast<physx::PxForceMode::Enum>(m_ForceMode));
	}
}

void ComponentEngine::Rigidbody::AddForce(Vector3 force, eForceMode mode/* = eForceMode::FORCE*/)
{
	if (m_Actor)
	{
		m_ForceMode = mode;

		physx::PxVec3 _forceVec;
		_forceVec.x = force.x;
		_forceVec.y = force.y;
		_forceVec.z = force.z;

		m_Actor->addForce(_forceVec, static_cast<physx::PxForceMode::Enum>(m_ForceMode));
	}
}

void ComponentEngine::Rigidbody::AddForce(float x, float y, float z, eForceMode mode/* = eForceMode::FORCE*/)
{
	if (m_Actor)
	{
		m_ForceMode = mode;

		m_Actor->addForce(physx::PxVec3(x, y, z), static_cast<physx::PxForceMode::Enum>(m_ForceMode));
	}
}


void ComponentEngine::Rigidbody::AddTorque(Vector3 force, eForceMode mode /*= eForceMode::FORCE*/)
{
	if (m_Actor)
	{
		m_ForceMode = mode;

		physx::PxVec3 _forceVec;
		_forceVec.x = force.x;
		_forceVec.y = force.y;
		_forceVec.z = force.z;

		m_Actor->addTorque(_forceVec, static_cast<physx::PxForceMode::Enum>(m_ForceMode));
	}
}

void ComponentEngine::Rigidbody::Sleep()
{
	if (m_Actor)
	{
		m_Actor->putToSleep();
	}
}

void ComponentEngine::Rigidbody::WakeUp()
{
	if (m_Actor)
	{
		m_Actor->wakeUp();
	}
}

bool ComponentEngine::Rigidbody::IsSleeping()
{
	if (m_Actor)
	{
		return m_Actor->isSleeping();
	}
	return false;
}

void ComponentEngine::Rigidbody::SetMaxAngularVelocity(float a)
{
	if (m_Actor)
	{
		// default value = 100.0
		m_Actor->setMaxAngularVelocity(a);
	}
}

void ComponentEngine::Rigidbody::AddTorque(float x, float y, float z, eForceMode mode /*= eForceMode::FORCE*/)
{
	if (m_Actor)
	{
		m_ForceMode = mode;

		m_Actor->addTorque(physx::PxVec3(x, y, z), static_cast<physx::PxForceMode::Enum>(m_ForceMode));
	}
}

void ComponentEngine::Rigidbody::Awake()
{
	Transform* _transform = m_GameObject->GetComponent<Transform>();
	///physx::PxTransform _pxTrans(m_PhysXEngine->MakePxTransformPos(_transform->GetPosition()));
	physx::PxTransform _pxTrans(m_PhysXEngine->MakePxTransform(_transform->GetPosition(), _transform->GetQuaternion()));
	m_Actor = m_PhysXEngine->CreateRigidDynamic(_pxTrans);
	SetMass(m_Mass);
	SetDrag(m_Drag);
	SetAngularDrag(m_AngularDrag);
	SetUseGravity(m_UseGravity);
	SetKinematic(m_IsKinematic);
	SetFreezePosition(m_FreezePosition);
	SetFreezeRotation(m_FreezeRotation);
	m_GameObject->SetActor(m_Actor);
}

void ComponentEngine::Rigidbody::DebugTextRender()
{
	CDebug::Ins()->DebugDrawText(L"%s %s", L"L", L"Rigidbody");
}

void ComponentEngine::Rigidbody::OnQuit()
{

}

void ComponentEngine::Rigidbody::OnDisable()
{

}

void ComponentEngine::Rigidbody::OnDestroy()
{

}

bool ComponentEngine::Rigidbody::isActive()
{
	// 임시
	return true;
}

void ComponentEngine::Rigidbody::Bind(GameObject* obj)
{
	if (obj)
	{
		obj->m_Rigidbody = this;
	}
}
