#pragma once
#define _WIN32_DCOM
#include <stdio.h>
#include <Windows.h>
#pragma comment(lib,"winmm.lib")

//�萔��`
#define WINDOW_WIDTH 640	//�E�B���h�E��
#define WINDOW_HEIGHT 480	//�E�B���h�E����

//�x����\��
#pragma warning(disable : 4305)


#define APP_NAME	L"DxirectX_Modules"

class WIN{
public:
	WIN();
	~WIN();
	HRESULT InitWindow(HINSTANCE, INT, INT, INT, INT, LPCWSTR);
	LRESULT MsgProc(HWND, UINT, WPARAM, LPARAM);

	//������
	HWND m_hWnd;

};