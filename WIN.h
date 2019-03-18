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

//�x����\��
#pragma warning(disable : 4305)

//�萔��`
#define WINDOW_WIDTH 640	//�E�B���h�E��
#define WINDOW_HEIGHT 480	//�E�B���h�E����
#define MAX_MODEL	10		//���f���ő吔
#define MAX_SHOT	100
#define APP_NAME	L"DxirectX_Modules"

//�}�N��
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

//���̂̍\����
struct MODEL	//�����ł͒P�Ȃ�O�p�|���S��
{
	D3DXVECTOR3 vPos;
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

	//������
	HWND m_hWnd;
	ID3D11Device *m_pDevice;
	ID3D11DeviceContext *m_pDeviceContext;
	IDXGISwapChain *m_pSwapChain;
	ID3D11RenderTargetView *m_pRenderTargetView;
	ID3D11DepthStencilView *m_pDepthStencilView;
	ID3D11Texture2D *m_pDepthStencil;
	//�V�K
	ID3D11InputLayout	*m_pVertexLayout;
	ID3D11VertexShader	*m_pVertexShader;
	ID3D11PixelShader	*m_pPixelShader;
	ID3D11Buffer		*m_pConstantBuffer;
	//�o�[�e�b�N�X�o�b�t�@
	ID3D11Buffer		*m_pVertexBuffer;
	//���f���̃C���X�^���X�z��
	MODEL m_Model[MAX_MODEL];
	int m_iNumModel;
	MODEL m_Shot[MAX_SHOT];
	int m_iNumShot;
};