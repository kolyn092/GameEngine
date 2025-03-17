#include "pch.h"
#include "CESystem.h"
#include "CDebug.h"
#include "PhysX.h"

ComponentEngine::CESystem* ComponentEngine::CESystem::m_pInstance = nullptr;

ComponentEngine::CESystem::CESystem()
	: m_SceneManager(SceneManager::Ins()), m_dTime(0.0f)
{

}

ComponentEngine::CESystem::~CESystem()
{
	if (m_Timer)
	{
		delete m_Timer;
		m_Timer = nullptr;
	}
}

void ComponentEngine::CESystem::Initalize(HWND hWnd)
{
	m_Timer = new ComponentTimer();
	m_Timer->Start();

	// 그래픽스 엔진
	GRAPHICENGINE::EngineSetUp(hWnd);
	GRAPHICENGINE::FbxLoaderSetUp();
	DrawText_Debug::GetInstance()->CreateFont();
	GRAPHICENGINE::SetShadow(true, Vector3(0, 0, 0), 45.f);
	GRAPHICENGINE::SetViewFrustumCulling(true);

	// 물리 엔진
	PhysXEngine::PhysX::Ins()->Init();
}

void ComponentEngine::CESystem::Start()
{
	// 씬은 무조건 0번을 로드한다.
	ComponentEngine::SceneManager::Ins()->LoadScene(0);
}

void ComponentEngine::CESystem::Update()
{
	m_dTime = m_Timer->GetElapseTime();

	m_SceneManager->UpdateScene(m_dTime);
}

void ComponentEngine::CESystem::Draw()
{
	GRAPHICENGINE::BeginRender(Vector4(0.6f, 0.6f, 0.6f, 1.0f));

	m_SceneManager->DrawScene();

	GRAPHICENGINE::EndRender();

	m_Timer->End();
}

void ComponentEngine::CESystem::Finalize()
{
	GRAPHICENGINE::Release();
	GRAPHICENGINE::ReleaseFbxLoader();
	m_SceneManager->Finalize();
	PhysXEngine::PhysX::Ins()->Finalize();
	CDebug::Ins()->Finalize();

	if (m_pInstance)
	{
		delete m_pInstance;
		m_pInstance = nullptr;
	}

	CoUninitialize();
}