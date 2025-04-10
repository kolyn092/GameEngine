#include "pch.h"
#include "CDebug.h"

using namespace DirectX::SimpleMath;

ComponentEngine::CDebug* ComponentEngine::CDebug::m_pInstance = nullptr;

ComponentEngine::CDebug::CDebug()
	: m_DebugPosX(m_DebugPosX_Default), m_DebugPosY(m_DebugPosY_Default), m_Color(Vector4(0.5f, 0, 0.5f, 1)), m_isActive(true)
{

}

ComponentEngine::CDebug::~CDebug()
{

}

void ComponentEngine::CDebug::Reset()
{
	m_DebugPosX = m_DebugPosX_Default;
	m_DebugPosY = m_DebugPosY_Default;
}

void ComponentEngine::CDebug::Spacing()
{
	m_DebugPosY += 14;
}

void ComponentEngine::CDebug::DebugDrawText(int x, int y, const TCHAR* text, ...)
{
	if (m_isActive)
	{
		va_list vl;
		va_start(vl, text);
		DrawText_Debug::GetInstance()->DrawTextEx(0, x, y, m_Color, 100, text, vl);
		va_end(vl);
	}
}

void ComponentEngine::CDebug::DebugDrawText(const TCHAR* text, ...)
{
	if (m_isActive)
	{
		va_list vl;
		va_start(vl, text);
		DrawText_Debug::GetInstance()->DrawTextEx(0, m_DebugPosX, m_DebugPosY, m_Color, 100, text, vl);
		va_end(vl);

		m_DebugPosY += 14;
		if (m_DebugPosY > m_MaxDebugPosY)
		{
			m_DebugPosX += 300;
			m_DebugPosY = m_DebugPosY_Default;
		}
	}
}

void ComponentEngine::CDebug::Finalize()
{
	if (m_pInstance)
	{
		delete m_pInstance;
		m_pInstance = nullptr;
	}
}

void ComponentEngine::CDebug::SetActive(bool value)
{
	m_isActive = value;
}

void ComponentEngine::CDebug::SetColor(DirectX::SimpleMath::Vector4 color)
{
	m_Color = color;
}
