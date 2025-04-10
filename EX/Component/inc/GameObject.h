#pragma once

#include <typeinfo>
#include <string>
#include <vector>

#include "PhysX.h"

namespace ComponentEngine
{
	class Component;
	class Scene;
	class Transform;
	class MeshRenderer;
	class Collider;
	class BoxCollider;
	class SphereCollider;
	class CapsuleCollider;
	class UI_Button;
	class UI_Image;
	class InputField;
	class ProgressBar;
	class CEAudioListener;

	class GameObject
	{

	private:
		std::string m_Name;
		std::string m_Tag;
		std::string m_LayerName;
		Scene* m_Scene;
		bool m_ActiveState;
		PhysXEngine::PhysX::ePairFlag m_TriggerFlag;
		PhysXEngine::PhysX::ePairFlag m_PrevTriggerFlag;

	private:
		// Trigger용
		Collider* m_TargetCollider;
		Collider* m_MyCollider;

		// 물리엔진 관련
		physx::PxRigidActor* m_PhysicsActor;

	private:
		// 가지고 있는 컴포넌트들
		std::vector<Component*> m_Components;
		// 자식 오브젝트
		std::vector<GameObject*> m_ChildObjects;

	public:
		Transform* m_Transform;
		MeshRenderer* m_MeshRenderer;
		Rigidbody* m_Rigidbody;
		CEAudioListener* m_AudioListener;
		UI_Image* m_Image;
		UI_Button* m_Button;
		ProgressBar* m_ProgressBar;
		InputField* m_InputField;

	public:
		GameObject(std::string name);
		GameObject();
		virtual ~GameObject();
	public:
		std::string GetName() const { return m_Name; }
		void SetName(std::string val) { m_Name = val; }

		std::string GetTag() const { return m_Tag; }
		void SetTag(std::string tagName) { m_Tag = tagName; }

		std::string GetLayerName() const { return m_LayerName; }
		void SetLayerName(std::string layerName) { m_LayerName = layerName; }

	public:
		void Awake();
		void Start();
		void OnEnable();
		void FixedUpdate(float dTime);
		void PhysicsUpdate(float dTime);
		void OnTriggerEnter(Collider* col);
		void OnTriggerStay(Collider* col);
		void OnTriggerExit(Collider* col);
		void OnCollisionEnter(Collider* col);
		void OnCollisionStay(Collider* col);
		void OnCollisionExit(Collider* col);
		void Update(float dTime);
		void LateUpdate(float dTime);
		void OnPreRender();
		void OnRender();
		void OnPostRender();
		void OnDrawGizmos();
		void OnGUI();
		void DebugTextRender();
		void OnQuit();
		void OnDisable();
		void OnDestroy();

	public:
		void SetActive(bool value);
		void SetActiveRecursively(bool state);
		bool isActive() const { return m_ActiveState; }

	public:
		template <typename T>
		T AddComponent(T);

		template <typename T>
		T* GetComponent();

		template <typename T>
		std::vector<T*> GetComponents();

		template <typename T>
		bool HasComponent();

		template <typename T>
		void RemoveComponent(T);

	public:
		Scene* GetScene() const { return m_Scene; }
		void SetScene(Scene* scene) { m_Scene = scene; }
		physx::PxRigidActor* GetActor() const { return m_PhysicsActor; }
		void SetActor(physx::PxRigidActor* actor) { m_PhysicsActor = actor; }
		void SetTriggerFlag(PhysXEngine::PhysX::ePairFlag flag) { m_TriggerFlag = flag; }
		void SetPrevTriggerFlag(PhysXEngine::PhysX::ePairFlag flag) { m_PrevTriggerFlag = flag; }
		PhysXEngine::PhysX::ePairFlag GetTriggerFlag() const { return m_TriggerFlag; }
		PhysXEngine::PhysX::ePairFlag GetPrevTriggerFlag() const { return m_PrevTriggerFlag; }
		void SetTargetTriggerCollider(Collider* col) { m_TargetCollider = col; }
		void SetMyTriggerCollider(Collider* col) { m_MyCollider = col; }
		size_t GetComponentSize() const { return m_Components.size(); }
		std::vector<Component*>& GetAllComponents() { return m_Components; }
		std::vector<GameObject*>& GetChildObjects() { return m_ChildObjects; }

	public:
		void AttatchShape(physx::PxShape* shape);
	};

	template <typename T>
	T GameObject::AddComponent(T component)
	{
		//// component가 transform 일 때
		//Transform t;
		//if (typeid(component) == typeid(&t))
		//{
		//	// 내가 가지고 있던 component들 중에 transform이 있는지 검사
		//	for (Component* co : m_Components)
		//	{
		//		if (typeid(*component) == typeid(*co))
		//		{
		//			// 이미 transform이 있으면 push_back 하지 않고 리턴
		//			return nullptr;
		//		}
		//	}
		//}
		component->Bind(this);
		Component* _component = dynamic_cast<Component*>(component);
		_component->m_GameObject = this;

		m_Components.emplace_back(_component);

		return component;
	}

	template <typename T>
	T* GameObject::GetComponent()
	{
		for (Component* co : m_Components)
		{
			//const type_info& _info = typeid(*co);
			//const type_info& _infoT = typeid(T);

			if (typeid(*co) == typeid(T))
			{
				return dynamic_cast<T*>(co);
			}
		}

		return nullptr;
	}

	template <typename T>
	std::vector<T*> GameObject::GetComponents()
	{
		std::vector<T*> _components;
		for (Component* co : m_Components)
		{
			const type_info& _info = typeid(*co);

			if (typeid(*co) == typeid(T))
			{
				_components.emplace_back(dynamic_cast<T*>(co));
			}
		}
		return _components;
	}

	template <typename T>
	bool GameObject::HasComponent()
	{
		for (Component* co : m_Components)
		{
			if (dynamic_cast<T*>(co))
				return true;
		}
		return false;
	}

	template <typename T>
	void GameObject::RemoveComponent(T)
	{
		for (auto it = m_Components.begin(); it != m_Components.end(); ++it)
		{
			if (typeid(*it) == typeid(T))
			{
				m_Components.erase(it);
				return;
			}
		}
	}

	///--------------------------------------------------
	/// 연산자 오버로딩
	static bool operator==(const GameObject& lhs, const GameObject& rhs)
	{
		if (lhs.GetName() == rhs.GetName())
		{
			return true;
		}

		return false;
	}

	static bool operator!=(const GameObject& lhs, const GameObject& rhs)
	{
		if (lhs.GetName() == rhs.GetName())
		{
			return false;
		}

		return true;
	}
}