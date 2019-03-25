//グローバル
cbuffer global
{
	matrix g_mWVP; //ワールドから射影までの変換行列
	float4 g_PolyColor;//ポリゴン色
};

//
//バーテックスシェーダー
//
float4 VS(float4 Pos : POSITION) : SV_POSITION
{
	Pos = mul(Pos,g_mWVP);

	return Pos;
}

//
//ピクセルシェーダー
//
float4 PS(float4 Pos : SV_POSITION) : SV_Target
{
	return g_PolyColor;
}