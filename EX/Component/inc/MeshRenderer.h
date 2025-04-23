#pragma once
#include "Component.h"
#include "CMaterial.h"
#include "EngineInterface.h"
#include "MaterialManager.h"

namespace ComponentEngine
{
	class MeshRenderer : public ComponentImpl<MeshRenderer>
	{

	public:
		std::string m_ModelPath;
		std::vector<std::string> m_ModelPathList;

	private:
		GRAPHICENGINE::IModelInterface* m_Model;
		MaterialManager* m_MaterialManager; // 모델 노드에 들어간 메터리얼들을 가지고 있는 매니저

	private:
		bool m_isAnimLoop;
		bool m_ActiveState;

	public:
		MeshRenderer();
		virtual ~MeshRenderer();

	public:
		// model
		void CreateModel();
		void SetModel();
		void InitModel();

		// animation
		void ChangeAnimation(unsigned int index, bool value);
		void IsAnimLoop(bool value);
		bool GetAnimationEnd();
		void SetAnimON();
		void SetAnimOFF();

		void SetKey(int key);
		int GetNowKey() const;
		int GetTotalKey() const;
		void SetAnimationSpeed(float speed);

		// node
		DirectX::SimpleMath::Matrix GetNodeMatrix(std::string nodeName);
		std::string GetNodeName(int index) { return m_Model->GetNodeName(index); }

		// user matrix (transform component)
		void SetUserMatrix(DirectX::SimpleMath::Matrix matrix);

		// material (texture)
		void CreateMaterialData();
		void SetMaterial(CMaterial* material);
		void InitModelTexture();
		void SetEmissiveActive(bool state);
		void AddEmissiveMaterial(std::string path);	// 임시. 머테리얼 안에 있어야함
		void SetEmissiveMaterial(int index);

	public:
		virtual void Awake() override;

		virtual void Update(float dTime) override;

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
