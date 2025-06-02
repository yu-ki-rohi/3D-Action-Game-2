// ピクセルシェーダーの入力
struct PS_INPUT
{
	float4 PPosition       : TEXCOORD0 ;        // 座標( ビュー空間 )
} ;

// ピクセルシェーダーの出力
struct PS_OUTPUT
{
	float4 Color0          : COLOR0 ;
} ;

// main関数
PS_OUTPUT main( PS_INPUT PSInput )
{
	PS_OUTPUT PSOutput ;

	// Ｚ値を色として出力
	PSOutput.Color0 = PSInput.PPosition.z;

	// 透明にならないようにアルファは必ず１
	PSOutput.Color0.a = 1.0f;

	// 出力パラメータを返す
	return PSOutput ;
}

