#pragma once
#include "PhysX.h"

namespace ComponentEngine
{
	class PhysicMaterial
	{
	public:
		enum class ePhysicMaterialCombine
		{
			AVERAGE = physx::PxCombineMode::eAVERAGE,		// ���
			MULTIPLY = physx::PxCombineMode::eMULTIPLY,		// ���� ��
			MINIMUM = physx::PxCombineMode::eMIN,			// ū ��
			MAXIMUM = physx::PxCombineMode::eMAX,			// ����
		};

	private:
		physx::PxPhysics* m_Physics;
		physx::PxMaterial* m_Material;

	private:
		float m_DynamicFriction;	// � ���� (0 : �̲����� ~ 1 : �߷¿� �и��� �ʴ� �� ���� ����)
		float m_StaticFriction;		// ���� ���� (0 : �̲����� ~ 1 : ������ ����)
		float m_Bounciness;			// ź�� (0 : ź��X ~ 1 : ������ �ս� ���� ź��O)
		ePhysicMaterialCombine m_FrictionCombine;	// �浹�ϴ� �� ������Ʈ�� ������ �������� ���
		ePhysicMaterialCombine m_BounceCombine;	// �浹�ϴ� �� ������Ʈ�� ź���� ��ġ�� ���
	
	public:
		PhysicMaterial();
		PhysicMaterial(float dynamicFriction, float staticFriction, float bounciness,
			ePhysicMaterialCombine frictionCombine, ePhysicMaterialCombine bounceCombine);
		PhysicMaterial(int index);
		PhysicMaterial(float dynamicFriction, float staticFriction, float bounciness,
			ePhysicMaterialCombine frictionCombine, ePhysicMaterialCombine bounceCombine, int index);
		~PhysicMaterial();

	public:
		void SetDynamicFriction(float value);
		void SetStaticFriction(float value);
		void SetBounciness(float value);
		void SetFrictionCombine(ePhysicMaterialCombine combine);
		void SetBounceCombine(ePhysicMaterialCombine combine);

	public:
		physx::PxMaterial* GetPxMaterial() const { return m_Material; }
	};
}

