// 頂点シェーダーの入力
struct VS_INPUT
{
	float4 Position        : POSITION ;         // 座標( ローカル空間 )
	int4   BlendIndices0   : BLENDINDICES0 ;    // スキニング処理用 Float型定数配列インデックス
	int4   BlendIndices1   : BLENDINDICES1 ;    // スキニング処理用 Float型定数配列インデックス
	float4 BlendWeight0    : BLENDWEIGHT0 ;     // スキニング処理用ウエイト値
	float4 BlendWeight1    : BLENDWEIGHT1 ;     // スキニング処理用ウエイト値
	float3 Normal          : NORMAL0 ;          // 法線( ローカル空間 )
	float4 Diffuse         : COLOR0 ;           // ディフューズカラー
	float4 Specular        : COLOR1 ;           // スペキュラカラー
	float4 TexCoords0      : TEXCOORD0 ;        // テクスチャ座標
} ;

// 頂点シェーダーの出力
struct VS_OUTPUT
{
	float4 Position        : POSITION ;
	float2 TexCoords0      : TEXCOORD0 ;
} ;



// C++ 側で設定する定数の定義
float4              cfProjectionMatrix[ 4 ]   : register( c2  ) ;		// ビュー　　→　射影行列
float4              cfViewMatrix[ 3 ]         : register( c6  ) ;		// ワールド　→　ビュー行列
float4              cfTextureMatrix[ 3 ][ 2 ] : register( c88 ) ;		// テクスチャ座標操作用行列
float4              cfLocalWorldMatrix[ 162 ] : register( c94 ) ;		// ローカル　→　ワールド行列




