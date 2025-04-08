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

		Color m_FogColor;				// 안개 컬러
		float m_FogDensity;				// 안개 밀도 (0에 가까울수록 걷힌다. 1에 가까울수록 짙어진다)
		bool m_FogActive;				// 안개 ON / OFF (default ON)

		eLightType m_LightType;			// 광원 타입
		int m_Range;					// 광원의 범위
		Color m_Color;					// 광원의 컬러
		float m_Intensity;				// 광원 밝기. Unity 기준 Directional 0.5 / 나머지 1
		float m_IndirectMultiplier;		// 간접광 강도

		eShadowType m_ShadowType;		// 그림자 타입 (없음, 소프트, 하드)

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

