#include "framework.h"
#include "main.h"
#include "Process.h"
#include <comdef.h>

// 프로그램의 엔트리 포인트.
int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	HeapSetInformation(NULL,
		HeapEnableTerminationOnCorruption, NULL, 0);

	try
	{
		Process* pProcess = new Process();

		// 초기화
		pProcess->Window_Initalize(hInstance);
		pProcess->Initalize();

		// 메인 루프
		pProcess->Loop();

		delete pProcess;
	}
	catch (_com_error e)
	{
		OutputDebugString(e.ErrorMessage());
	}

	return 0;
}