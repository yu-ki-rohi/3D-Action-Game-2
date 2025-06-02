// ピクセルシェーダーの入力
struct PS_INPUT
{
	float2 TexCoords0      : TEXCOORD0 ;    // テクスチャ座標
	float4 LPPosition      : TEXCOORD1 ;    // ライトからみた座標( xとyはライトの射影座標、zはビュー座標 )
	float3 VPosition       : TEXCOORD2 ;    // 座標( ビュー空間 )
	float3 VNormal         : TEXCOORD3 ;    // 法線( ビュー空間 )
} ;

// ピクセルシェーダーの出力
struct PS_OUTPUT
{
	float4 Color0          : COLOR0 ;
} ;

// マテリアルパラメータ
struct MATERIAL
{
	float4 Diffuse ;      // ディフューズカラー
	float4 Specular ;     // スペキュラカラー
	float4 Power ;        // スペキュラの強さ
} ;

// ライトパラメータ
struct LIGHT
{
	float4 Position ;               // 座標( ビュー空間 )
	float3 Direction ;              // 方向( ビュー空間 )
	float4 Diffuse ;                // ディフューズカラー
	float4 Specular ;               // スペキュラカラー
	float4 Ambient ;                // アンビエントカラーとマテリアルのアンビエントカラーを乗算したもの
	float4 Range_FallOff_AT0_AT1 ;  // x:有効距離  y:スポットライト用FallOff  z:距離による減衰処理用パラメータ０  w:距離による減衰処理用パラメータ１
	float4 AT2_SpotP0_SpotP1 ;      // x:距離による減衰処理用パラメータ２  y:スポットライト用パラメータ０( cos( Phi / 2.0f ) )  z:スポットライト用パラメータ１( 1.0f / ( cos( Theta / 2.0f ) - cos( Phi / 2.0f ) ) )
} ;


// C++ 側で設定するテクスチャや定数の定義
sampler  DiffuseMapTexture             : register( s0 ) ;		// ディフューズマップテクスチャ
sampler  SpecularMapTexture            : register( s1 ) ;		// スペキュラーマップテクスチャ
sampler  DepthMapTexture               : register( s2 ) ;		// 深度バッファテクスチャ
float4   cfAmbient_Emissive            : register( c1 ) ;		// エミッシブカラー + マテリアルアンビエントカラー * グローバルアンビエントカラー
MATERIAL cfMaterial                    : register( c2 ) ;		// マテリアルパラメータ
float4   cfFactorColor                 : register( c5 ) ;		// 不透明度等
float4   monochromeRate				   : register( c22 ) ;      // モノクロ率
LIGHT    cfLight[ 1 ]                  : register( c32 ) ;		// ライトパラメータ


