#pragma once
#include "Script.h"

class Player : public ComponentEngine::Script
{
private:
	enum class eAnim
	{
		None,
		Idle,
		WalkFront, WalkLeft, WalkRight, WalkBack,
		Run,
		Attack, AttackEnd,
	};

private:
	eAnim m_PreAnim;
	eAnim m_NowAnim;

public:
	Player();
	~Player();

public:
	virtual void Awake() override;
	virtual void Update(float dTime) override;

public:
	void UpdateCamera();
	void Input(float dTime);
	void ChangeAnimation();
};

