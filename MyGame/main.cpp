#include "GameMainApp.h"


int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{
#ifdef CATCH_EXCEPTIONS
	try
	{
#endif
#ifdef _DEBUG
		_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF); // check for memory leaks
#endif
		LogFile::WriteLog("program have been started", LogFile::Information);
		THROW_IF_FAILED(CoInitialize(nullptr));
		{ GameMainApp().Run(); } // I used `{}` to call d'tor before `CoUninitialize`
		CoUninitialize();
		LogFile::WriteLog("program have been ended", LogFile::Information);

#ifdef CATCH_EXCEPTIONS
	}
	catch (exception& ex)
	{
		string msg = ex.what();
		LogFile::WriteLog(msg, LogFile::Error);
		MessageBox(NULL, wstring(msg.begin(), msg.end()).c_str(), L"Error", MB_OK | MB_ICONERROR);
	}
#endif

	return 0;
}
