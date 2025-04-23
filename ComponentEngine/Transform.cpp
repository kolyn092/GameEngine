#include "pch.h"
#include "Transform.h"
#include "CUtil.h"
#include "MeshRenderer.h"
#include "Rigidbody.h"
#include "Scene.h"
#include "CDebug.h"

ComponentEngine::Transform::Transform()
	: ComponentImpl("Transform"), m_Root(nullptr), m_Parent(nullptr),
	m_Forward(Vector3(0, 0, 1)), m_Backward(Vector3(0, 0, -1)), m_Up(Vector3(0, 1, 0)), m_Left(Vector3(-1, 0, 0)), m_Right(Vector3(1, 0, 0)),
	m_Position(Vector3::Zero), m_Rotation(Quaternion::Identity), m_Scale(Vector3::One),
	m_LocalPosition(Vector3::Zero), m_LocalRotation(Quaternion::Identity), m_LocalScale(Vector3::One),
	m_EulerAngles(Vector3::Zero), m_LocalEulerAngles(Vector3::Zero),
	m_LocalTM(Matrix::Identity), m_WorldTM(Matrix::Identity)
{
	m_Root = this;
}

ComponentEngine::Transform::~Transform()
{
	if (m_Root)
	{
		m_Root = nullptr;
	}

	if (m_Parent)
	{
		m_Parent = nullptr;
	}
}

void ComponentEngine::Transform::Awake()
{
	MakeTransformMatrix();
}

void ComponentEngine::Transform::Update()
{
	Rigidbody* _rig = m_GameObject->GetComponent<Rigidbody>();
	if (_rig)
	{
		_rig->ClearForce();
		_rig->ClearTorque();
	}
}

void ComponentEngine::Transform::DebugTextRender()
{

}

void ComponentEngine::Transform::OnQuit()
{

}

void ComponentEngine::Transform::OnDisable()
{

}

void ComponentEngine::Transform::OnDestroy()
{

}

void ComponentEngine::Transform::Bind(GameObject* obj)
{
	if (obj)
	{
		obj->m_Transform = this;
	}
}

void ComponentEngine::Transform::SetPosition(float x, float y, float z)
{
	m_Position = Vector3(x, y, z);

	if (m_Parent)
	{
		m_LocalPosition = m_Position - m_Parent->m_Position;
	}
	else
	{
		m_LocalPosition = m_Position;
	}

	MakeTransformMatrix();
}

void ComponentEngine::Transform::SetPosition(Vector3 pos)
{
	m_Position = pos;

	if (m_Parent)
	{
		m_LocalPosition = m_Position - m_Parent->m_Position;
	}
	else
	{
		m_LocalPosition = m_Position;
	}

	MakeTransformMatrix();
}

void ComponentEngine::Transform::Translate(float x, float y, float z)
{
	m_Position += Vector3(x, y, z);

	if (m_Parent)
	{
		m_LocalPosition = m_Position - m_Parent->m_Position;
	}
	else
	{
		m_LocalPosition = m_Position;
	}

	MakeTransformMatrix();
}

void ComponentEngine::Transform::Translate(Vector3 translation)
{
	m_Position += translation;

	if (m_Parent)
	{
		m_LocalPosition = m_Position - m_Parent->m_Position;
	}
	else
	{
		m_LocalPosition = m_Position;
	}

	MakeTransformMatrix();
}

void ComponentEngine::Transform::SetRotate(Vector3 eulers)
{
	m_EulerAngles = eulers;
	m_Rotation = Quaternion::CreateFromYawPitchRoll(DegreeToRadian(m_EulerAngles.y), DegreeToRadian(m_EulerAngles.x), DegreeToRadian(m_EulerAngles.z));

	if (m_Parent)
	{
		m_LocalEulerAngles = m_EulerAngles - m_Parent->m_EulerAngles;
		m_LocalRotation = Quaternion::CreateFromYawPitchRoll(DegreeToRadian(m_LocalEulerAngles.y), DegreeToRadian(m_LocalEulerAngles.x), DegreeToRadian(m_LocalEulerAngles.z));
	}
	else
	{
		m_LocalEulerAngles = m_EulerAngles;
		m_LocalRotation = m_Rotation;
	}

	MakeTransformMatrix();
}

