#include "pch.h"
#include "Ray.h"

using namespace DirectX::SimpleMath;

ComponentEngine::Ray::Ray(Vector3 origin, Vector3 direction)
	: m_Origin(origin), m_Direction(direction)
{

}

ComponentEngine::Ray::~Ray()
{

}

Vector3 ComponentEngine::Ray::GetPoint(float distance)
{
	m_Direction.Normalize();
	return 	m_Origin + (m_Direction * distance);
}