// main関数
PS_OUTPUT main( PS_INPUT PSInput )
{
	PS_OUTPUT PSOutput ;
	float4 TextureDiffuseColor ;
	float4 SpecularColor ;
    float4 SpecularPower ;
	float3 Normal ;
	float DiffuseAngleGen ;
	float4 TotalDiffuse ;
	float4 TotalSpecular ;
	float3 TempF3 ;
	float Temp ;
	float3 V_to_Eye ;
	float3 lLightDir ;
	float  TextureDepth ;
	float2 DepthTexCoord ;


	// 法線の準備
	Normal = normalize( PSInput.VNormal ) ;

	// 頂点座標から視点へのベクトルを正規化
	V_to_Eye = normalize( -PSInput.VPosition ) ;

	// ディフューズカラーとスペキュラカラーの蓄積値を初期化
	TotalDiffuse  = float4( 0.0f, 0.0f, 0.0f, 0.0f ) ;
	TotalSpecular = float4( 0.0f, 0.0f, 0.0f, 0.0f ) ;


	// ディレクショナルライトの処理 +++++++++++++++++++++++++++++++++++++++++++++++++++++( 開始 )

	// ライト方向ベクトルのセット
	lLightDir = cfLight[ 0 ].Direction ;

	// ディフューズ色計算

	// DiffuseAngleGen = ディフューズ角度減衰率計算
	DiffuseAngleGen = saturate( dot( Normal, -lLightDir ) ) ;

	// ディフューズカラー蓄積値 += ライトのディフューズカラー * マテリアルのディフューズカラー * ディフューズカラー角度減衰率 + ライトのアンビエントカラーとマテリアルのアンビエントカラーを乗算したもの 
	TotalDiffuse += cfLight[ 0 ].Diffuse * cfMaterial.Diffuse * DiffuseAngleGen + cfLight[ 0 ].Ambient ;


	// スペキュラカラー計算

	// ハーフベクトルの計算
	TempF3 = normalize( V_to_Eye - lLightDir ) ;

	// Temp = pow( max( 0.0f, N * H ), cfMaterial.Power.x )
	Temp = pow( max( 0.0f, dot( Normal, TempF3 ) ), cfMaterial.Power.x ) ;

	// スペキュラカラー蓄積値 += Temp * ライトのスペキュラカラー
	TotalSpecular += Temp * cfLight[ 0 ].Specular ;

	// ディレクショナルライトの処理 +++++++++++++++++++++++++++++++++++++++++++++++++++++( 終了 )



	// 出力カラー計算 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++( 開始 )

	// TotalDiffuse = ライトディフューズカラー蓄積値 + ( マテリアルのアンビエントカラーとグローバルアンビエントカラーを乗算したものとマテリアルエミッシブカラーを加算したもの )
	TotalDiffuse += cfAmbient_Emissive ;

	// SpecularColor = ライトのスペキュラカラー蓄積値 * マテリアルのスペキュラカラー
	SpecularColor = TotalSpecular * cfMaterial.Specular ;
	SpecularPower = tex2D( SpecularMapTexture, PSInput.TexCoords0.xy ) ;
    SpecularColor *= SpecularPower.x ;
    

	// 出力カラー = TotalDiffuse * テクスチャカラー + SpecularColor
	TextureDiffuseColor = tex2D( DiffuseMapTexture, PSInput.TexCoords0.xy ) ;
    TextureDiffuseColor.rgb *= TotalDiffuse.rgb + SpecularColor.rgb ;

	// 深度テクスチャの座標を算出
	// PSInput.LPPosition.xy は -1.0f ～ 1.0f の値なので、これを 0.0f ～ 1.0f の値にする
	DepthTexCoord.x = ( PSInput.LPPosition.x + 1.0f ) / 2.0f;

	// yは更に上下反転
	DepthTexCoord.y = 1.0f - ( PSInput.LPPosition.y + 1.0f ) / 2.0f;

	if(DepthTexCoord.x >= 0.0f && DepthTexCoord.x <= 1.0f && 
	   DepthTexCoord.y >= 0.0f && DepthTexCoord.y <= 1.0f)
	   {
			// 深度バッファテクスチャから深度を取得
			TextureDepth = tex2D( DepthMapTexture, DepthTexCoord );

			// テクスチャに記録されている深度( +補正値 )よりＺ値が大きかったら奥にあるということで輝度を減らす
			if( PSInput.LPPosition.z > TextureDepth + 0.8f )
			{
				TextureDiffuseColor.rgb *= 0.75f;
			}


	   }

	
     // モノクロ変換
    float Y = 0.299f * TextureDiffuseColor.r + 0.587f * TextureDiffuseColor.g + 0.114f * TextureDiffuseColor.b ;
	float3 monochromeColor = float3( Y, Y, Y) ;
	TextureDiffuseColor.rgb = lerp(TextureDiffuseColor, monochromeColor, monochromeRate.x) ;

	// 出力カラーをセット
	PSOutput.Color0 = TextureDiffuseColor;

    // アルファ値 = テクスチャアルファ * マテリアルのディフューズアルファ * 不透明度
	PSOutput.Color0.a = TextureDiffuseColor.a * cfMaterial.Diffuse.a * cfFactorColor.a ;

	// 出力カラー計算 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++( 終了 )


	// 出力パラメータを返す
	return PSOutput ;
}