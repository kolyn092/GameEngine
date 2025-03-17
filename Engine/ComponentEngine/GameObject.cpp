#include "pch.h"
#include "GameObject.h"
#include "Component.h"	// 템플릿 상호참조 주의
#include "Transform.h"
#include "Collider.h"
#include "MeshRenderer.h"
#include "Image.h"

using namespace ComponentEngine;

ComponentEngine::GameObject::GameObject(std::string name)
	: m_Name(name), m_Tag(""), m_LayerName("Default"), m_Scene(nullptr), m_ActiveState(true),
	m_TriggerFlag(PhysXEngine::PhysX::ePairFlag::None), m_PrevTriggerFlag(PhysXEngine::PhysX::ePairFlag::None),
	m_TargetCollider(nullptr), m_MyCollider(nullptr), m_PhysicsActor(nullptr),
	m_Transform(nullptr), m_MeshRenderer(nullptr), m_Rigidbody(nullptr), m_AudioListener(nullptr),
	m_Image(nullptr), m_Button(nullptr), m_ProgressBar(nullptr), m_InputField(nullptr)
{
	Transform* _transform = new Transform();
	this->AddComponent<Transform*>(_transform);
	m_Transform = _transform;
}

ComponentEngine::GameObject::GameObject()
	: m_Name("GameObject"), m_Tag(""), m_LayerName("Default"), m_Scene(nullptr), m_ActiveState(true),
	m_TriggerFlag(PhysXEngine::PhysX::ePairFlag::None), m_PrevTriggerFlag(PhysXEngine::PhysX::ePairFlag::None),
	m_TargetCollider(nullptr), m_MyCollider(nullptr), m_PhysicsActor(nullptr),
	m_Transform(nullptr), m_MeshRenderer(nullptr), m_Rigidbody(nullptr), m_AudioListener(nullptr),
	m_Image(nullptr), m_Button(nullptr), m_ProgressBar(nullptr), m_InputField(nullptr)
{
	Transform* _transform = new Transform();
	this->AddComponent<Transform*>(_transform);
	m_Transform = _transform;
}

ComponentEngine::GameObject::~GameObject()
{
	for (Component* com : m_Components)
	{
		if (com)
		{
			com->m_GameObject = nullptr;
			delete com;
			com = nullptr;
		}
	}

	if (m_PhysicsActor)
	{
		m_PhysicsActor->release();
		m_PhysicsActor = nullptr;
	}
}

void ComponentEngine::GameObject::Awake()
{
	for (Component* co : m_Components)
	{
		co->Awake();
	}

	if (m_PhysicsActor)
	{
		PhysXEngine::PhysX::Ins()->AddPhysXActor(*(m_PhysicsActor));
	}

	if (m_Transform == nullptr)
		return;

	// 자식 GameObject Awake 호출
	if (m_Transform->GetChildCount() > 0)
	{
		for (int i = 0; i < m_Transform->GetChildCount(); i++)
		{
			m_Transform->GetChild(i)->m_GameObject->Awake();
		}
	}
}

void ComponentEngine::GameObject::Start()
{
	if (m_ActiveState)
	{
		for (Component* co : m_Components)
		{
			co->Start();
		}

		if (m_Transform == nullptr)
			return;

		// 자식 GameObject Start 호출
		if (m_Transform->GetChildCount() > 0)
		{
			for (int i = 0; i < m_Transform->GetChildCount(); i++)
			{
				m_Transform->GetChild(i)->m_GameObject->Start();
			}
		}
	}
}

void ComponentEngine::GameObject::OnEnable()
{
	if (m_ActiveState)
	{
		for (Component* co : m_Components)
		{
			co->OnEnable();
		}

		if (m_Transform == nullptr)
			return;

		// 자식 GameObject OnEnable 호출
		if (m_Transform->GetChildCount() > 0)
		{
			for (int i = 0; i < m_Transform->GetChildCount(); i++)
			{
				m_Transform->GetChild(i)->m_GameObject->OnEnable();
			}
		}
	}
}

void ComponentEngine::GameObject::FixedUpdate(float dTime)
{
	if (m_ActiveState)
	{
		for (Component* co : m_Components)
		{
			co->FixedUpdate();
		}

		if (m_Transform == nullptr)
			return;

		// 자식 GameObject FixedUpdate 호출
		if (m_Transform->GetChildCount() > 0)
		{
			for (int i = 0; i < m_Transform->GetChildCount(); i++)
			{
				m_Transform->GetChild(i)->m_GameObject->FixedUpdate(dTime);
			}
		}
	}
}

