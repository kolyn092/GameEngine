#pragma once

#include <string>
#include "Object.h"
#include "GameObject.h"
#include "TypeIDGenerator.h"

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
		virtual size_t GetTypeID() const = 0;

	public:
		std::string m_ComponentName;
		int m_TypeLocalIndex = -1;	// 같은 타입의 컴포넌트끼리 구분하기 위한 인덱스

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

	template<typename T>
	class ComponentImpl : public Component
	{
	public:
		ComponentImpl(std::string componentName);
		virtual ~ComponentImpl();

	public:
		static size_t StaticTypeID()
		{
			return TypeIDGenerator::GetTypeID<T>();
		}

		size_t GetTypeID() const override
		{
			return StaticTypeID();
		}
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

	template<typename T>
	ComponentImpl<T>::ComponentImpl(std::string componentName)
		: Component()
	{
		m_ComponentName = componentName;
	}

	template<typename T>
	ComponentImpl<T>::~ComponentImpl()
	{

	}
}
