#pragma once
#include "Component.h"

namespace ComponentEngine
{
	class Rigidbody : public Component
	{

	public:
		enum class eInterpolate
		{
			NONE,					// ������ ������� ����
			INTERPOLATE,			// ���� �������� Transform�� �°� �������� �ε巴�� ó��
			EXTRAPOLATE,			// ���� �������� Transform�� ������ �������� �ε巴�� ó��
		};

		enum class eCollisionDetection
		{
			DISCRETE,				// ������ ��� �ݶ��̴��� ���� �ҿ��� �浹 �˻縦 ��� (�⺻��)
			CONTINUOUS,				// ���� �ݶ��̴��� �ҿ��� �浹 �˻�, ���� �ݶ��̴��� ������ ��� ���� �浹 �˻�
			CONTINUOUS_DYNAMIC,		// ������ٵ𳢸� �浹�ϴ� ������Ʈ�� �ҿ��� �浹 �˻�
			CONTINUOUS_SPECULATIVE,	// ������ٵ�� �ݶ��̴��� ������ ���� �浹 �˻� (CCD). Ű��ƽ �ٵ� ������ �� �ִ� ������ CCD ���
		};

		enum class eForceMode
		{
			FORCE = physx::PxForceMode::eFORCE,						// �������� ���� ���� + ���� ����
			IMPULSE = physx::PxForceMode::eIMPULSE,					// �������� ���� ���� + ���� ����
			VELOCITYCHANGE = physx::PxForceMode::eVELOCITY_CHANGE,	// �������� ���� ���� + ���� ����
			ACCELERATION = physx::PxForceMode::eACCELERATION,		// �������� ���� ���� + ���� ����
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
		float m_Mass;								// ����
		float m_Drag;								// ���� ���� ������ �� ���� ���� ���� ����
		float m_AngularDrag;						// ��ũ�� ȸ���� �� ���� ���� ���� ����
		bool m_UseGravity;							// �߷� ���� 
		bool m_IsKinematic;							// Ȱ��ȭ - ������������ ����X, ������ Transform���� ����
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