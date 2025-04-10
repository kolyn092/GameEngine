#pragma once
#include "Collider.h"

namespace ComponentEngine
{
	// �� �ݶ��̴�
	class SphereCollider : public Collider
	{

	public:
		DirectX::SimpleMath::Vector3 m_Center;
		float m_Radius;

	public:
		SphereCollider();
		virtual ~SphereCollider();

	public:
		void SetRadius(float radius);

	private:
		virtual void SetGeometryData() override;

	public:
		virtual void SetCenter(DirectX::SimpleMath::Vector3 pos = DirectX::SimpleMath::Vector3::Zero) override;

	public:
		virtual void Awake() override;

	public:
		virtual void Bind(GameObject* obj) override;
	};

}

