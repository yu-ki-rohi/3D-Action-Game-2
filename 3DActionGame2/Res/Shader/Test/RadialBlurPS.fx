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

inline float InterleavedGradientNoise(float2 pixCoord)
{
    const float3 magic = float3(0.06711056f, 0.00583715f, 52.9829189f);
    float2 frameMagicScale = float2(2.083f, 4.867f);
    pixCoord += frameMagicScale;
    return frac(magic.z * frac(dot(pixCoord, magic.xy)));
}

sampler ScreenTexture : register( s0 )  ;
float4 RadialParams   : register( c23 ) ;

PS_OUTPUT main( PS_INPUT PSInput )
{
    PS_OUTPUT PSOutput ;
    float4    TextureColor ;
    float     SampleCount ;
    float     RcpSampleCount ;
    float     Intensity ;
    float     Dither ;
    float2    Center ;

    Center = float2( 0.5f, 0.5f ) ;

    Dither = InterleavedGradientNoise(PSInput.TexCoords0.xy) ;
    Dither = 0.0f ;

    SampleCount = 4.0f ;
    RcpSampleCount = 0.25f ;
    Intensity = RadialParams.w ;

    for (int i = 0; i < SampleCount; i++)
    {
        float t = (i + Dither) * RcpSampleCount;
        TextureColor += tex2D( ScreenTexture, Center + (PSInput.TexCoords0.xy - Center) * lerp(1, 1 - Intensity, t)) ;
    }
    TextureColor *= RcpSampleCount ;

    PSOutput.Color0 = TextureColor ;
    return PSOutput ;
}