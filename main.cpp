#include <DxLib.h>
#include "Application.h"

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_  HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd)
{
	// πﬁ∞—Ÿ∞Ãﬂ
	lpApplication.getInstance().Run();
	// DxLibÇÃèIóπ
	lpApplication.Exit_DxLib();
}