#pragma once
#include "Component.h"
#include "Axis.h"

namespace ComponentEngine
{
	class CAxis : public Component
	{

	private:
		Axis* m_Axis;
		bool m_ActiveState;

	public:
		CAxis();
		virtual ~CAxis();

	public:
		virtual void Awake() override {};
		virtual void OnEnable() override;
		virtual void Start() override {};

		virtual void FixedUpdate() override {};
		virtual void PhysicsUpdate() override {};
		virtual void OnTriggerEnter(Collider* col) override {};
		virtual void OnTriggerStay(Collider* col) override {};
		virtual void OnTriggerExit(Collider* col) override {};
		virtual void OnCollisionEnter(Collider* col) override {};
		virtual void OnCollisionStay(Collider* col) override {};
		virtual void OnCollisionExit(Collider* col) override {};

		virtual void Update() override;
		virtual void Update(float dTime) override;
		virtual void LateUpdate(float dTime) override;

		virtual void OnPreRender() override;
		virtual void OnRender() override;
		virtual void OnPostRender() override;

		virtual void OnDrawGizmos() override;
		virtual void OnGUI() override {};
		virtual void DebugTextRender() override;

		virtual void OnQuit() override;
		virtual void OnDisable() override;
		virtual void OnDestroy() override;

		virtual void SetActive(bool value) override;
		virtual bool isActive() override { return m_ActiveState; }

	};
}
