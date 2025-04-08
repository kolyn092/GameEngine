#pragma once
#include "Component.h"
#include "Camera.h"

namespace ComponentEngine
{
	class CECamera : public Component
	{

	public:
		enum class eClearFlags
		{
			SKYBOX,
			SOLIDCOLOR,
		};

		enum class eProjection
		{
			PERSPECTIVE,
			ORTHOGRAPHIC,
		};

		enum class eMode
		{
			DEFAULT,
			FOLLOW,
		};

	public:
		int m_ViewPortWidth;
		int m_ViewPortHeight;
		float m_FOV;
		float m_AspectRatio;
		float m_Near;
		float m_Far;
		Color m_BackGroundColor;

		DirectX::SimpleMath::Vector3 m_FollowTargetPos;
		GameObject* m_TargetObj;

		DirectX::SimpleMath::Matrix m_View;
		DirectX::SimpleMath::Matrix m_Proj;

		eMode m_Mode;										// ī�޶� ���
		eClearFlags m_ClearFlags;							// ��ī�̹ڽ� ��� ����
		eProjection m_ViewMode;								// Projection View ���

		bool m_ActiveState;

	public:
		CECamera();
		virtual ~CECamera();

	public:
		void SetViewPort(int width, int height);
		void SetFOV(float fovDegree);
		void SetAspectRatio(float aspectRatio);
		void SetClippingPlanes(float camnear, float camfar);
		void SetClippingPlanes_Near(float camnear);
		void SetClippingPlanes_Far(float camfar);
		void SetBackgroundColor(Color col);
		void SetFollowTarget(GameObject* obj);
		void SetFollowTarget(Vector3 pos);
		void SetCameraMode(eMode mode = eMode::DEFAULT, DirectX::SimpleMath::Vector3 pos = DirectX::SimpleMath::Vector3::Zero);
		void SetClearFlags(eClearFlags flag = eClearFlags::SOLIDCOLOR);
		void SetViewMode(eProjection viewMode);


	public:
		eMode GetCameraMode() const { return m_Mode; }
		DirectX::SimpleMath::Vector3 GetFollowTargetPos() const { return m_FollowTargetPos; }
		DirectX::SimpleMath::Matrix GetViewMatrix() const { return m_View; }
		DirectX::SimpleMath::Matrix GetProjMatrix() const { return m_Proj; }

	public:
		virtual void FixedUpdate() override;
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

