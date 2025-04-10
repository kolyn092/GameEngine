#pragma once
#include "Component.h"

namespace ComponentEngine
{
	/// ������ �� ��ũ��Ʈ Ŭ������ ��ӹ޾Ƽ� ����Ѵ�.
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


