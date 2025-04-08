#pragma once
#include "Component.h"

namespace ComponentEngine
{
	class MeshFilter : public Component
	{

	private:
		bool m_ActiveState;

	public:


	public:
		MeshFilter();
		virtual ~MeshFilter();

	public:
		virtual void Awake() override {};
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

		virtual void OnPreRender() override;
		virtual void OnRender() override;
		virtual void OnPostRender() override;

		virtual void SetActive(bool value) override;
	};
}

