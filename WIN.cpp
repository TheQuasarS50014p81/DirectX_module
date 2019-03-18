#include "WIN.h"

//グローバル変数
WIN *g_pWin = NULL;

//関数プロトタイプ宣言
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

//
//エントリー関数
//
INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInst, LPSTR szStr, INT iCmdShow)
{
	g_pWin = new WIN;
	if (g_pWin != NULL) {
		if(SUCCEEDED(g_pWin->InitWindow(hInstance,0,0,WINDOW_WIDTH,
			WINDOW_HEIGHT,APP_NAME)))
		{
			if (SUCCEEDED(g_pWin->InitD3D())) {
				g_pWin->Loop();
			}
		}
		g_pWin->DestroyD3D();
		delete g_pWin;
	}
	return 0;
}

//OSからアクセスされるプロシージャ
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	return g_pWin->MsgProc(hWnd, uMsg, wParam, lParam);
}

//コンストラクタ
WIN::WIN() 
{
	ZeroMemory(this, sizeof(WIN));
}

//デストラクタ
WIN::~WIN()
{
}


//ウィンドウ作成
HRESULT WIN::InitWindow(HINSTANCE hInstance, INT iX, INT iY, INT iWidth, INT iHeight, LPCWSTR WindowName)
{
	WNDCLASSEX wc;
	ZeroMemory(&wc, sizeof(wc));

	wc.cbSize = sizeof(wc);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WndProc;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(LTGRAY_BRUSH);
	wc.lpszClassName = WindowName;
	wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	RegisterClassEx(&wc);

	m_hWnd = CreateWindow(WindowName, WindowName, WS_OVERLAPPEDWINDOW,
		0, 0, iWidth, iHeight, 0, 0, hInstance, 0);
	if (!m_hWnd) {
		return E_FAIL;
	}
	//ウィンドウの表示
	ShowWindow(m_hWnd, SW_SHOW);
	UpdateWindow(m_hWnd);

	return S_OK;
}

//ウィンドウプロシージャー
LRESULT WIN::MsgProc(HWND hWnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
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

//メッセージループとアプリケーション処理の入り口
void WIN::Loop()
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

//アプリケーション処理。アプリのメイン関数
void WIN::App()
{
	//移動
	for (int i = 0; i < m_iNumModel; i++) 
	{
		m_Model[i].vPos.x += 0.001f;
	}
	Render();
}

HRESULT WIN::InitD3D()
{
	//デバイスとスワップチェーン
	DXGI_SWAP_CHAIN_DESC sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.BufferCount = 1;
	sd.BufferDesc.Width = WINDOW_WIDTH;
	sd.BufferDesc.Height = WINDOW_HEIGHT;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.OutputWindow = m_hWnd;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.Windowed = TRUE;

	D3D_FEATURE_LEVEL pFeatureLevels = D3D_FEATURE_LEVEL_11_0;
	D3D_FEATURE_LEVEL *pFeatureLevel = NULL;

	if (FAILED(D3D11CreateDeviceAndSwapChain(NULL,D3D_DRIVER_TYPE_HARDWARE, NULL,
		0,&pFeatureLevels, 1,D3D11_SDK_VERSION,&sd,&m_pSwapChain,&m_pDevice,
		pFeatureLevel, &m_pDeviceContext))){
		return FALSE;
	}
	//レンダーターゲットビューの作成
	ID3D11Texture2D *pBackBuffer;
	m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
	m_pDevice->CreateRenderTargetView(pBackBuffer, NULL, &m_pRenderTargetView);
	SAFE_RELEASE(pBackBuffer);
	//深度ステンシルバッファビューの作成
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
	//レンダーターゲットビューと深度ステンシルビューをパイプラインにバインド
	m_pDeviceContext->OMSetRenderTargets(1, &m_pRenderTargetView, m_pDepthStencilView);
	//ビューポートの設定
	D3D11_VIEWPORT vp;
	vp.Width = WINDOW_WIDTH;
	vp.Height = WINDOW_HEIGHT;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	m_pDeviceContext->RSSetViewports(1, &vp);
	//ラスタライズ設定
	D3D11_RASTERIZER_DESC rdc;
	ZeroMemory(&rdc, sizeof(rdc));
	rdc.CullMode = D3D11_CULL_NONE;
	rdc.FillMode = D3D11_FILL_SOLID;
	ID3D11RasterizerState *pIr = NULL;
	m_pDevice->CreateRasterizerState(&rdc, &pIr);
	m_pDeviceContext->RSSetState(pIr);
	SAFE_RELEASE(pIr);
	//追加
	//シェーダー初期化
	if (FAILED(InitShader()))
	{
		return E_FAIL;
	}
	//ポリゴン作成
	if (FAILED(InitPolygon()))
	{
		return E_FAIL;
	}

	return S_OK;
}

//メモリ解放
void WIN::DestroyD3D()
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
}

