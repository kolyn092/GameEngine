#include "GameLogic.h"
#include <Windows.h>

GameLogic::GameLogic()
{
}

GameLogic::~GameLogic()
{
}

void GameLogic::Initialize()
{

}

void GameLogic::GameLoop()
{
	while (true)
	{
		if (PeekMessage(&m_Msg, NULL, 0, 0, PM_REMOVE))
		{
			if (m_Msg.message == WM_QUIT) break;

			DispatchMessage(&m_Msg);
		}
	}
}

void GameLogic::Finalize()
{

}
