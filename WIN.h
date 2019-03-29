#pragma once
#define _WIN32_DCOM
#include <stdio.h>
#include <Windows.h>
#pragma comment(lib,"winmm.lib")

//定数定義
#define WINDOW_WIDTH 640	//ウィンドウ幅
#define WINDOW_HEIGHT 480	//ウィンドウ高さ

//警告非表示
#pragma warning(disable : 4305)


#define APP_NAME	L"DxirectX_Modules"

class WIN{
public:
	WIN();
	~WIN();
	HRESULT InitWindow(HINSTANCE, INT, INT, INT, INT, LPCWSTR);
	LRESULT MsgProc(HWND, UINT, WPARAM, LPARAM);

	//初期化
	HWND m_hWnd;

};