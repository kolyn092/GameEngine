#include "pch.h"
#include "PhysicMaterial.h"

ComponentEngine::PhysicMaterial::PhysicMaterial()
	: m_Physics(nullptr), m_Material(nullptr), m_DynamicFriction(0.6f), m_StaticFriction(0.6f), m_Bounciness(0.0f),
	m_FrictionCombine(ePhysicMaterialCombine::AVERAGE), m_BounceCombine(ePhysicMaterialCombine::AVERAGE)
{
	// get PxPhysics
	m_Physics = PhysXEngine::PhysX::Ins()->GetPxPhysics();

	// create material
	m_Material = m_Physics->createMaterial(m_StaticFriction, m_DynamicFriction, m_Bounciness);

	// set material combine mode
	m_Material->setFrictionCombineMode(static_cast<physx::PxCombineMode::Enum>(m_FrictionCombine));
	m_Material->setRestitutionCombineMode(static_cast<physx::PxCombineMode::Enum>(m_BounceCombine));
}
ComponentEngine::PhysicMaterial::PhysicMaterial(int index)
	: m_Physics(nullptr), m_Material(nullptr), m_DynamicFriction(0.6f), m_StaticFriction(0.6f), m_Bounciness(0.0f),
	m_FrictionCombine(ePhysicMaterialCombine::AVERAGE), m_BounceCombine(ePhysicMaterialCombine::AVERAGE)
{
	// get PxPhysics
	m_Physics = PhysXEngine::PhysX::Ins()->GetPxPhysics(index);

	// create material
	m_Material = m_Physics->createMaterial(m_StaticFriction, m_DynamicFriction, m_Bounciness);

	// set material combine mode
	m_Material->setFrictionCombineMode(static_cast<physx::PxCombineMode::Enum>(m_FrictionCombine));
	m_Material->setRestitutionCombineMode(static_cast<physx::PxCombineMode::Enum>(m_BounceCombine));
}

ComponentEngine::PhysicMaterial::PhysicMaterial(float dynamicFriction, float staticFriction, float bounciness, ePhysicMaterialCombine frictionCombine, ePhysicMaterialCombine bounceCombine, int index)
	: m_Physics(nullptr), m_Material(nullptr), m_DynamicFriction(dynamicFriction), m_StaticFriction(staticFriction), m_Bounciness(bounciness),
	m_FrictionCombine(frictionCombine), m_BounceCombine(bounceCombine)
{
	// get PxPhysics
	m_Physics = PhysXEngine::PhysX::Ins()->GetPxPhysics(index);

	// create material
	m_Material = m_Physics->createMaterial(m_StaticFriction, m_DynamicFriction, m_Bounciness);

	// set material combine mode
	m_Material->setFrictionCombineMode(static_cast<physx::PxCombineMode::Enum>(m_FrictionCombine));
	m_Material->setRestitutionCombineMode(static_cast<physx::PxCombineMode::Enum>(m_BounceCombine));
}


ComponentEngine::PhysicMaterial::PhysicMaterial(float dynamicFriction, float staticFriction, float bounciness,
	ePhysicMaterialCombine frictionCombine, ePhysicMaterialCombine bounceCombine)
	: m_Physics(nullptr), m_Material(nullptr), m_DynamicFriction(dynamicFriction), m_StaticFriction(staticFriction), m_Bounciness(bounciness),
	m_FrictionCombine(frictionCombine), m_BounceCombine(bounceCombine)
{
	// get PxPhysics
	m_Physics = PhysXEngine::PhysX::Ins()->GetPxPhysics();

	// create material
	m_Material = m_Physics->createMaterial(m_StaticFriction, m_DynamicFriction, m_Bounciness);

	// set material combine mode
	m_Material->setFrictionCombineMode(static_cast<physx::PxCombineMode::Enum>(m_FrictionCombine));
	m_Material->setRestitutionCombineMode(static_cast<physx::PxCombineMode::Enum>(m_BounceCombine));
}

ComponentEngine::PhysicMaterial::~PhysicMaterial()
{
	PX_RELEASE(m_Material);
}

void ComponentEngine::PhysicMaterial::SetDynamicFriction(float value)
{
	if (0 <= value && value <= 1)
	{
		m_DynamicFriction = value;
		m_Material->setDynamicFriction(m_DynamicFriction);
	}
}

void ComponentEngine::PhysicMaterial::SetStaticFriction(float value)
{
	if (0 <= value && value <= 1)
	{
		m_StaticFriction = value;
		m_Material->setStaticFriction(m_StaticFriction);
	}
}

void ComponentEngine::PhysicMaterial::SetBounciness(float value)
{
	if (0 <= value && value <= 1)
	{
		m_Bounciness = value;
		m_Material->setRestitution(m_Bounciness);
	}
}

void ComponentEngine::PhysicMaterial::SetFrictionCombine(ePhysicMaterialCombine combine)
{
	m_FrictionCombine = combine;
	m_Material->setFrictionCombineMode(static_cast<physx::PxCombineMode::Enum>(m_FrictionCombine));
}

void ComponentEngine::PhysicMaterial::SetBounceCombine(ePhysicMaterialCombine combine)
{
	m_BounceCombine = combine;
	m_Material->setRestitutionCombineMode(static_cast<physx::PxCombineMode::Enum>(m_BounceCombine));
}
