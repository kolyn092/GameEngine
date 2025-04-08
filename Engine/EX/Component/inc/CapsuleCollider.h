#pragma once
#include "Collider.h"

namespace ComponentEngine
{
	// Ä¸½¶ ÄÝ¶óÀÌ´õ
	class CapsuleCollider : public Collider
	{
	public:
		enum class eCapsuleDirection
		{
			XAxis,
			YAxis,
			ZAxis,
		};

	private:
		DirectX::SimpleMath::Vector3 m_Center;
		float m_Radius;
		float m_HalfHeight;
		eCapsuleDirection m_Direction;

	private:
		DirectX::SimpleMath::Quaternion m_Rotate;

	public:
		CapsuleCollider();
		virtual ~CapsuleCollider();

	public:
		void SetRadius(float radius);
		void SetHalfHeight(float halfHeight);
		void SetDirection(eCapsuleDirection dir);

	private:
		virtual void SetGeometryData() override;
		void SetRotateByDirection();

	public:
		virtual void SetCenter(Vector3 pos = Vector3::Zero) override;

	public:
		virtual void Awake() override;

	public:
		virtual void Bind(GameObject* obj) override;
	};


}