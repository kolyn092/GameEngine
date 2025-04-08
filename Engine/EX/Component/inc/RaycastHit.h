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
		Collider* col;											// �浹�� �ݶ��̴�
		DirectX::SimpleMath::Vector3 point;						// �浹�� �浹ü�� �浹 ����
		DirectX::SimpleMath::Vector3 normal;					// �浹�� ǥ���� ���
		DirectX::SimpleMath::Vector3 barycentricCoordinate;		// �浹�� �ﰢ���� �����߽� ��ǥ
		float distance;											// ���� ���������� �浹 ���������� �Ÿ�
		int triangleIndex;										// �浹�� �ﰢ���� �ε���
		Transform* transform;									// �浹�� �ݶ��̴��� Ʈ������
		Rigidbody* rigidbody;									// �浹�� �ݶ��̴��� ������ٵ�. ������ null ��ȯ
	};

}

