#pragma once
#include "Component.h"
#include "EngineInterface.h"

namespace ComponentEngine
{
	class CTerrain : public Component
	{

	public:
		std::string m_FilePath;

	private:
		GRAPHICENGINE::ITerrain_Interface* m_Terrain;

	public:
		CTerrain();
		virtual ~CTerrain();

	public:
		virtual void Awake() override;

		virtual void Update() override;

		virtual void DebugTextRender() override;

		virtual void OnQuit() override;
		virtual void OnDisable() override;
		virtual void OnDestroy() override;

		virtual void SetActive(bool value) override;
		virtual bool isActive() override { return true; }

	public:
		virtual void Bind(GameObject* obj) override;
	};
}

