#include "pch.h"
#include "Component.h"
#include <typeinfo>

ComponentEngine::Component::Component()
	: Object(), m_ComponentName(""), m_GameObject(nullptr)
{

}

ComponentEngine::Component::Component(std::string componentName)
	: Object(), m_ComponentName(componentName), m_GameObject(nullptr)
{

}

ComponentEngine::Component::~Component()
{

}

void ComponentEngine::Component::Bind(GameObject* obj)
{

}
