#include "dx_module.h"

DX_MODULE::DX_MODULE()
{
};

DX_MODULE::~DX_MODULE()
{
};

HRESULT DX_MODULE::InitD3D(WIN *win)
{
	//�f�o�C�X�ƃX���b�v�`�F�[��
	DXGI_SWAP_CHAIN_DESC sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.BufferCount = 1;
	sd.BufferDesc.Width = WINDOW_WIDTH;
	sd.BufferDesc.Height = WINDOW_HEIGHT;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.OutputWindow = win->m_hWnd;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.Windowed = TRUE;

	D3D_FEATURE_LEVEL pFeatureLevels = D3D_FEATURE_LEVEL_11_0;
	D3D_FEATURE_LEVEL *pFeatureLevel = NULL;

	if (FAILED(D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL,
		0, &pFeatureLevels, 1, D3D11_SDK_VERSION, &sd, &m_pSwapChain, &m_pDevice,
		pFeatureLevel, &m_pDeviceContext))) {
		return FALSE;
	}
	//�����_�[�^�[�Q�b�g�r���[�̍쐬
	ID3D11Texture2D *pBackBuffer;
	m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
	m_pDevice->CreateRenderTargetView(pBackBuffer, NULL, &m_pRenderTargetView);
	SAFE_RELEASE(pBackBuffer);
	//�[�x�X�e���V���o�b�t�@�r���[�̍쐬
	D3D11_TEXTURE2D_DESC descDepth;
	descDepth.Width = WINDOW_WIDTH;
	descDepth.Height = WINDOW_HEIGHT;
	descDepth.MipLevels = 1;
	descDepth.ArraySize = 1;
	descDepth.Format = DXGI_FORMAT_D32_FLOAT;
	descDepth.SampleDesc.Count = 1;
	descDepth.SampleDesc.Quality = 0;
	descDepth.Usage = D3D11_USAGE_DEFAULT;
	descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	descDepth.CPUAccessFlags = 0;
	descDepth.MiscFlags = 0;
	m_pDevice->CreateTexture2D(&descDepth, NULL, &m_pDepthStencil);
	m_pDevice->CreateDepthStencilView(m_pDepthStencil, NULL, &m_pDepthStencilView);
	//�����_�[�^�[�Q�b�g�r���[�Ɛ[�x�X�e���V���r���[���p�C�v���C���Ƀo�C���h
	m_pDeviceContext->OMSetRenderTargets(1, &m_pRenderTargetView, m_pDepthStencilView);
	//�r���[�|�[�g�̐ݒ�
	D3D11_VIEWPORT vp;
	vp.Width = WINDOW_WIDTH;
	vp.Height = WINDOW_HEIGHT;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	m_pDeviceContext->RSSetViewports(1, &vp);
	//���X�^���C�Y�ݒ�
	D3D11_RASTERIZER_DESC rdc;
	ZeroMemory(&rdc, sizeof(rdc));
	rdc.CullMode = D3D11_CULL_NONE;
	rdc.FillMode = D3D11_FILL_SOLID;
	ID3D11RasterizerState *pIr = NULL;
	m_pDevice->CreateRasterizerState(&rdc, &pIr);
	m_pDeviceContext->RSSetState(pIr);
	SAFE_RELEASE(pIr);
	//�ǉ�
	//�V�F�[�_�[������
	if (FAILED(InitShader()))
	{
		return E_FAIL;
	}
	//�|���S���쐬
	if (FAILED(InitPolygon()))
	{
		return E_FAIL;
	}

	return S_OK;
}

