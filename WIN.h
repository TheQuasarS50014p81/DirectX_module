#pragma once
#include <Windows.h>
#include <d3d11.h>
#include <D3DX10.h>
#include <d3dx11.h>
#include <d3dcompiler.h>
#pragma comment(lib,"d3d11.lib")
#pragma comment(lib,"d3dx10.lib")
#pragma comment(lib,"d3dx11.lib")
#pragma comment(lib,"d3dCompiler.lib")

//定数定義
#define WINDOW_WIDTH 640	//ウィンドウ幅
#define WINDOW_HEIGHT 480	//ウィンドウ高さ
#define APP_NAME	L"DxirectX_Modules"
#define SAFE_RELEASE(x) if(x){x->Release(); x = NULL;}

//
//
//
class WIN{
public:
	HRESULT InitWindow(HINSTANCE, INT, INT, INT, INT, LPCWSTR);
	HRESULT InitD3D();
	LRESULT MsgProc(HWND, UINT, WPARAM, LPARAM);
	void Run();
	void DestroyD3D();

	HWND m_hWnd;
	ID3D11Device *m_pDevice;
	ID3D11DeviceContext *m_pDeviceContext;
	IDXGISwapChain *m_pSwapChain;
	ID3D11RenderTargetView *m_pRenderTargetView;
	ID3D11DepthStencilView *m_pDepthStencilView;
	ID3D11Texture2D *m_pDepthStencil;
};