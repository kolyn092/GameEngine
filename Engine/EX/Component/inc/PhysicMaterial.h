#pragma once
#include "PhysX.h"

namespace ComponentEngine
{
	class PhysicMaterial
	{
	public:
		enum class ePhysicMaterialCombine
		{
			AVERAGE = physx::PxCombineMode::eAVERAGE,		// 평균
			MULTIPLY = physx::PxCombineMode::eMULTIPLY,		// 작은 값
			MINIMUM = physx::PxCombineMode::eMIN,			// 큰 값
			MAXIMUM = physx::PxCombineMode::eMAX,			// 곱함
		};

	private:
		physx::PxPhysics* m_Physics;
		physx::PxMaterial* m_Material;

	private:
		float m_DynamicFriction;	// 운동 마찰 (0 : 미끄러움 ~ 1 : 중력에 밀리지 않는 한 빨리 정지)
		float m_StaticFriction;		// 정지 마찰 (0 : 미끄러움 ~ 1 : 움직임 힘듬)
		float m_Bounciness;			// 탄성 (0 : 탄성X ~ 1 : 에너지 손실 없이 탄성O)
		ePhysicMaterialCombine m_FrictionCombine;	// 충돌하는 두 오브젝트의 마찰이 합쳐지는 방법
		ePhysicMaterialCombine m_BounceCombine;	// 충돌하는 두 오브젝트의 탄성을 합치는 방법
	
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

