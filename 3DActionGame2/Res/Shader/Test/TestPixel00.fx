// ピクセルシェーダーの入力
struct PS_INPUT
{
	float2 TexCoords0      : TEXCOORD0 ;
} ;

// ピクセルシェーダーの出力
struct PS_OUTPUT
{
	float4 Color0          : COLOR0 ;
} ;


// C++ 側で設定するテクスチャの定義
sampler  DiffuseMapTexture             : register( s0 ) ;		// ディフューズマップテクスチャ
float4   cfFactorColor                 : register( c5 ) ;		// 不透明度等
float4   monochromeRate				   : register( c22 ) ;


// main関数
PS_OUTPUT main( PS_INPUT PSInput )
{
	PS_OUTPUT PSOutput ;
	float4 TextureDiffuseColor ;

	// テクスチャカラーの読み込み
	TextureDiffuseColor = tex2D( DiffuseMapTexture, PSInput.TexCoords0 ) ;

	// 出力カラー = テクスチャカラー
	PSOutput.Color0 = TextureDiffuseColor ;

	float Y = 0.299f * PSOutput.Color0.r + 0.587f * PSOutput.Color0.g + 0.114f * PSOutput.Color0.b ;

	float3 monochromeColor = float3( Y, Y, Y) ;

	PSOutput.Color0.xyz = lerp(PSOutput.Color0, monochromeColor, monochromeRate.x) ;

	// 出力アルファ = テクスチャアルファ * 不透明度
	PSOutput.Color0.a = TextureDiffuseColor.a * cfFactorColor.a ;

	// 出力パラメータを返す
	return PSOutput ;
}