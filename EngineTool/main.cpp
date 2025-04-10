#include "framework.h"
#include "main.h"
#include <comdef.h>
#include "Process.h"

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
