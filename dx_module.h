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

//�}�N��
#define MAX_MODEL	10		//���f���ő吔
#define MAX_SHOT	100		//�e�ő吔
#define MAX_WAV		100		//WAV�T�E���h�ő吔

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
	//�I�[�f�B�I�n
	IXAudio2 *m_pXAudio2;
	IXAudio2MasteringVoice *m_pMasteringVoice;
	//�T�E���h�t�@�C��
	IXAudio2SourceVoice *m_pSourceVoice[MAX_WAV];
	BYTE *m_pWavBuffer[MAX_WAV];		//�g�`�f�[�^�i�t�H�[�}�b�g�����܂܂Ȃ��A�����ɔg�`�f�[�^�[�̂݁j
	DWORD	m_dwWavSize[MAX_WAV];		//�g�`�f�[�^�[�̃T�C�Y
};