void ComponentEngine::GameObject::PhysicsUpdate(float dTime)
{
	if (m_ActiveState)
	{
		for (Component* co : m_Components)
		{
			co->PhysicsUpdate();
		}
	}

	if (m_TriggerFlag == PhysXEngine::PhysX::ePairFlag::Stay)
	{
		if (m_MyCollider->isActive() && m_TargetCollider->isActive())
		{
			OnTriggerStay(m_TargetCollider);
		}
	}

	if (m_Transform == nullptr)
		return;

	// 자식 GameObject PhysicsUpdate 호출
	if (m_Transform->GetChildCount() > 0)
	{
		for (int i = 0; i < m_Transform->GetChildCount(); i++)
		{
			m_Transform->GetChild(i)->m_GameObject->PhysicsUpdate(dTime);
		}
	}
}

void ComponentEngine::GameObject::OnTriggerEnter(Collider* col)
{
	if (m_ActiveState)
	{
		for (Component* co : m_Components)
		{
			co->OnTriggerEnter(col);
		}

		if (m_Transform == nullptr)
			return;

		// 자식 GameObject OnTriggerEnter 호출
		if (m_Transform->GetChildCount() > 0)
		{
			for (int i = 0; i < m_Transform->GetChildCount(); i++)
			{
				m_Transform->GetChild(i)->m_GameObject->OnTriggerEnter(col);
			}
		}
	}
}

void ComponentEngine::GameObject::OnTriggerStay(Collider* col)
{
	if (m_ActiveState)
	{
		for (Component* co : m_Components)
		{
			co->OnTriggerStay(col);
		}

		if (m_Transform == nullptr)
			return;

		// 자식 GameObject OnTriggerStay 호출
		if (m_Transform->GetChildCount() > 0)
		{
			for (int i = 0; i < m_Transform->GetChildCount(); i++)
			{
				m_Transform->GetChild(i)->m_GameObject->OnTriggerStay(col);
			}
		}
	}
}

void ComponentEngine::GameObject::OnTriggerExit(Collider* col)
{
	for (Component* co : m_Components)
	{
		co->OnTriggerExit(col);
	}

	if (m_Transform == nullptr)
		return;

	// 자식 호출
	if (m_Transform->GetChildCount() > 0)
	{
		for (int i = 0; i < m_Transform->GetChildCount(); i++)
		{
			m_Transform->GetChild(i)->m_GameObject->OnTriggerExit(col);
		}
	}
}

void ComponentEngine::GameObject::OnCollisionEnter(Collider* col)
{
	if (m_ActiveState)
	{
		for (Component* co : m_Components)
		{
			if (co->isActive())
			{
				co->OnCollisionEnter(col);
			}
		}

		if (m_Transform == nullptr)
			return;

		// 자식 호출
		if (m_Transform->GetChildCount() > 0)
		{
			for (int i = 0; i < m_Transform->GetChildCount(); i++)
			{
				m_Transform->GetChild(i)->m_GameObject->OnCollisionEnter(col);
			}
		}
	}
}

void ComponentEngine::GameObject::OnCollisionStay(Collider* col)
{
	if (m_ActiveState)
	{
		for (Component* co : m_Components)
		{
			co->OnCollisionStay(col);
		}

		if (m_Transform == nullptr)
			return;

		// 자식 호출
		if (m_Transform->GetChildCount() > 0)
		{
			for (int i = 0; i < m_Transform->GetChildCount(); i++)
			{
				m_Transform->GetChild(i)->m_GameObject->OnCollisionStay(col);
			}
		}
	}
}

void ComponentEngine::GameObject::OnCollisionExit(Collider* col)
{
	if (m_ActiveState)
	{
		for (Component* co : m_Components)
		{
			co->OnCollisionExit(col);
		}

		if (m_Transform == nullptr)
			return;

		// 자식 호출
		if (m_Transform->GetChildCount() > 0)
		{
			for (int i = 0; i < m_Transform->GetChildCount(); i++)
			{
				m_Transform->GetChild(i)->m_GameObject->OnCollisionExit(col);
			}
		}
	}
}

