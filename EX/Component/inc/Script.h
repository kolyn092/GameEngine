#pragma once
#include "Component.h"

namespace ComponentEngine
{
	/// 유저는 이 스크립트 클래스를 상속받아서 사용한다.
	class Script : public Component
	{
		
	private:
		std::string m_ScriptName;
		int m_Index;

	public:
		Script();
		virtual ~Script();


	public:
		virtual void Bind(GameObject* obj) override;

	};
}


