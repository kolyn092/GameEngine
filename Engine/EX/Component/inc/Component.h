#pragma once

#include <string>
#include "Object.h"
#include "GameObject.h"

namespace ComponentEngine
{
	//////////////////////////////////////////////////////////////////////////
	//
	// 	주요 기능들을 컴포넌트화
	//  컴포넌트 클래스를 상속받는 컴포넌트들과 게임 오브젝트를 연결
	// 
	//////////////////////////////////////////////////////////////////////////

	class Component : public Object
	{
	public:
		Component();
		Component(std::string componentName);
		virtual ~Component();

	public:
		std::string m_ComponentName;

	public:
		template <typename T>
		T AddComponent(T);

		template <typename T>
		T* GetComponent();

		template <typename T>
		std::vector<T> GetComponents();

		template <typename T>
		bool HasComponent();

	public:
		GameObject* m_GameObject;

	public:
		virtual void Bind(GameObject* obj);

		std::string GetComponentName() const { return m_ComponentName; }

	};

	template <typename T>
	T* Component::GetComponent()
	{
		if (m_GameObject != nullptr)
		{
			return m_GameObject->template GetComponent<T>();
		}

		return nullptr;
	}

	template <typename T>
	T Component::AddComponent(T)
	{
		if (m_GameObject != nullptr)
		{
			return m_GameObject->template AddComponent<T>();
		}

		return nullptr;
	}

	template <typename T>
	std::vector<T> Component::GetComponents()
	{
		if (m_GameObject != nullptr)
		{
			return m_GameObject->template GetComponents<T>();
		}

		return nullptr;
	}

	template <typename T>
	bool Component::HasComponent()
	{
		if (m_GameObject != nullptr) 
		{
			return m_GameObject->template HasComponent<T>();
		}

		return false;
	}
}