//�V�F�[�_�[���쐬
HRESULT DX_MODULE::InitShader()
{
	//hlslfile import and create blob
	ID3DBlob *pCompiledShader = NULL;
	ID3DBlob *pErrors = NULL;
	//Blob����o�[�e�b�N�X�V�F�[�_�[���쐬
	if (FAILED(D3DX11CompileFromFile(L"Simple.hlsl", NULL, NULL, "VS", "vs_5_0", 0, 0, NULL, &pCompiledShader, &pErrors, NULL)))
	{
		MessageBox(0, L"hlsl�ǂݍ��ݎ��s", NULL, MB_OK);
		return E_FAIL;
	}
	SAFE_RELEASE(pErrors);

	if (FAILED(m_pDevice->CreateVertexShader(pCompiledShader->GetBufferPointer(), pCompiledShader->GetBufferSize(), NULL, &m_pVertexShader)))
	{
		SAFE_RELEASE(pCompiledShader);
		MessageBox(0, L"�o�[�e�b�N�X�V�F�[�_�[�쐬���s", NULL, MB_OK);
		return E_FAIL;
	}
	//���_�C���v�b�g���C�A�E�g���`
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0},
	};
	UINT numElements = sizeof(layout) / sizeof(layout[0]);
	//���_�C���v�b�g���C�A�E�g���쐬
	if (FAILED(m_pDevice->CreateInputLayout(layout, numElements, pCompiledShader->GetBufferPointer(), pCompiledShader->GetBufferSize(), &m_pVertexLayout)))
	{
		return FALSE;
	}
	//blob����s�N�Z���V�F�[�_�[���쐬
	if (FAILED(D3DX11CompileFromFile(L"Simple.hlsl", NULL, NULL, "PS", "ps_5_0", 0, 0, NULL, &pCompiledShader, &pErrors, NULL)))
	{
		MessageBox(0, L"hlsl�ǂݍ��ݎ��s", NULL, MB_OK);
		return E_FAIL;
	}
	SAFE_RELEASE(pErrors);
	if (FAILED(m_pDevice->CreatePixelShader(pCompiledShader->GetBufferPointer(), pCompiledShader->GetBufferSize(), NULL, &m_pPixelShader)))
	{
		SAFE_RELEASE(pCompiledShader);
		MessageBox(0, L"�s�N�Z���V�F�[�_�[�쐬���s", NULL, MB_OK);
		return E_FAIL;
	}
	SAFE_RELEASE(pCompiledShader);
	//�R���X�^���ƃo�b�t�@�[�쐬�A�����ł͕ϊ��s��n��
	D3D11_BUFFER_DESC cb;
	cb.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cb.ByteWidth = sizeof(SIMPLESHADER_CONSTANT_BUFFER);
	cb.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cb.MiscFlags = 0;
	cb.StructureByteStride = 0;
	cb.Usage = D3D11_USAGE_DYNAMIC;

	if (FAILED(m_pDevice->CreateBuffer(&cb, NULL, &m_pConstantBuffer)))
	{
		return E_FAIL;
	}
	return S_OK;
}
//�A�v���P�[�V���������B�A�v���̃��C���֐�
void DX_MODULE::App()
{
	Render();
}

//���b�Z�[�W���[�v�ƃA�v���P�[�V���������̓����
void DX_MODULE::Loop()
{
	//message roop
	MSG msg = { 0 };
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
			//application run
			App();
		}
	}
	//exit application
}

//�|���S���쐬
HRESULT DX_MODULE::InitPolygon()
{
	//�o�[�e�b�N�X�o�b�t�@�[�쐬
	SimpleVertex vertices[] =
	{
		D3DXVECTOR3(-0.5,-0.5,0),//���_1	
		D3DXVECTOR3(-0.5,0.5,0), //���_2
		D3DXVECTOR3(0.5,-0.5,0),  //���_3
		D3DXVECTOR3(0.5,0.5,0), //���_4	
	};
	D3D11_BUFFER_DESC bd;
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(SimpleVertex) * 4;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA InitData;
	InitData.pSysMem = vertices;
	if (FAILED(m_pDevice->CreateBuffer(&bd, &InitData, &m_pVertexBuffer)))
	{
		return E_FAIL;
	}
	//�o�[�e�b�N�X�o�b�t�@���Z�b�g
	UINT stride = sizeof(SimpleVertex);
	UINT offset = 0;
	m_pDeviceContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);

	return S_OK;
}

