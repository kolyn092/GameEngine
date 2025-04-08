#pragma once
#include "Collider.h"

namespace ComponentEngine
{
	// 박스 콜라이더
	class BoxCollider : public Collider
	{
	private:
		DirectX::SimpleMath::Vector3 m_Center;
		DirectX::SimpleMath::Vector3 m_Size;

	public:
		BoxCollider();
		virtual ~BoxCollider();

	public:
		void SetScale(DirectX::SimpleMath::Vector3 scale);
		void SetScaleX(float x);
		void SetScaleY(float y);
		void SetScaleZ(float z);

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
