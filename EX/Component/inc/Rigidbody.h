#pragma once
#include "Component.h"

namespace ComponentEngine
{
	class Rigidbody : public Component
	{

	public:
		enum class eInterpolate
		{
			NONE,					// 보간이 적용되지 않음
			INTERPOLATE,			// 이전 프레임의 Transform에 맞게 움직임을 부드럽게 처리
			EXTRAPOLATE,			// 다음 프레임의 Transform을 추정해 움직임을 부드럽게 처리
		};

		enum class eCollisionDetection
		{
			DISCRETE,				// 씬에서 모든 콜라이더에 대해 불연속 충돌 검사를 사용 (기본값)
			CONTINUOUS,				// 동적 콜라이더에 불연속 충돌 검사, 정적 콜라이더에 스위핑 기반 연속 충돌 검사
			CONTINUOUS_DYNAMIC,		// 리지드바디끼리 충돌하는 오브젝트만 불연속 충돌 검사
			CONTINUOUS_SPECULATIVE,	// 리지드바디와 콜라이더에 추측성 연속 충돌 검사 (CCD). 키네틱 바디 설정할 수 있는 유일한 CCD 모드
		};

		enum class eForceMode
		{
			FORCE = physx::PxForceMode::eFORCE,						// 연속적인 힘을 가함 + 질량 적용
			IMPULSE = physx::PxForceMode::eIMPULSE,					// 순간적인 힘을 가함 + 질량 적용
			VELOCITYCHANGE = physx::PxForceMode::eVELOCITY_CHANGE,	// 연속적인 힘을 가함 + 질량 무시
			ACCELERATION = physx::PxForceMode::eACCELERATION,		// 순간적인 힘을 가함 + 질량 무시
		};

		struct FreezeTransform
		{
			bool x;
			bool y;
			bool z;
		};

	private:
		PhysXEngine::PhysX* m_PhysXEngine;
		physx::PxRigidDynamic* m_Actor;

	private:
		float m_Mass;								// 질량
		float m_Drag;								// 힘에 의해 움직일 때 공기 저항 영향 정도
		float m_AngularDrag;						// 토크로 회전할 때 공기 저항 영향 정도
		bool m_UseGravity;							// 중력 영향 
		bool m_IsKinematic;							// 활성화 - 물리엔진으로 제어X, 오로지 Transform으로 조작
		eInterpolate m_Interpolate;
		eCollisionDetection m_CollisionDetection;
		FreezeTransform m_FreezePosition;
		FreezeTransform m_FreezeRotation;
		eForceMode m_ForceMode;

	public:
		Rigidbody();
		virtual ~Rigidbody();

	public:
		void SetMass(float mass);
		void SetDrag(float drag);
		void SetAngularDrag(float angularDrag);
		void SetUseGravity(bool value);
		void SetKinematic(bool value);
		void SetFreezePosition(FreezeTransform freezePositionState);
		void SetFreezeRotation(FreezeTransform freezeRotationState);

		FreezeTransform GetFreezeTransform() const { return m_FreezePosition; };

	public:
		void ClearForce();
		void ClearTorque();
		void AddForce(DirectX::SimpleMath::Vector3 force, eForceMode mode = eForceMode::FORCE);
		void AddForce(float x, float y, float z, eForceMode mode = eForceMode::FORCE);
		void AddTorque(DirectX::SimpleMath::Vector3 force, eForceMode mode = eForceMode::FORCE);
		void AddTorque(float x, float y, float z, eForceMode mode = eForceMode::FORCE);
		void Sleep();
		void WakeUp();
		bool IsSleeping();
		void SetMaxAngularVelocity(float a);

	public:
		virtual void Awake() override;

		virtual void DebugTextRender() override;

		virtual void OnQuit() override;
		virtual void OnDisable() override;
		virtual void OnDestroy() override;

		virtual bool isActive() override;

	public:
		virtual void Bind(GameObject* obj) override;
	};
}