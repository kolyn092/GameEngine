#include "Player.h"
#include "Transform.h"
#include "Input.h"
#include "CCamera.h"
#include "SceneManager.h"
#include "MeshRenderer.h"

Player::Player()
	: m_PreAnim(eAnim::None), m_NowAnim(eAnim::Idle)
{

}

Player::~Player()
{

}

void Player::Awake()
{

}

void Player::Update(float dTime)
{
	// 카메라 업데이트
	UpdateCamera();

	// 마우스 및 키보드 입력
	Input(dTime);

	// 애니메이션 설정
	ChangeAnimation();

	// 애니메이션 현재 상태 저장
	m_PreAnim = m_NowAnim;
}

void Player::UpdateCamera()
{
	ComponentEngine::CCamera* _mainCamera = ComponentEngine::SceneManager::Ins()->GetNowSceneCamera();

	Vector3 _pos = m_GameObject->m_Transform->m_Backward * 8.0f;
	_pos += m_GameObject->m_Transform->m_Up * 7.0f;
	_mainCamera->m_GameObject->GetComponent<ComponentEngine::Transform>()->SetPosition(m_GameObject->m_Transform->GetPosition() + _pos);
	_mainCamera->SetFollowTarget(m_GameObject->GetComponent<ComponentEngine::Transform>()->GetChild(0)->GetPosition());
}

void Player::Input(float dTime)
{
	// 마우스 이동에 따른 좌우 회전
	if (Input::Ins()->GetMouseDistance() > 0)
	{
		if (Input::Ins()->GetMouseDirection().x != 0)
		{
			DirectX::SimpleMath::Vector3 _rotAxis = DirectX::SimpleMath::Vector3::Zero;
			_rotAxis.y = Input::Ins()->GetMouseDirection().x;
			m_GameObject->m_Transform->Rotate(_rotAxis, 360 * dTime);
		}
	}

	// 키보드 이동
	if (Input::Ins()->GetKey(Keyboard::Keys::W))
	{
		if (Input::Ins()->GetKey(Keyboard::Keys::LeftShift))
		{
			m_GameObject->m_Transform->Translate(Vector3(0, 0, 20 * dTime));
			m_NowAnim = eAnim::Run;
		}
		else
		{
			m_GameObject->m_Transform->Translate(Vector3(0, 0, 10 * dTime));
			m_NowAnim = eAnim::WalkFront;
		}
	}
	if (Input::Ins()->GetKey(Keyboard::Keys::S))
	{
		m_GameObject->m_Transform->Translate(Vector3(0, 0, -10 * dTime));
		m_NowAnim = eAnim::WalkBack;
	}
	if (Input::Ins()->GetKey(Keyboard::Keys::A))
	{
		m_GameObject->m_Transform->Translate(Vector3(-10 * dTime, 0, 0));
		m_NowAnim = eAnim::WalkLeft;
	}
	if (Input::Ins()->GetKey(Keyboard::Keys::D))
	{
		m_GameObject->m_Transform->Translate(Vector3(10 * dTime, 0, 0));
		m_NowAnim = eAnim::WalkRight;
	}

	// 키보드 입력(이동)이 끝났을 때는 무조건 idle 상태로 돌아간다.
	if (Input::Ins()->GetKeyUp(Keyboard::Keys::W) || Input::Ins()->GetKeyUp(Keyboard::Keys::S)
		|| Input::Ins()->GetKeyUp(Keyboard::Keys::A) || Input::Ins()->GetKeyUp(Keyboard::Keys::D))
	{
		m_NowAnim = eAnim::Idle;
	}
}

void Player::ChangeAnimation()
{
	if (m_NowAnim != m_PreAnim)
	{
		switch (m_NowAnim)
		{
			case eAnim::Idle:
				m_GameObject->m_MeshRenderer->ChangeAnimation(0, true);
				break;
			case eAnim::WalkFront:
				m_GameObject->m_MeshRenderer->ChangeAnimation(1, true);
				break;
			case eAnim::WalkLeft:
				m_GameObject->m_MeshRenderer->ChangeAnimation(3, true);
				break;
			case eAnim::WalkRight:
				m_GameObject->m_MeshRenderer->ChangeAnimation(4, true);
				break;
			case eAnim::WalkBack:
				m_GameObject->m_MeshRenderer->ChangeAnimation(2, true);
				break;
			case eAnim::Run:
				m_GameObject->m_MeshRenderer->ChangeAnimation(5, true);
				break;
			case eAnim::Attack:
				m_GameObject->m_MeshRenderer->ChangeAnimation(6, true);
				break;
			case eAnim::AttackEnd:
				m_GameObject->m_MeshRenderer->ChangeAnimation(7, true);
				break;
			default:
				break;
		}
	}
}
