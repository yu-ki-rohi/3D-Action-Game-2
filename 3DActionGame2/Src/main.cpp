#include "App.h"

// Suggested 
// include順番の整理 (余裕があれば)

// プログラムは WinMain から始まります
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	App app;
	app.Run();

	return 0;				// ソフトの終了 
}