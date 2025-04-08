#pragma once
#include "Ray.h"

namespace ComponentEngine
{
	class GameObject;
	class Collider;
	class Transform;
	class Rigidbody;

	class RaycastHit
	{
	public:
		Collider* col;											// 충돌한 콜라이더
		DirectX::SimpleMath::Vector3 point;						// 충돌한 충돌체의 충돌 지점
		DirectX::SimpleMath::Vector3 normal;					// 충돌한 표면의 노멀
		DirectX::SimpleMath::Vector3 barycentricCoordinate;		// 충돌한 삼각면의 무게중심 좌표
		float distance;											// 레이 시작점부터 충돌 지점까지의 거리
		int triangleIndex;										// 충돌한 삼각면의 인덱스
		Transform* transform;									// 충돌한 콜라이더의 트랜스폼
		Rigidbody* rigidbody;									// 충돌한 콜라이더의 리지드바디. 없으면 null 반환
	};

}