//�����_�����O
void DX_MODULE::Render()
{
	//��ʃN���A
	float ClearColor[4] = { 0,0,1,1 };	//RBGA
	m_pDeviceContext->ClearRenderTargetView(m_pRenderTargetView, ClearColor);	//��ʃN���A
	m_pDeviceContext->ClearDepthStencilView(m_pDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);	//�[�x�o�b�t�@

	D3DXMATRIX mWorld;
	D3DXMATRIX mView;
	D3DXMATRIX mProj;

	//���[���h�g�����X�t�H�[���i��΍��W�ϊ��j
	D3DXMatrixRotationY(&mWorld, timeGetTime() / 100.0f);	//�P����yaw��]������
	//�r���[�g�����X�t�H�[��
	D3DXVECTOR3 vEyePt(0.0f, 1.0f, -2.0f);	//camera position
	D3DXVECTOR3 vLookatPt(0.0f, 0.0f, 0.0f);	//�����ʒu
	D3DXVECTOR3 vUpVec(0.0f, 1.0f, 0.0f);	//����ʒu
	D3DXMatrixLookAtLH(&mView, &vEyePt, &vLookatPt, &vUpVec);
	//�v���W�F�N�V�����g�����X�t�H�[���i�ˉe�ϊ��j
	D3DXMatrixPerspectiveFovLH(&mProj, D3DX_PI / 4, (FLOAT)WINDOW_WIDTH / (FLOAT)WINDOW_HEIGHT, 0.1f, 100.0f);

	//�g�p����V�F�[�_�[�̓o�^
	m_pDeviceContext->VSSetShader(m_pVertexShader, NULL, 0);
	m_pDeviceContext->PSSetShader(m_pPixelShader, NULL, 0);
	//�V�F�[�_�[�̃R���X�^���g�o�b�t�@�[�Ɋe��f�[�^�[��n��
	D3D11_MAPPED_SUBRESOURCE pData;
	SIMPLESHADER_CONSTANT_BUFFER cb;
	if (SUCCEEDED(m_pDeviceContext->Map(m_pConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &pData)))
	{
		//���[���h�A�J�����A�ˉe�s���n��
		D3DXMATRIX m = mWorld * mView*mProj;
		D3DXMatrixTranspose(&m, &m);
		cb.mWVP = m;
		//�J���[��n��
		D3DXVECTOR4 vColor(0, 1, 0, 0);
		cb.vColor = vColor;
		memcpy_s(pData.pData, pData.RowPitch, (void*)(&cb), sizeof(cb));
		m_pDeviceContext->Unmap(m_pConstantBuffer, 0);
	}
	//���̃R���X�^���g�o�b�t�@�[���g���V�F�[�_�[�̓o�^
	m_pDeviceContext->VSSetConstantBuffers(0, 1, &m_pConstantBuffer);
	m_pDeviceContext->PSSetConstantBuffers(0, 1, &m_pConstantBuffer);
	//���_�C���v�b�g���C�A�E�g
	m_pDeviceContext->IASetInputLayout(m_pVertexLayout);
	//�v���~�e�B�u�g�|���W�[���Z�b�g
	m_pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	//�v���~�e�B�u�������_�����O
	m_pDeviceContext->Draw(4, 0);

	m_pSwapChain->Present(0, 0);		//��ʍX�V
}



//���������
void DX_MODULE::DestroyD3D()
{
	SAFE_RELEASE(m_pVertexLayout);
	SAFE_RELEASE(m_pVertexShader);
	SAFE_RELEASE(m_pPixelShader);
	SAFE_RELEASE(m_pConstantBuffer);
	SAFE_RELEASE(m_pVertexBuffer);
	SAFE_RELEASE(m_pSwapChain);
	SAFE_RELEASE(m_pRenderTargetView);
	SAFE_RELEASE(m_pDeviceContext);
	SAFE_RELEASE(m_pDepthStencil);
	SAFE_RELEASE(m_pDepthStencilView);
	SAFE_RELEASE(m_pDevice);
	SAFE_RELEASE(m_pXAudio2);
}