void ComponentEngine::Transform::SetRotate(float xAngle, float yAngle, float zAngle)
{
	m_EulerAngles = Vector3(xAngle, yAngle, zAngle);
	m_Rotation = Quaternion::CreateFromYawPitchRoll(DegreeToRadian(m_EulerAngles.y), DegreeToRadian(m_EulerAngles.x), DegreeToRadian(m_EulerAngles.z));

	if (m_Parent)
	{
		m_LocalEulerAngles = m_EulerAngles - m_Parent->m_EulerAngles;
		m_LocalRotation = Quaternion::CreateFromYawPitchRoll(DegreeToRadian(m_LocalEulerAngles.y), DegreeToRadian(m_LocalEulerAngles.x), DegreeToRadian(m_LocalEulerAngles.z));
	}
	else
	{
		m_LocalEulerAngles = m_EulerAngles;
		m_LocalRotation = m_Rotation;
	}

	MakeTransformMatrix();
}

void ComponentEngine::Transform::Rotate(Vector3 eulers)
{
	m_LocalEulerAngles += eulers;

	m_LocalRotation = Quaternion::CreateFromYawPitchRoll(DegreeToRadian(m_EulerAngles.y), DegreeToRadian(m_EulerAngles.x), DegreeToRadian(m_EulerAngles.z));

	if (m_Parent)
	{
		m_LocalEulerAngles = m_EulerAngles - m_Parent->m_EulerAngles;
		m_LocalRotation = Quaternion::CreateFromYawPitchRoll(DegreeToRadian(m_LocalEulerAngles.y), DegreeToRadian(m_LocalEulerAngles.x), DegreeToRadian(m_LocalEulerAngles.z));
	}
	else
	{
		m_LocalEulerAngles = m_EulerAngles;
		m_LocalRotation = m_Rotation;
	}

	MakeTransformMatrix();
}

void ComponentEngine::Transform::Rotate(float xAngle, float yAngle, float zAngle)
{
	m_LocalEulerAngles += Vector3(xAngle, yAngle, zAngle);

	m_LocalRotation = Quaternion::CreateFromYawPitchRoll(DegreeToRadian(m_EulerAngles.y), DegreeToRadian(m_EulerAngles.x), DegreeToRadian(m_EulerAngles.z));

	if (m_Parent)
	{
		m_LocalEulerAngles = m_EulerAngles - m_Parent->m_EulerAngles;
		m_LocalRotation = Quaternion::CreateFromYawPitchRoll(DegreeToRadian(m_LocalEulerAngles.y), DegreeToRadian(m_LocalEulerAngles.x), DegreeToRadian(m_LocalEulerAngles.z));
	}
	else
	{
		m_LocalEulerAngles = m_EulerAngles;
		m_LocalRotation = m_Rotation;
	}

	MakeTransformMatrix();
}

void ComponentEngine::Transform::Rotate(Vector3 axis, float angle)
{
	// TODO : Transform Rotate 기능 보완
	// user script 에서 90도 보다 큰 값을 넣을 경우 문제가 생길 수 있음.
	// 결국 쿼터니언을 오일러각으로 바꿀 때 문제가 생기기 때문에 계산을 여러번 하도록 구현.
	// 다른 Rotate 함수보다 느리게 돌아감.. 
	int _inequalitySign = static_cast<int>(angle / fabs(angle));	// 부등호 저장
	int _quotient = static_cast<int>(fabs(angle / 90));				// 몫
	float _remainder = angle - static_cast<float>(_quotient * 90);	// 나머지

	for (int i = 0; i < _quotient + 1; i++)
	{
		Quaternion q;
		if (i != _quotient)
		{
			q = Quaternion::CreateFromAxisAngle(axis, DegreeToRadian(_inequalitySign * 90));
		}
		else
		{
			q = Quaternion::CreateFromAxisAngle(axis, DegreeToRadian(_remainder));
		}
		m_Rotation *= q;
		m_EulerAngles += QuaternionToEulerAngles(q);
	}

	if (m_Parent)
	{
		m_LocalEulerAngles = m_EulerAngles - m_Parent->m_EulerAngles;
		m_LocalRotation = Quaternion::CreateFromYawPitchRoll(DegreeToRadian(m_LocalEulerAngles.y), DegreeToRadian(m_LocalEulerAngles.x), DegreeToRadian(m_LocalEulerAngles.z));
	}
	else
	{
		m_LocalEulerAngles = m_EulerAngles;
		m_LocalRotation = m_Rotation;
	}

	MakeTransformMatrix();
}

