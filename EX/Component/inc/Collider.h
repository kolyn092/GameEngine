#pragma once
#include "Component.h"
#include "PhysicMaterial.h"
#include <SimpleMath.h>

namespace ComponentEngine
{
	class Collider : public Component
	{

	protected:
		PhysXEngine::PhysX* m_PhysXEngine;
		physx::PxShape* m_Shape;

	protected:
		bool m_ActiveState;
		bool m_IsTrigger;
		float m_ContactOffset;
		PhysicMaterial* m_Material;

	public:
		Collider(std::string componentName);
		Collider(std::string componentName, PhysicMaterial* material);
		virtual ~Collider();

	public:
		virtual void SetGeometryData() abstract;
		virtual void SetCenter(DirectX::SimpleMath::Vector3 pos = DirectX::SimpleMath::Vector3::Zero) abstract;
		void SetTrigger(bool value);
		bool IsTrigger() const { return m_IsTrigger; }
		physx::PxShape* GetShape() const { return m_Shape; }

		std::string GetTag() const
		{
			if (m_GameObject)
			{
				return m_GameObject->GetTag();
			}
			else
			{
				return "";
			}
		}
		
	protected:
		void SetShapeLocal(DirectX::SimpleMath::Vector3 pos, DirectX::SimpleMath::Quaternion rot = DirectX::SimpleMath::Quaternion::Identity);
		void SetMaterial(PhysicMaterial* material);

	public:
		virtual void Awake() override;
		virtual void Start() override;

		virtual void PhysicsUpdate() override;

		virtual void DebugTextRender() override;

		virtual void OnQuit() override;
		virtual void OnDisable() override;
		virtual void OnDestroy() override;

		virtual void SetActive(bool value) override;
		virtual bool isActive() override { return m_ActiveState; }

	public:
		virtual void Bind(GameObject* obj) override;

	};
}

