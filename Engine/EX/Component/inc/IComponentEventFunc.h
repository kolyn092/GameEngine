#pragma once

namespace ComponentEngine
{
	class Collider;

	/// Object의 이벤트 함수 인터페이스
	__interface IComponentEventFunc
	{
		// Initialization
		virtual void Awake() abstract;
		virtual void OnEnable() abstract;
		virtual void Start() abstract;

		// physics
		virtual void FixedUpdate() abstract;
		virtual void PhysicsUpdate() abstract;
		virtual void OnTriggerEnter(Collider* col) abstract;
		virtual void OnTriggerStay(Collider* col) abstract;
		virtual void OnTriggerExit(Collider* col) abstract;
		virtual void OnCollisionEnter(Collider* col) abstract;
		virtual void OnCollisionStay(Collider* col) abstract;
		virtual void OnCollisionExit(Collider* col) abstract;

		// input

		// game logic
		virtual void Update() abstract;
		virtual void Update(float dTime) abstract;
		virtual void LateUpdate(float dTime) abstract;

		// rendering
		virtual void OnPreRender() abstract;
		virtual void OnRender() abstract;
		virtual void OnPostRender() abstract;

		// gizmo rendering
		virtual void OnDrawGizmos() abstract;

		// gui rendering
		virtual void OnGUI() abstract;
		virtual void DebugTextRender() abstract;

		// exit
		virtual void OnQuit() abstract;
		virtual void OnDisable() abstract;
		virtual void OnDestroy() abstract;

		virtual void SetActive(bool value) abstract;
		virtual bool isActive() abstract;
	};

}