#pragma once
#include "GameObject.h"

namespace ComponentEngine
{
	class CECamera;
	class CEAudioListener;
	class Scene
	{
	private:
		std::string					m_SceneName;
		std::vector<GameObject*>	m_GameObjects;
		std::vector<GameObject*>	m_RootObjects;

		CECamera*					m_MainCamera;
		CEAudioListener*				m_AudioListener;

		bool						m_isInputMouseLock;

		bool						m_ShowDebug;

		bool						m_isLoadFbxComplete;
		bool						m_isSetModelComplete;

	public:
		int							m_LoadFbxCompleteCount;
		int							m_ObjectCount;

		HANDLE m_Handle;

	public:
		Scene();
		Scene(std::string sceneName);
		~Scene();

	public:
		void LoadFbxFile();
		void SetFbxModel();
		void AddGameObjectByName(std::string name);
		void AddGameObject(GameObject* obj);
		void AddGameObject(GameObject* obj, GameObject* parent);
		void AddRootObject(GameObject* obj);
		void DeleteGameObject(GameObject* obj);
		void DeleteRootObject(GameObject* obj);
		void Awake();
		void OnEnable();
		void Start();
		void Update(float dTime);
		void Render();
		void OnDestroy();

		GameObject* FindGameObjectWithName(std::string name);

	public:
		std::vector<GameObject*> GetRootObjects() const { return m_RootObjects; }
		std::vector<GameObject*> GetObjects() const { return m_GameObjects; }

		CECamera* GetMainCamera() const { return m_MainCamera; }
		void SetMainCamera(CECamera* cam) { m_MainCamera = cam; }

		CEAudioListener* GetAudioListener() const { return m_AudioListener; }
		void SetAudioListener(CEAudioListener* audioListener) { m_AudioListener = audioListener; }

		void SetInputMouseLock(bool state);

		void SetDebugTextDraw(bool value) { m_ShowDebug = value; }

		void SetLoadComplete(bool val) { m_isLoadFbxComplete = val; }
		bool GetLoadComplete() const { return m_isLoadFbxComplete; }

		void SetModelSettingComplete(bool val) { m_isSetModelComplete = val; }
		bool GetModelSettingComplete() const { return m_isSetModelComplete; }

		std::string GetSceneName() const { return m_SceneName; }


		/// 씬에서 Draw Call -> Graphics에서 오브젝트들 알파소팅해서 Model들 Draw. 
		/// Camera Transform, Model Transform 가공해서 데이터 넘기는 쪽으로.
	};
}

