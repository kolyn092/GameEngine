#include "pch.h"
#include "Collider.h"
#include "Rigidbody.h"
#include "Transform.h"
#include "CDebug.h"

ComponentEngine::Collider::Collider(std::string componentName)
	: Component(componentName), m_PhysXEngine(PhysXEngine::PhysX::Ins()), m_Shape(nullptr),
	m_ActiveState(true), m_IsTrigger(false), m_ContactOffset(0.1f), m_Material(nullptr)
{
	m_Material = new PhysicMaterial();
}

ComponentEngine::Collider::Collider(std::string componentName, PhysicMaterial* material)
	: Component(componentName), m_PhysXEngine(PhysXEngine::PhysX::Ins()), m_Shape(nullptr),
	m_ActiveState(true), m_IsTrigger(false), m_ContactOffset(0.1f), m_Material(material)
{

}

ComponentEngine::Collider::~Collider()
{
	if (m_Shape)
	{
		m_PhysXEngine->RemoveCollision(this);
		physx::PxRigidActor* _actor = m_Shape->getActor();
		_actor->detachShape(*m_Shape);
		m_Shape->release();
		m_Shape = nullptr;
	}
	delete m_Material;
}

void ComponentEngine::Collider::SetTrigger(bool value)
{
	m_IsTrigger = value;
	if (m_Shape)
	{
		m_Shape->setFlag(physx::PxShapeFlag::eSIMULATION_SHAPE, !m_IsTrigger);
		m_Shape->setFlag(physx::PxShapeFlag::eTRIGGER_SHAPE, m_IsTrigger);
	}
}

void ComponentEngine::Collider::SetShapeLocal(Vector3 pos, Quaternion rot /*= Quaternion::Identity*/)
{
	if (m_Shape)
	{
		physx::PxTransform _transform(m_PhysXEngine->MakePxTransform(pos, rot));
		m_Shape->setLocalPose(_transform);
	}
}

void ComponentEngine::Collider::SetMaterial(PhysicMaterial* material)
{
	m_Material = material;
	if (m_Shape)
	{
		physx::PxMaterial* _mat[1];
		_mat[0] = m_Material->GetPxMaterial();
		m_Shape->setMaterials(_mat, 1);
	}
}

void ComponentEngine::Collider::Awake()
{
	// Rigidbody 가 없을 경우
	Rigidbody* rigidbody = m_GameObject->GetComponent<Rigidbody>();
	if (!rigidbody)
	{
		if (!m_GameObject->GetActor())
		{
			Transform* _transform = m_GameObject->GetComponent<Transform>();
			physx::PxTransform _pxTrans = m_PhysXEngine->MakePxTransform(_transform->GetPosition(), _transform->GetQuaternion());
			m_GameObject->SetActor(m_PhysXEngine->CreateRigidStatic(_pxTrans));
		}
	}
	SetTrigger(m_IsTrigger);
}

void ComponentEngine::Collider::Start()
{
	m_PhysXEngine->AddCollision(this);
}

void ComponentEngine::Collider::PhysicsUpdate()
{
	if (m_GameObject)
	{
		// 현재 물리 상태값
		physx::PxRigidActor* _actor = m_GameObject->GetActor();
		Transform* _trans = m_GameObject->GetComponent<Transform>();

		const physx::PxMat44 _shapePose(physx::PxShapeExt::getGlobalPose(*m_Shape, *_actor));
		const physx::PxGeometryHolder h = m_Shape->getGeometry();

		if (m_Shape->getFlags() & physx::PxShapeFlag::eTRIGGER_SHAPE)
		{
			// set render state : wire frame
		}

		physx::PxTransform _pxTrans = _actor->getGlobalPose();

		/// 임시 해결. FreezeTransform 하면 충돌을 안하는 원인 찾아낼 것.
		//Vector3 _pos = Vector3::Zero;
		//_pos.x = _pxTrans.p.x;
		//_pos.y = _pxTrans.p.y;
		//_pos.z = _pxTrans.p.z;

		Vector3 _pos = _trans->GetPosition();
		_pos.x = _pxTrans.p.x;
		_pos.x = _pxTrans.p.y;
		_pos.z = _pxTrans.p.z;

		Quaternion _quat = Quaternion::Identity;
		_quat.x = _pxTrans.q.x;
		_quat.y = _pxTrans.q.y;
		_quat.z = _pxTrans.q.z;
		_quat.w = _pxTrans.q.w;

		_trans->SetTransform(_pos, _quat);

		// set render state : solid
	}
}

void ComponentEngine::Collider::DebugTextRender()
{
	//CDebug::Ins()->DebugDrawText(L"%s %s", L"L", L"Collider");
}

void ComponentEngine::Collider::OnQuit()
{

}

void ComponentEngine::Collider::OnDisable()
{

}

void ComponentEngine::Collider::OnDestroy()
{

}

void ComponentEngine::Collider::SetActive(bool value)
{
	if (value == true && m_ActiveState != value)
	{
		Start();
	}
	m_ActiveState = value;
}

void ComponentEngine::Collider::Bind(GameObject* obj)
{

}
