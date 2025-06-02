// ピクセルシェーダーの入力
struct PS_INPUT
{
	float2 TexCoords0      : TEXCOORD0 ;    // テクスチャ座標
} ;

// ピクセルシェーダーの出力
struct PS_OUTPUT
{
	float4 Color0          : COLOR0 ;
} ;

sampler ScreenTexture : register( s0 )  ;

PS_OUTPUT main( PS_INPUT PSInput )
{
    PS_OUTPUT PSOutput ;
    float4    TextureColor ;

    TextureColor = tex2D( ScreenTexture, PSInput.TexCoords0.xy ) ;

    PSOutput.Color0 = TextureColor ;
    PSOutput.Color0.a = 1.0f ;

    return PSOutput ;
}