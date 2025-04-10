#pragma once
#include "Component.h"
#include "EngineInterface.h"
#include "CMaterial.h"
#include <SimpleMath.h>

namespace ComponentEngine
{
	class UI_Image : public Component
	{ 

	public:
		enum class eImageType
		{
			Filled, 
			Simple = Filled,
		};

		enum class eFillMethod
		{
			Horizontal, Vertical,
		};

		enum class eFillOrigin
		{
			Left, Top,
		};

	public:
		float m_FillAmount;

	private:
		const float m_DefaultAmount = 1.0f;

	private:
		// Source Image
		GRAPHICENGINE::IUiInterface* m_UI;

		std::string m_ImagePath;

		// Color
		DirectX::SimpleMath::Color m_Color;

		// Material
		CMaterial* m_Material;

		eImageType m_ImageType;
		eFillMethod m_FillMethod;
		eFillOrigin m_FillOrigin;

		// Raycast Target


		bool m_ActiveState;

	public:
		UI_Image();
		virtual ~UI_Image();

	public:
		void SetImagePath(std::string path);
		void SetImageType(eImageType imageType) { m_ImageType = imageType; }
		void SetFillMethod(eFillMethod fillMethod) { m_FillMethod = fillMethod; }
		void SetFillOrigin(eFillOrigin fillOrigin) { m_FillOrigin = fillOrigin; }
		DWORD GetImageWidth();
		DWORD GetImageHeight();
		void SetColor(DirectX::SimpleMath::Color col) { m_Color = col; }
		DirectX::SimpleMath::Color GetColor() const { return m_Color; }

		void OffUI();
		void OnUI();

	public:
		virtual void Awake() override;
		virtual void OnEnable() override;
		virtual void Start() override;

		virtual void Update() override;

		virtual void OnGUI() override;
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