void ComponentEngine::Transform::SetScale(Vector3 scale)
{
	m_Scale = scale;

	if (m_Parent)
	{
		m_LocalScale = m_Scale / m_Parent->m_Scale;
	}
	else
	{
		m_LocalScale = m_Scale;
	}

	MakeTransformMatrix();
}

void ComponentEngine::Transform::SetLocalPosition(float x, float y, float z)
{
	m_LocalPosition = Vector3(x, y, z);

	if (m_Parent == nullptr)
	{
		m_Position = m_LocalPosition;
	}

	MakeTransformMatrix();
}

void ComponentEngine::Transform::SetLocalPosition(Vector3 position)
{
	m_LocalPosition = position;

	if (m_Parent == nullptr)
	{
		m_Position = m_LocalPosition;
	}

	MakeTransformMatrix();
}

void ComponentEngine::Transform::SetLocalRotation(Vector3 rotation)
{
	m_LocalEulerAngles = rotation;
	m_LocalRotation = Quaternion::CreateFromYawPitchRoll(DegreeToRadian(rotation.y), DegreeToRadian(rotation.x), DegreeToRadian(rotation.z));

	if (m_Parent == nullptr)
	{
		m_EulerAngles = m_LocalEulerAngles;
		m_Rotation = m_LocalRotation;
	}

	MakeTransformMatrix();
}

void ComponentEngine::Transform::SetLocalScale(Vector3 scale)
{
	m_LocalScale = scale;

	if (m_Parent == nullptr)
	{
		m_Scale = m_LocalScale;
	}

	MakeTransformMatrix();
}

ComponentEngine::Transform* ComponentEngine::Transform::GetChild(unsigned int index)
{
	if (index < m_Childs.size())
	{
		return m_Childs[index];
	}
	return nullptr;
}

void ComponentEngine::Transform::SetTransform(Vector3 pos, Quaternion rot)
{
	m_Position = pos;
	m_Rotation = rot;
	m_EulerAngles = QuaternionToEulerAngles(m_Rotation);

	if (m_Parent)
	{
		m_WorldTM = Matrix::CreateScale(m_Scale) * Matrix::CreateFromQuaternion(m_Rotation) * Matrix::CreateTranslation(m_Position);

		// 로컬 갱신
		m_LocalTM = m_WorldTM * m_Parent->GetWorldTM().Invert();
		m_LocalTM.Decompose(m_LocalScale, m_LocalRotation, m_LocalPosition);
		m_LocalEulerAngles = QuaternionToEulerAngles(m_LocalRotation);
	}
	else
	{
		m_LocalPosition = m_Position;
		m_LocalRotation = m_Rotation;
		m_LocalEulerAngles = m_EulerAngles;
	}

	MakeTransformMatrix();
}

void ComponentEngine::Transform::SetDirection()
{
	Matrix _mRot = Matrix::CreateFromQuaternion(m_Rotation);
	m_Forward = Vector3::TransformNormal(Vector3(0, 0, 1), _mRot);
	m_Backward = -1 * m_Forward;
	m_Right = m_Up.Cross(m_Forward);
	m_Left = -1 * m_Right;
}

