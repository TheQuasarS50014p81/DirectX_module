#pragma once
#include <Windows.h>

//定数定義
#define WINDOW_WIDTH 640	//ウィンドウ幅
#define WINDOW_HEIGHT 480	//ウィンドウ高さ
#define APP_NAME	L"ウィンドウ作成"

//
//
//
class WIN{
public:
	HRESULT InitWindow(HINSTANCE, INT, INT, INT, INT, LPCWSTR);
	LRESULT MsgProc(HWND, UINT, WPARAM, LPARAM);
	void Run();

	HWND m_hWnd;
};