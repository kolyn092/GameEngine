#pragma once

#include "IComponentEventFunc.h"

namespace ComponentEngine
{
	class Object : public IComponentEventFunc
	{
	public:
		Object();
		virtual ~Object();

	public:
		
		/// IComponentEventFunc Override
	public:
		// Initialization
		virtual void Awake() override {}
		virtual void OnEnable() override {}
		virtual void Start() override {}

		// physics
		virtual void FixedUpdate() override {}
		virtual void PhysicsUpdate() override {}
		virtual void OnTriggerEnter(Collider* col) override {}
		virtual void OnTriggerStay(Collider* col) override {}
		virtual void OnTriggerExit(Collider* col) override {}
		virtual void OnCollisionEnter(Collider* col) override {}
		virtual void OnCollisionStay(Collider* col) override {}
		virtual void OnCollisionExit(Collider* col) override {}

		// input

		// game logic
		virtual void Update() override {}
		virtual void Update(float dTime) override {}
		virtual void LateUpdate(float dTime) override {}

		// rendering
		virtual void OnPreRender() override {}
		virtual void OnRender() override {}
		virtual void OnPostRender() override {}

		// gizmo rendering
		virtual void OnDrawGizmos() override {}

		// gui rendering
		virtual void OnGUI() override {}
		virtual void DebugTextRender() override {}

		// exit
		virtual void OnQuit() override {}
		virtual void OnDisable() override {}
		virtual void OnDestroy() override {}

		virtual void SetActive(bool value) override {}
		virtual bool isActive() override { return true; }
	};

}