#pragma once
#include "Component.h"

namespace ComponentEngine
{
	class CELight : public Component
	{

	private:
		enum class eLightType
		{
			SPOT, DIRECTIONAL, POINT,
		};

		enum class eShadowType
		{
			NONE, SOFT, HARD,
		};

	private:
		GRAPHICENGINE::IDirectionLightInterface* m_Light;

		Color m_FogColor;				// �Ȱ� �÷�
		float m_FogDensity;				// �Ȱ� �е� (0�� �������� ������. 1�� �������� £������)
		bool m_FogActive;				// �Ȱ� ON / OFF (default ON)

		eLightType m_LightType;			// ���� Ÿ��
		int m_Range;					// ������ ����
		Color m_Color;					// ������ �÷�
		float m_Intensity;				// ���� ���. Unity ���� Directional 0.5 / ������ 1
		float m_IndirectMultiplier;		// ������ ����

		eShadowType m_ShadowType;		// �׸��� Ÿ�� (����, ����Ʈ, �ϵ�)

	private:
		bool m_ActiveState;

	public:
		CELight();
		CELight(eLightType lightType);
		virtual ~CELight();

	public:
		void SetColor(Color col) { m_Color = col; }
		void SetRange(int range) { m_Range = range; }
		void SetIntensity(float value) { m_Intensity = value; }
		void SetAmbient(float value) { m_IndirectMultiplier = value; }

		void SetFogActive(bool state);
		void SetFogColor(Color col);
		void SetFogDensity(float value);

	public:
		virtual void Awake() override;

		virtual void LateUpdate(float dTime) override;

		virtual void DebugTextRender() override;

		virtual void OnQuit() override;
		virtual void OnDisable() override;
		virtual void OnDestroy() override;

		virtual void SetActive(bool value) override;
		virtual bool isActive() override { return m_ActiveState; }

	public:
		virtual void Bind(GameObject* obj) override;
	};
}

