#include "framework.h"
#include "main.h"
#include "Process.h"
#include <comdef.h>

// ���α׷��� ��Ʈ�� ����Ʈ.
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

		// �ʱ�ȭ
		pProcess->Window_Initalize(hInstance);
		pProcess->Initalize();

		// ���� ����
		pProcess->Loop();

		delete pProcess;
	}
	catch (_com_error e)
	{
		OutputDebugString(e.ErrorMessage());
	}

	return 0;
}