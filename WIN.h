#pragma once
#include <Windows.h>

//�萔��`
#define WINDOW_WIDTH 640	//�E�B���h�E��
#define WINDOW_HEIGHT 480	//�E�B���h�E����
#define APP_NAME	L"�E�B���h�E�쐬"

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