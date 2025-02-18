#pragma once
class GameLogic
{
public:
	GameLogic();
	~GameLogic();

private:
	const int m_ScreenWidth = 1920;
	const int m_ScreenHeight = 1080;

	HWND m_hWnd;
	MSG m_Msg;

public:
	void Initialize();
	void GameLoop();
	void Finalize();
};