//シェーダーを作成
HRESULT WIN::InitShader()
{
	//hlslfile import and create blob
	ID3DBlob *pCompiledShader = NULL;
	ID3DBlob *pErrors = NULL;
	//Blobからバーテックスシェーダーを作成
	if (FAILED(D3DX11CompileFromFile(L"Simple.hlsl", NULL, NULL, "VS", "vs_5_0", 0, 0, NULL, &pCompiledShader, &pErrors, NULL)))
	{
		MessageBox(0, L"hlsl読み込み失敗", NULL, MB_OK);
		return E_FAIL;
	}
	SAFE_RELEASE(pErrors);

	if (FAILED(m_pDevice->CreateVertexShader(pCompiledShader->GetBufferPointer(), pCompiledShader->GetBufferSize(), NULL, &m_pVertexShader)))
	{
		SAFE_RELEASE(pCompiledShader);
		MessageBox(0, L"バーテックスシェーダー作成失敗", NULL, MB_OK);
		return E_FAIL;
	}
	//頂点インプットレイアウトを定義
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0},
	};
	UINT numElements = sizeof(layout) / sizeof(layout[0]);
	//頂点インプットレイアウトを作成
	if (FAILED(m_pDevice->CreateInputLayout(layout, numElements, pCompiledShader->GetBufferPointer(), pCompiledShader->GetBufferSize(), &m_pVertexLayout)))
	{
		return FALSE;
	}
	//blobからピクセルシェーダーを作成
	if (FAILED(D3DX11CompileFromFile(L"Simple.hlsl", NULL, NULL, "PS", "ps_5_0", 0, 0, NULL, &pCompiledShader, &pErrors, NULL)))
	{
		MessageBox(0, L"hlsl読み込み失敗", NULL, MB_OK);
		return E_FAIL;
	}
	SAFE_RELEASE(pErrors);
	if (FAILED(m_pDevice->CreatePixelShader(pCompiledShader->GetBufferPointer(), pCompiledShader->GetBufferSize(), NULL, &m_pPixelShader)))
	{
		SAFE_RELEASE(pCompiledShader);
		MessageBox(0, L"ピクセルシェーダー作成失敗", NULL, MB_OK);
		return E_FAIL;
	}
	SAFE_RELEASE(pCompiledShader);
	//コンスタンとバッファー作成、ここでは変換行列渡し
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

