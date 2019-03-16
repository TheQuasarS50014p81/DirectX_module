#include <windows.h>

//�萔��`
#define WINDOW_WIDTH 640	//�E�B���h�E��
#define WINDOW_HEIGHT 480	//�E�B���h�E����

//�O���[�o���ϐ�
HWND g_hWnd = NULL;

//�֐��v���g�^�C�v�錾
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

//
//�G���g���[�֐�
//
INT WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR szStr, INT iCmdShow)
{
	//�E�B���h�E�̏�����
	static WCHAR szAppName[] = L"�E�B���h�E�쐬";

	WNDCLASSEX wc;
	ZeroMemory(&wc, sizeof(wc));

	wc.cbSize = sizeof(wc);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WndProc;
	wc.hInstance = hInst;
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(LTGRAY_BRUSH);
	wc.lpszClassName = szAppName;
	wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

	RegisterClassEx(&wc);

	g_hWnd = CreateWindow(szAppName, szAppName, WS_OVERLAPPEDWINDOW,
		0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, 0, 0, hInst, 0);
	ShowWindow(g_hWnd, SW_SHOW);
	UpdateWindow(g_hWnd);

	//���b�Z�[�W���[�v
	MSG msg;
	ZeroMemory(&msg, sizeof(msg));
	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			//�A�v���P�[�V�����̏����͂����ōs��
		}
	}

	//�I��
	return (INT)msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	switch (iMsg) {
	case WM_KEYDOWN:
		switch ((char)wParam)
		{
		case VK_ESCAPE:
			PostQuitMessage(0);
			break;
		}
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hWnd, iMsg, wParam, lParam);
}