HRESULT DX_MODULE::InitXAudio()
{
	CoInitializeEx(NULL, COINIT_MULTITHREADED);

	if (FAILED(XAudio2Create(&m_pXAudio2, XAUDIO2_DEBUG_ENGINE))) {
		CoUninitialize();
		return E_FAIL;
	}
	if (FAILED(m_pXAudio2->CreateMasteringVoice(&m_pMasteringVoice))) {
		CoUninitialize();
		return E_FAIL;
	}
	return S_OK;
}

HRESULT DX_MODULE::LoadSound(LPSTR szFileName, DWORD dwIndex)
{
	HMMIO hMmio = NULL;		//windows�}���`���f�B�AAPI�̃n���h���iwindows�}���`���f�B�AAPI��wav�t�@�C���֌W����p��API�j
	DWORD dwWavSize = 0;	//WAV�t�@�C������WAV�f�[�^�[�̃T�C�Y�iWAV�t�@�C����WAV�f�[�^�Ő�߂��Ă���)
	WAVEFORMATEX *pwfex;	//wav�̃t�H�[�}�b�g
	MMCKINFO	ckInfo;		//�`�����N���
	MMCKINFO	riffckInfo;	//�ŏ㕔�`�����N�iRIFF�`�����N�j�ۑ��p
	PCMWAVEFORMAT pcmWaveForm;
	//WAV�t�@�C�����̃w�b�_�[���̊m�F�Ɠǂݍ���
	hMmio = mmioOpenA(szFileName, NULL, MMIO_ALLOCBUF | MMIO_READ);
	//�t�@�C���|�C���^��RIFF�`�����N�̐擪�ɃZ�b�g����
	mmioDescend(hMmio, &riffckInfo, NULL, 0);
	//�t�@�C���|�C���^��'f','m','t' ' ' �`�����N�ɃZ�b�g����
	ckInfo.ckid = mmioFOURCC('f', 'm', 't', ' ');
	mmioDescend(hMmio, &ckInfo, &riffckInfo, MMIO_FINDCHUNK);
	//�t�H�[�}�b�g��ǂݍ���
	mmioRead(hMmio, (HPSTR)&pcmWaveForm, sizeof(pcmWaveForm));
	pwfex = (WAVEFORMATEX*)new CHAR[sizeof(pcmWaveForm)];
	memcpy(pwfex, &pcmWaveForm, sizeof(pcmWaveForm));
	pwfex->cbSize = 0;
	mmioAscend(hMmio, &ckInfo, 0);
	//WAV�t�@�C�����̉��f�[�^�̓ǂݍ���
	ckInfo.ckid = mmioFOURCC('d', 'a', 't', 'a');
	mmioDescend(hMmio, &ckInfo, &riffckInfo, MMIO_FINDCHUNK);	//�f�[�^�`�����N�ɃZ�b�g
	dwWavSize = ckInfo.cksize;
	m_pWavBuffer[dwIndex] = new BYTE[dwWavSize];
	DWORD dwOffset = ckInfo.dwDataOffset;
	mmioRead(hMmio, (HPSTR)m_pWavBuffer[dwIndex], dwWavSize);
	//�\�[�X�{�C�X�Ƀf�[�^�[���l�ߍ���
	if (FAILED(m_pXAudio2->CreateSourceVoice(&m_pSourceVoice[dwIndex], pwfex)))
	{
		MessageBox(0, L"�\�[�X�{�C�X�쐬���s", 0, MB_OK);
		return E_FAIL;
	}
	m_dwWavSize[dwIndex] = dwWavSize;

	return S_OK;
}

HRESULT DX_MODULE::PlaySound(DWORD dwIndex)
{
	XAUDIO2_BUFFER buffer = { 0 };
	buffer.pAudioData = m_pWavBuffer[dwIndex];
	buffer.Flags = XAUDIO2_END_OF_STREAM;
	buffer.AudioBytes = m_dwWavSize[dwIndex];
	if (FAILED(m_pSourceVoice[dwIndex]->SubmitSourceBuffer(&buffer)))
	{
		MessageBox(0, L"�\�[�X�{�C�X�ɃT�u�~�b�g���s", 0, MB_OK);
		return E_FAIL;
	}
	m_pSourceVoice[dwIndex]->Start(0, XAUDIO2_COMMIT_NOW);
}