// main関数
VS_OUTPUT main( VS_INPUT VSInput )
{
	VS_OUTPUT VSOutput ;
	float4 lLocalWorldMatrix[ 3 ] ;
	float4 lWorldPosition ;
	float4 lViewPosition ;


	// 複数のフレームのブレンド行列の作成
	lLocalWorldMatrix[ 0 ]  = cfLocalWorldMatrix[ VSInput.BlendIndices0.x + 0 ] * VSInput.BlendWeight0.x;
	lLocalWorldMatrix[ 1 ]  = cfLocalWorldMatrix[ VSInput.BlendIndices0.x + 1 ] * VSInput.BlendWeight0.x;
	lLocalWorldMatrix[ 2 ]  = cfLocalWorldMatrix[ VSInput.BlendIndices0.x + 2 ] * VSInput.BlendWeight0.x;

	lLocalWorldMatrix[ 0 ] += cfLocalWorldMatrix[ VSInput.BlendIndices0.y + 0 ] * VSInput.BlendWeight0.y;
	lLocalWorldMatrix[ 1 ] += cfLocalWorldMatrix[ VSInput.BlendIndices0.y + 1 ] * VSInput.BlendWeight0.y;
	lLocalWorldMatrix[ 2 ] += cfLocalWorldMatrix[ VSInput.BlendIndices0.y + 2 ] * VSInput.BlendWeight0.y;

	lLocalWorldMatrix[ 0 ] += cfLocalWorldMatrix[ VSInput.BlendIndices0.z + 0 ] * VSInput.BlendWeight0.z;
	lLocalWorldMatrix[ 1 ] += cfLocalWorldMatrix[ VSInput.BlendIndices0.z + 1 ] * VSInput.BlendWeight0.z;
	lLocalWorldMatrix[ 2 ] += cfLocalWorldMatrix[ VSInput.BlendIndices0.z + 2 ] * VSInput.BlendWeight0.z;

	lLocalWorldMatrix[ 0 ] += cfLocalWorldMatrix[ VSInput.BlendIndices0.w + 0 ] * VSInput.BlendWeight0.w;
	lLocalWorldMatrix[ 1 ] += cfLocalWorldMatrix[ VSInput.BlendIndices0.w + 1 ] * VSInput.BlendWeight0.w;
	lLocalWorldMatrix[ 2 ] += cfLocalWorldMatrix[ VSInput.BlendIndices0.w + 2 ] * VSInput.BlendWeight0.w;

	lLocalWorldMatrix[ 0 ] += cfLocalWorldMatrix[ VSInput.BlendIndices1.x + 0 ] * VSInput.BlendWeight1.x;
	lLocalWorldMatrix[ 1 ] += cfLocalWorldMatrix[ VSInput.BlendIndices1.x + 1 ] * VSInput.BlendWeight1.x;
	lLocalWorldMatrix[ 2 ] += cfLocalWorldMatrix[ VSInput.BlendIndices1.x + 2 ] * VSInput.BlendWeight1.x;

	lLocalWorldMatrix[ 0 ] += cfLocalWorldMatrix[ VSInput.BlendIndices1.y + 0 ] * VSInput.BlendWeight1.y;
	lLocalWorldMatrix[ 1 ] += cfLocalWorldMatrix[ VSInput.BlendIndices1.y + 1 ] * VSInput.BlendWeight1.y;
	lLocalWorldMatrix[ 2 ] += cfLocalWorldMatrix[ VSInput.BlendIndices1.y + 2 ] * VSInput.BlendWeight1.y;

	lLocalWorldMatrix[ 0 ] += cfLocalWorldMatrix[ VSInput.BlendIndices1.z + 0 ] * VSInput.BlendWeight1.z;
	lLocalWorldMatrix[ 1 ] += cfLocalWorldMatrix[ VSInput.BlendIndices1.z + 1 ] * VSInput.BlendWeight1.z;
	lLocalWorldMatrix[ 2 ] += cfLocalWorldMatrix[ VSInput.BlendIndices1.z + 2 ] * VSInput.BlendWeight1.z;

	lLocalWorldMatrix[ 0 ] += cfLocalWorldMatrix[ VSInput.BlendIndices1.w + 0 ] * VSInput.BlendWeight1.w;
	lLocalWorldMatrix[ 1 ] += cfLocalWorldMatrix[ VSInput.BlendIndices1.w + 1 ] * VSInput.BlendWeight1.w;
	lLocalWorldMatrix[ 2 ] += cfLocalWorldMatrix[ VSInput.BlendIndices1.w + 2 ] * VSInput.BlendWeight1.w;



	// 頂点座標変換 ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++( 開始 )

	// ローカル座標をワールド座標に変換
	lWorldPosition.x = dot( VSInput.Position, lLocalWorldMatrix[ 0 ] ) ;
	lWorldPosition.y = dot( VSInput.Position, lLocalWorldMatrix[ 1 ] ) ;
	lWorldPosition.z = dot( VSInput.Position, lLocalWorldMatrix[ 2 ] ) ;
	lWorldPosition.w = 1.0f ;

	// ワールド座標をビュー座標に変換
	lViewPosition.x = dot( lWorldPosition, cfViewMatrix[ 0 ] ) ;
	lViewPosition.y = dot( lWorldPosition, cfViewMatrix[ 1 ] ) ;
	lViewPosition.z = dot( lWorldPosition, cfViewMatrix[ 2 ] ) ;
	lViewPosition.w = 1.0f ;

	// ビュー座標を射影座標に変換
	VSOutput.Position.x = dot( lViewPosition, cfProjectionMatrix[ 0 ] ) ;
	VSOutput.Position.y = dot( lViewPosition, cfProjectionMatrix[ 1 ] ) ;
	VSOutput.Position.z = dot( lViewPosition, cfProjectionMatrix[ 2 ] ) ;
	VSOutput.Position.w = dot( lViewPosition, cfProjectionMatrix[ 3 ] ) ;

	// 頂点座標変換 ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++( 終了 )



	// 出力パラメータセット ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++( 開始 )

	// テクスチャ座標変換行列による変換を行った結果のテクスチャ座標をセット
	VSOutput.TexCoords0.x = dot( VSInput.TexCoords0, cfTextureMatrix[ 0 ][ 0 ] ) ;
	VSOutput.TexCoords0.y = dot( VSInput.TexCoords0, cfTextureMatrix[ 0 ][ 1 ] ) ;

	// 出力パラメータセット ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++( 終了 )


	// 出力パラメータを返す
	return VSOutput ;
}