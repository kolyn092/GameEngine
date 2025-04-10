#pragma once
#include "Component.h"
#include <SimpleMath.h>

namespace ComponentEngine
{
	using namespace DirectX::SimpleMath;

	class Transform : public Component
	{

	private:
		Transform* m_Root;
		Transform* m_Parent;
		std::vector<Transform*> m_Childs;

	public:
		Vector3 m_Forward;
		Vector3 m_Backward;
		Vector3 m_Up;
		Vector3 m_Left;
		Vector3 m_Right;

	private:
		Vector3 m_Position;
		Quaternion m_Rotation;
		Vector3 m_Scale;

		Vector3 m_LocalPosition;
		Quaternion m_LocalRotation;
		Vector3 m_LocalScale;

		Vector3 m_EulerAngles;
		Vector3 m_LocalEulerAngles;

		Matrix m_LocalTM;
		Matrix m_WorldTM;

	public:
		Transform();
		virtual ~Transform();

	public:
		void SetPosition(float x, float y, float z);
		void SetPosition(Vector3 pos);

		void Translate(float x, float y, float z);
		void Translate(Vector3 translation);

		void SetRotate(Vector3 eulers);
		void SetRotate(float xAngle, float yAngle, float zAngle);

		void Rotate(Vector3 eulers);
		void Rotate(float xAngle, float yAngle, float zAngle);
		void Rotate(Vector3 axis, float angle);

		void SetScale(Vector3 scale);

		void SetLocalPosition(float x, float y, float z);
		void SetLocalPosition(Vector3 position);
		void SetLocalRotation(Vector3 rotation);
		void SetLocalScale(Vector3 scale);

		//////////////////////////////////////////////////////////////////////////

		Transform* GetChild(unsigned int index);
		Transform* GetParent() { return m_Parent; }
		int GetChildCount() const { return m_Childs.size(); }

		//////////////////////////////////////////////////////////////////////////

		void SetTransform(Vector3 pos, Quaternion rot);

		void AttatchChild(Transform* child);
		void DetachChild(Transform* child, Transform* newParent);
		void DetachChild(unsigned int index, Transform* newParent);
		void DetachChild(std::string name, Transform* newParent);

	private:
		void SetDirection();
		void MakeTransformMatrix();
		void SetParent(Transform* transform);
		void ColliderUpdate();

	public:
		Vector3 GetPosition() { return m_Position; }
		Vector3 GetRotation() { return m_EulerAngles; }
		Quaternion GetQuaternion() { return m_Rotation; }
		Vector3 GetScale() { return m_Scale; }

		Vector3 GetLocalPosition() { return m_LocalPosition; }
		Vector3 GetLocalRotation() { return m_LocalEulerAngles; }
		Quaternion GetLocalQuaternion() { return m_LocalRotation; }
		Vector3 GetLocalScale() { return m_LocalScale; }

		Matrix GetWorldTM() { return m_WorldTM; }
		Matrix GetLocalTM() { return m_LocalTM; }

	public:
		virtual void Awake() override;

		virtual void Update() override;

		virtual void DebugTextRender() override;

		virtual void OnQuit() override;
		virtual void OnDisable() override;
		virtual void OnDestroy() override;

		virtual bool isActive() override { return true; }

	public:
		virtual void Bind(GameObject* obj) override;
	};
}