#include "WIN.h"
#include "dx_module.h"

WIN *g_pWin = NULL;
DX_MODULE *d_pModule = NULL;

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInst, LPSTR szStr, INT iCmdShow)
{
	g_pWin = new WIN;
	d_pModule = new DX_MODULE;

	if (g_pWin != NULL) {
		if (SUCCEEDED(g_pWin->InitWindow(hInstance, 0, 0, WINDOW_WIDTH,
			WINDOW_HEIGHT, APP_NAME)))
		{
			if (SUCCEEDED(d_pModule->InitD3D(g_pWin))) {
				d_pModule->Loop();
			}
		}
		d_pModule->DestroyD3D();
		delete g_pWin;
	}
	return 0;
}

//OSからアクセスされるプロシージャ
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	return g_pWin->MsgProc(hWnd, uMsg, wParam, lParam);
}