void ComponentEngine::Transform::MakeTransformMatrix()
{
	m_LocalTM = Matrix::CreateScale(m_LocalScale) * Matrix::CreateFromQuaternion(m_LocalRotation) * Matrix::CreateTranslation(m_LocalPosition);

	if (m_Parent)
	{
		//행열 갱신
		m_WorldTM = m_LocalTM * Matrix::CreateScale(m_Parent->m_Scale) * Matrix::CreateFromQuaternion(m_Parent->m_Rotation) * Matrix::CreateTranslation(m_Parent->m_Position);
		m_WorldTM.Decompose(m_Scale, m_Rotation, m_Position);
		m_EulerAngles = QuaternionToEulerAngles(m_Rotation);
	}
	else
	{
		m_WorldTM = m_LocalTM;
	}

	// 피직스 갱신
	if (m_GameObject->GetActor())
	{
		physx::PxTransform _pxTrans(PhysXEngine::PhysX::Ins()->MakePxTransform(m_Position, m_Rotation));
		m_GameObject->GetActor()->setGlobalPose(_pxTrans);
	}

	SetDirection();

	MeshRenderer* _mesh = m_GameObject->GetComponent<MeshRenderer>();
	if (_mesh != nullptr)
	{
		_mesh->SetUserMatrix(m_WorldTM);
	}

	//자식들 갱신
	for (Transform* child : m_Childs)
	{
		child->MakeTransformMatrix();
	}
}

void ComponentEngine::Transform::SetParent(Transform* transform)
{
	if (m_Parent == transform)
	{
		return;
	}

	if (transform)
	{
		// 로컬 갱신
		m_LocalTM = m_LocalTM * transform->GetWorldTM().Invert();
		m_LocalTM.Decompose(m_LocalScale, m_LocalRotation, m_LocalPosition);
		m_LocalEulerAngles = QuaternionToEulerAngles(m_LocalRotation);
	}
	else
	{
		m_LocalTM = m_WorldTM;
		m_LocalPosition = m_Position;
		m_LocalRotation = m_Rotation;
		m_LocalEulerAngles = m_EulerAngles;
		m_LocalScale = m_Scale;
	}

	// 부모 적용
	m_Parent = transform;

	// 루트
	if (m_Parent)
	{
		m_Root = m_Parent->m_Root;

		// 자식으로 넣기
		m_Parent->m_Childs.emplace_back(this);
	}
	else
	{
		m_Root = this;
	}

	for (Transform* child : m_Childs)
	{
		child->m_Root = m_Root;
	}
}

void ComponentEngine::Transform::AttatchChild(Transform* child)
{
	for (Transform* trans : m_Childs)
	{
		if (trans == child)
		{
			return;
		}
	}

	m_GameObject->GetScene()->DeleteRootObject(child->m_GameObject);
	child->SetParent(this);
}

void ComponentEngine::Transform::DetachChild(unsigned int index, Transform* newParent)
{
	if (index < 0 || index >= m_Childs.size())
	{
		return;
	}

	auto it = m_Childs.begin();
	for (int i = 0; i < index; i++)
	{
		++it;
		if (it == m_Childs.end())
		{
			return;
		}
	}

	if (m_Childs.size() < 1)
		return;

	Transform* trans = (*it);
	m_GameObject->GetScene()->AddRootObject(trans->m_GameObject);
	m_Childs.erase(it);
	trans->SetParent(newParent);
}

void ComponentEngine::Transform::DetachChild(std::string name, Transform* newParent)
{
	for (auto it = m_Childs.begin(); it != m_Childs.end(); ++it)
	{
		if ((*it)->m_GameObject->GetName() == name)
		{
			Transform* trans = (*it);
			m_GameObject->GetScene()->AddRootObject(trans->m_GameObject);
			m_Childs.erase(it);
			trans->SetParent(newParent);
			return;
		}
	}
}

void ComponentEngine::Transform::DetachChild(Transform* child, Transform* newParent)
{
	for (auto it = m_Childs.begin(); it != m_Childs.end(); ++it)
	{
		if ((*it) == child)
		{
			Transform* trans = (*it);
			m_GameObject->GetScene()->AddRootObject(trans->m_GameObject);
			m_Childs.erase(it);
			trans->SetParent(newParent);
			return;
		}
	}
}

void ComponentEngine::Transform::ColliderUpdate()
{
	if (m_GameObject->GetActor())
	{
		physx::PxTransform _pxTrans(PhysXEngine::PhysX::Ins()->MakePxTransform(m_Position, m_Rotation));
		m_GameObject->GetActor()->setGlobalPose(_pxTrans);
	}
}
