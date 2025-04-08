#pragma once
#include <SimpleMath.h>

namespace ComponentEngine
{
	class Ray
	{

	public:
		DirectX::SimpleMath::Vector3 m_Origin;
		DirectX::SimpleMath::Vector3 m_Direction;

	public:
		Ray(DirectX::SimpleMath::Vector3 origin, DirectX::SimpleMath::Vector3 direction);
		~Ray();

	public:
		DirectX::SimpleMath::Vector3 GetPoint(float distance);

	};
}

