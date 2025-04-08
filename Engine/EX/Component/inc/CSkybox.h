#pragma once
#include "Component.h"
#include "EngineInterface.h"

namespace ComponentEngine
{
	class Skybox : public Component
	{

	public:
		std::string m_FilePath;

	private:
		GRAPHICENGINE::ISkyBoxInterface* m_pSkybox;

	public:
		Skybox();
		virtual ~Skybox();

	public:
		virtual void Awake() override;

	public:
		virtual void Bind(GameObject* obj) override;
	};
}