void ComponentEngine::GameObject::Update(float dTime)
{
	if (m_ActiveState == true)
	{
		for (Component* co : m_Components)
		{
			co->Update();
		}

		for (Component* co : m_Components)
		{
			co->Update(dTime);
		}
	}

	if (m_MeshRenderer)
	{
		m_MeshRenderer->SetActive(m_ActiveState);
	}

	if (m_Image)
	{
		m_Image->SetActive(m_ActiveState);
	}

	// 자식 호출
	if (m_Transform)
	{
		if (m_Transform->GetChildCount() > 0)
		{
			for (int i = 0; i < m_Transform->GetChildCount(); i++)
			{
				auto child = m_Transform->GetChild(i);

				if (child)
				{
					MeshRenderer* _mesh = child->m_GameObject->m_MeshRenderer;
					if (_mesh)
					{
						_mesh->SetActive(m_ActiveState);
					}

					UI_Image* _image = child->m_GameObject->m_Image;
					if (_image)
					{
						_image->SetActive(m_ActiveState);
					}

					if (m_ActiveState == true)
					{
						child->m_GameObject->Update(dTime);
					}
				}
			}
		}
	}
}

void ComponentEngine::GameObject::LateUpdate(float dTime)
{
	if (m_ActiveState)
	{
		for (Component* co : m_Components)
		{
			co->LateUpdate(dTime);
		}

		if (m_Transform == nullptr)
			return;

		// 자식 호출
		if (m_Transform->GetChildCount() > 0)
		{
			for (int i = 0; i < m_Transform->GetChildCount(); i++)
			{
				m_Transform->GetChild(i)->m_GameObject->LateUpdate(dTime);
			}
		}
	}
}

void ComponentEngine::GameObject::OnPreRender()
{
	if (m_ActiveState)
	{
		for (Component* co : m_Components)
		{
			co->OnPreRender();
		}
	}
}

void ComponentEngine::GameObject::OnRender()
{
	if (m_ActiveState)
	{
		for (Component* co : m_Components)
		{
			co->OnRender();
		}
	}
}

void ComponentEngine::GameObject::OnPostRender()
{
	if (m_ActiveState)
	{
		for (Component* co : m_Components)
		{
			co->OnPostRender();
		}
	}
}

void ComponentEngine::GameObject::OnDrawGizmos()
{
	if (m_ActiveState)
	{
		for (Component* co : m_Components)
		{
			co->OnDrawGizmos();
		}
	}
}

void ComponentEngine::GameObject::OnGUI()
{
	if (m_ActiveState)
	{
		for (Component* co : m_Components)
		{
			co->OnGUI();
		}

		if (m_Transform == nullptr)
			return;

		// 자식 호출
		if (m_Transform->GetChildCount() > 0)
		{
			for (int i = 0; i < m_Transform->GetChildCount(); i++)
			{
				m_Transform->GetChild(i)->m_GameObject->OnGUI();
			}
		}
	}
}

void ComponentEngine::GameObject::DebugTextRender()
{
	if (m_ActiveState)
	{
		for (Component* co : m_Components)
		{
			co->DebugTextRender();
		}
	}
}

void ComponentEngine::GameObject::OnQuit()
{
	if (m_ActiveState)
	{
		for (Component* co : m_Components)
		{
			co->OnQuit();
		}
	}
}

void ComponentEngine::GameObject::OnDisable()
{
	if (m_ActiveState)
	{
		for (Component* co : m_Components)
		{
			co->OnDisable();
		}
	}
}

void ComponentEngine::GameObject::OnDestroy()
{
	if (m_ActiveState)
	{
		for (Component* co : m_Components)
		{
			co->OnDestroy();
		}
	}
}

void ComponentEngine::GameObject::SetActive(bool value)
{
	if (m_ActiveState != value)
	{
		if (value == true)
		{
			for (Component* co : m_Components)
			{
				co->Start();
			}
		}
	}
	m_ActiveState = value;
}

void ComponentEngine::GameObject::SetActiveRecursively(bool state)
{
	if (state == true && m_ActiveState != state)
	{
		for (Component* co : m_Components)
		{
			co->Start();
		}

		// 자식 호출 - Start
		if (m_Transform)
		{
			if (m_Transform->GetChildCount() > 0)
			{
				for (int i = 0; i < m_Transform->GetChildCount(); i++)
				{
					m_Transform->GetChild(i)->m_GameObject->Start();
				}
			}
		}

	}
	m_ActiveState = state;

	// 자식 호출 - SetActiveRecursively
	if (m_Transform)
	{
		if (m_Transform->GetChildCount() > 0)
		{
			for (int i = 0; i < m_Transform->GetChildCount(); i++)
			{
				m_Transform->GetChild(i)->m_GameObject->SetActiveRecursively(state);
			}
		}
	}
}

void ComponentEngine::GameObject::AttatchShape(physx::PxShape* shape)
{
	m_PhysicsActor->attachShape(*shape);
}
