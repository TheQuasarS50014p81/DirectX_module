#pragma once
#include <Windows.h>
#include <d3d11.h>
#include <D3DX10.h>
#include <d3dx11.h>
#include <d3dcompiler.h>
#pragma comment(lib,"winmm.lib")
#pragma comment(lib,"d3d11.lib")
#pragma comment(lib,"d3dx10.lib")
#pragma comment(lib,"d3dx11.lib")
#pragma comment(lib,"d3dCompiler.lib")

//警告非表示
#pragma warning(disable : 4305)

//定数定義
#define WINDOW_WIDTH 640	//ウィンドウ幅
#define WINDOW_HEIGHT 480	//ウィンドウ高さ
#define APP_NAME	L"DxirectX_Modules"

//マクロ
#define SAFE_RELEASE(x) if(x){x->Release(); x = NULL;}

struct SimpleVertex
{
	D3DXVECTOR3 Pos;	//position
};

struct SIMPLESHADER_CONSTANT_BUFFER
{
	D3DXMATRIX mWVP;
	D3DXVECTOR4 vColor;
};
//
//
//
class WIN{
public:
	WIN();
	~WIN();
	HRESULT InitWindow(HINSTANCE, INT, INT, INT, INT, LPCWSTR);
	HRESULT InitD3D();
	LRESULT MsgProc(HWND, UINT, WPARAM, LPARAM);
	//add function
	HRESULT InitPolygon();
	HRESULT InitShader();
	void Loop();
	void App();
	void Render();
	void DestroyD3D();

	//初期化
	HWND m_hWnd;
	ID3D11Device *m_pDevice;
	ID3D11DeviceContext *m_pDeviceContext;
	IDXGISwapChain *m_pSwapChain;
	ID3D11RenderTargetView *m_pRenderTargetView;
	ID3D11DepthStencilView *m_pDepthStencilView;
	ID3D11Texture2D *m_pDepthStencil;
	//新規
	ID3D11InputLayout	*m_pVertexLayout;
	ID3D11VertexShader	*m_pVertexShader;
	ID3D11PixelShader	*m_pPixelShader;
	ID3D11Buffer		*m_pConstantBuffer;
	//バーテックスバッファ
	ID3D11Buffer		*m_pVertexBuffer;
};