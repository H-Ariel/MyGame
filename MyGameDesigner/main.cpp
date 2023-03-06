#include "GameDesignerApp.h"

int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{
#ifdef CATCH_EXCEPTIONS
	try
	{
#endif
#ifdef _DEBUG
		// check for memory leaks
		_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

		THROW_IF_FAILED(CoInitialize(nullptr));

		{ GameDesignerApp().Run(); } // I used `{}` to call d'tor before `CoUninitialize`

		CoUninitialize();

#ifdef CATCH_EXCEPTIONS
	}
	catch (exception& ex)
	{
		MessageBoxA(nullptr, ex.what(), "Error", MB_ICONERROR | MB_OK);
	}
#endif

	return 0;
}
