#pragma once
#include <d3d11.h>
#include <D3DX10.h>
#include <d3dx11.h>
#include <d3dcompiler.h>
#include <XAudio2.h>
#include <mmsystem.h>
#include "WIN.h"

#pragma comment(lib,"d3d11.lib")
#pragma comment(lib,"d3dx10.lib")
#pragma comment(lib,"d3dx11.lib")
#pragma comment(lib,"d3dCompiler.lib")

//マクロ
#define MAX_MODEL	10		//モデル最大数
#define MAX_SHOT	100		//弾最大数
#define MAX_WAV		100		//WAVサウンド最大数

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

//物体の構造体
struct MODEL	//ここでは単なる三角ポリゴン
{
	D3DXVECTOR3 vPos;
	D3DXVECTOR4 vColor;
};

class DX_MODULE 
{
public:
	DX_MODULE();
	~DX_MODULE();
	HRESULT InitD3D(WIN* w);
	HRESULT InitPolygon();
	HRESULT InitShader();
	HRESULT InitXAudio();
	HRESULT LoadSound(LPSTR, DWORD);
	HRESULT PlaySound(DWORD);
	void Loop();
	void App();
	void Render();
	void DestroyD3D();

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
	//モデルのインスタンス配列
	MODEL m_Model[MAX_MODEL];
	int m_iNumModel;
	MODEL m_Shot[MAX_SHOT];
	int m_iNumShot;
	//オーディオ系
	IXAudio2 *m_pXAudio2;
	IXAudio2MasteringVoice *m_pMasteringVoice;
	//サウンドファイル
	IXAudio2SourceVoice *m_pSourceVoice[MAX_WAV];
	BYTE *m_pWavBuffer[MAX_WAV];		//波形データ（フォーマット等を含まない、純粋に波形データーのみ）
	DWORD	m_dwWavSize[MAX_WAV];		//波形データーのサイズ
};