//ポリゴン作成
HRESULT WIN::InitPolygon()
{
	//バーテックスバッファー作成
	SimpleVertex vertices[] =
	{
		D3DXVECTOR3(0.0f,0.5f,0.0f),
		D3DXVECTOR3(0.5f, -0.5f, 0.0f),
		D3DXVECTOR3(-0.5f,-0.5f, 0.0f),
	};
	D3D11_BUFFER_DESC bd;
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(SimpleVertex) * 3;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA InitData;
	InitData.pSysMem = vertices;
	if (FAILED(m_pDevice->CreateBuffer(&bd, &InitData, &m_pVertexBuffer)))
	{
		return E_FAIL;
	}
	//バーテックスバッファをセット
	UINT stride = sizeof(SimpleVertex);
	UINT offset = 0;
	m_pDeviceContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);

	//全てのモデルで同じポリゴン。同じバーテックスバッファーを使う。モデルごとに異なるのはモデルの位置と色
	for (int i = 0; i < MAX_MODEL; i++) {
		m_Model[i].vPos = D3DXVECTOR3(float(rand()) / 1000.0f - 16.0f, float(rand()) / 1000.0f - 16.0f, float(rand()) / 1000.0f + 10.0f);	//初期位置はランダム
		m_Model[i].vColor = D3DXVECTOR4(float(rand()) / 32767.0f, float(rand()) / 32767.0f, float(rand()) / 32767.0f, 1.0f);				//色もランダム設定
	}
	m_iNumModel = MAX_MODEL;
	return S_OK;
}
//レンダリング
void WIN::Render() 
{
	//画面クリア
	float ClearColor[4] = { 0,0,1,1 };	//RBGA
	m_pDeviceContext->ClearRenderTargetView(m_pRenderTargetView, ClearColor);	//画面クリア
	m_pDeviceContext->ClearDepthStencilView(m_pDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);	//深度バッファ

	D3DXMATRIX mWorld;
	D3DXMATRIX mView;
	D3DXMATRIX mProj;

	//ワールドトランスフォーム（絶対座標変換）
	D3DXMatrixRotationY(&mWorld, timeGetTime() / 100.0f);	//単純にyaw回転させる
	//ビュートランスフォーム
	D3DXVECTOR3 vEyePt(0.0f, 0.0f, -2.0f);	//camera position
	D3DXVECTOR3 vLookatPt(0.0f, 0.0f, 0.0f);	//注視位置
	D3DXVECTOR3 vUpVec(0.0f, 1.0f, 0.0f);	//上方位置
	D3DXMatrixLookAtLH(&mView, &vEyePt, &vLookatPt, &vUpVec);
	//プロジェクショントランスフォーム（射影変換）
	D3DXMatrixPerspectiveFovLH(&mProj, D3DX_PI / 4, (FLOAT)WINDOW_WIDTH / (FLOAT)WINDOW_HEIGHT, 0.1f, 100.0f);

	//使用するシェーダーの登録
	m_pDeviceContext->VSSetShader(m_pVertexShader, NULL, 0);
	m_pDeviceContext->PSSetShader(m_pPixelShader, NULL, 0);
	//このコンスタントバッファーを使うシェーダーの登録
	m_pDeviceContext->VSSetConstantBuffers(0, 1, &m_pConstantBuffer);
	m_pDeviceContext->PSSetConstantBuffers(0, 1, &m_pConstantBuffer);
	//頂点インプットレイアウト
	m_pDeviceContext->IASetInputLayout(m_pVertexLayout);
	//プリミティブトポロジーをセット
	m_pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	//プリミティブをレンダリング、複数なのでワールドトランスフォームとそれを渡す部分をループ内で処理
	for (int i = 0; i < m_iNumModel; i++) {
		D3DXMATRIX mTrans;
		D3DXMatrixTranslation(&mTrans, m_Model[i].vPos.x, m_Model[i].vPos.y, m_Model[i].vPos.z);
		mWorld = mTrans;
		//シェーダーのコンスタントバッファーに各種データーを渡す
		D3D11_MAPPED_SUBRESOURCE pData;
		SIMPLESHADER_CONSTANT_BUFFER cb;
		if (SUCCEEDED(m_pDeviceContext->Map(m_pConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &pData)))
		{
			//ワールド、カメラ、射影行列を渡す
			D3DXMATRIX m = mWorld * mView*mProj;
			D3DXMatrixTranspose(&m, &m);
			cb.mWVP = m;
			//カラーを渡す
			cb.vColor = m_Model[i].vColor;
			memcpy_s(pData.pData, pData.RowPitch, (void*)(&cb), sizeof(cb));
			m_pDeviceContext->Unmap(m_pConstantBuffer, 0);
		}
		//プリミティブをレンダリング
		m_pDeviceContext->Draw(3, 0);
	}

	m_pSwapChain->Present(0, 0);		//画面更新
}