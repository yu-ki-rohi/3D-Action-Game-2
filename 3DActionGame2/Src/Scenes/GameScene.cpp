#include "GameScene.h"
#include "../Objects/ObjectManager.h"
#include "../Collision/CollisionManager.h"
#include "../Assets/AssetsManager.h"
#include "../Objects/CameraManager.h"
#include "../Objects/ObjectFactory.h"
#include "../Collision/ColliderRegisterInterface.h"
#include "../Assets/ShaderResource.h"
#include "../Mathmatics/Vector3.h"
#include "../Common.h"
#include "../Systems/TimerFactory.h"
#include "PlayerEventNotifier.h"

#include "../Objects/Player.h"
#include "../Objects/Enemy.h"

#include "../Input/InputManager.h"

GameScene::GameScene() :
	objectManager(std::make_shared<ObjectManager>()),
	collisionManager(std::make_shared<CollisionManager>()),
	assetsManager(std::make_shared<AssetsManager>()),
	cameraManager(std::make_shared<CameraManager>()),
	colliderRegisterInterface(std::make_shared<ColliderRegisterInterface>(collisionManager)),
	objectFactory(std::make_shared<ObjectFactory>(objectManager, assetsManager, colliderRegisterInterface)),
	isReady(false),
	aSyncLoadNumMax(100),
	isJustAvoidTime(false),
	intensity(0.0f),
	lightCameraViewMatrix(MGetIdent()),
	lightCameraProjectionMatrix(MGetIdent())
{

}

GameScene::~GameScene()
{

}

void GameScene::Start()
{
	if (objectManager != nullptr)
	{
		objectManager->Start();
	}
}

void GameScene::FixedUpdate()
{
	if (collisionManager != nullptr)
	{
		collisionManager->CheckCollision();
	}

	if (objectManager != nullptr)
	{
		objectManager->FixedUpdate();
	}
}

void GameScene::Update(float elapsed_time_)
{
	if (objectManager != nullptr)
	{
		objectManager->Update(elapsed_time_);
		objectManager->Erase();
	}
}

void GameScene::Render()
{
	// シャドウマップへの描画
	SetupDepthImage();

	if (objectManager != nullptr)
	{
		objectManager->RenderShadow();
	}

	// シャドウマップにぼかしをかける
	GraphFilter(assetsManager->GetShadowMapHandle(), DX_GRAPH_FILTER_GAUSS, 8, 3000);

	// 一時スクリーンへの描画
	SetDrawScreen(assetsManager->GetTmpScreenHandle());
	//SetDrawScreen(DX_SCREEN_BACK);
	//SetBackgroundColor(0, 0, 0);
	ClearDrawScreen();
	SetCameraNearFar(1.0f, 2000.0f);

	cameraManager->SetCameraInfo();
	
	// カメラの向きはライトの向き
	/*Vector3 light_direction = Vector3::ConvertFromVECTOR(GetLightDirection());

	Vector3 target_position = cameraManager->GetNearShadowAreaPos();
	float distance = 100.0f;
	Vector3 camera_position = target_position - light_direction * distance;

	SetCameraPositionAndTargetAndUpVec(
		camera_position.ToVECTOR(),
		target_position.ToVECTOR(),
		Vector3::UP.ToVECTOR()
	);*/

	SetVSConstFMtx(44, lightCameraViewMatrix);
	SetVSConstFMtx(48, lightCameraProjectionMatrix);
	SetUseTextureToShader(2, assetsManager->GetShadowMapHandle());

	if (objectManager != nullptr)
	{
		objectManager->Render();
	}

	// 使用テクスチャの設定を解除
	SetUseTextureToShader(2, -1);

	// 設定した定数を解除
	ResetVSConstF(43, 8);

	// バックバッファへの描画
	SetDrawScreen(DX_SCREEN_BACK);
	ClearDrawScreen();

	PostProcessing();

#ifdef DEBUG
	

#endif
}

void GameScene::UpdateInLoading(float elapsed_time_)
{
	if (isReady)
	{
		InputManager::Instance().ChangeMap(InputManager::Map::Player);

		currentStep = Step::Update;
	}
	else if (GetASyncLoadNum() == 0)
	{
		GenerateObjects();
		isReady = true;
	}
}

void GameScene::RenderInLoading()
{
	const int width = WindowSettings::WindowWidth * 0.8f;
	const int height = WindowSettings::WindowHeight * 0.01f;
	int x = WindowSettings::WindowWidth * 0.1f;
	int y = WindowSettings::WindowHeight * 0.85f;
	unsigned int color = GetColor(0, 0, 255);
	unsigned int guarge_color = GetColor(200, 200, 200);
	float rate = 1.0f;
	if (aSyncLoadNumMax != 0)
	{
		rate = (aSyncLoadNumMax - GetASyncLoadNum()) / (float)aSyncLoadNumMax;
	}
	DrawBox(x, y, x + width, y + height, guarge_color, TRUE);
	DrawBox(x, y, x + width * rate, y + height, color, TRUE);
}

void GameScene::Initialize()
{
	playerEventNotifier = std::make_shared<PlayerEventNotifier>(shared_from_this());
	SetUseZBuffer3D(true);
	SetWriteZBuffer3D(true);
	SetupLight();
	MV1SetUseOrigShader(TRUE);
	ASyncLoad();

	currentStep = Step::Load;
}

SceneBase::Type GameScene::Delete()
{
	return Type::Title;
}

void GameScene::SuccessJustAvoid()
{
	if (isJustAvoidTime) { return; }

	if (objectManager)
	{
		objectManager->SetMonochrome(0.6f, ObjectBase::Tag::Stage);
		objectManager->SetLocalTimeScale(0.05f, ObjectBase::Tag::Enemy);
	}

	intensity = 0.03f;

	TimerFactory::CreateTimer<GameScene>(3.0f, this, &GameScene::FinishJustAvoid);

	isJustAvoidTime = true;
}

void GameScene::SetupLight()
{
	/*
		ライトの使用をONにする
	*/
	SetUseLighting(true);

	/*
		ライティングの設定をONにする
		この設定をONにしないとディレクショナルライト等の計算が行われない
	*/
	SetLightEnable(true);

	/*
		グローバルAmbient(環境光)の設定をする
	*/
	SetGlobalAmbientLight(GetColorF(0.6f, 0.6f, 0.6f, 1.0f));

	/*
		設定されているライトの方向設定
			※デフォルトはディレクショナルライトが設定されている
	*/
	VECTOR light_dir = VGet(0.8f, -0.7f, 0.4f);
	ChangeLightTypeDir(light_dir);

	/*
		ライトの拡散光の色を設定する
	*/
	SetLightDifColor(GetColorF(0.6f, 0.6f, 0.65f, 1.0f));

	/*
		ライトの環境光の色を設定する
	*/
	SetLightAmbColor(GetColorF(0.4f, 0.4f, 0.5f, 1.0f));
}

void GameScene::ASyncLoad()
{
	assetsManager->Load();
	// 生成手順の分の表示のため、1足しておく
	aSyncLoadNumMax = GetASyncLoadNum() + 1;
}

void GameScene::SetupDepthImage() 
{
	// 描画先を影用深度記録画像にする
	SetDrawScreen(assetsManager->GetShadowMapHandle());

	// 影用深度記録画像を真っ白にクリア
	SetBackgroundColor(255, 255, 255);
	ClearDrawScreen();
	SetBackgroundColor(0, 0, 0);


	// カメラのタイプを正射影タイプにセット、描画範囲も指定
	SetupCamera_Ortho(180.0f);

	// 描画する奥行き範囲をセット
	SetCameraNearFar(1.0f, 300.0f);

	//cameraManager->SetCameraInfo();

	// カメラの向きはライトの向き
	Vector3 light_direction = Vector3::ConvertFromVECTOR(GetLightDirection());

	Vector3 target_position = cameraManager->GetNearShadowAreaPos();
	float distance = 100.0f;
	Vector3 camera_position = target_position - light_direction * distance;

	SetCameraPositionAndTargetAndUpVec(
		camera_position.ToVECTOR(),
		target_position.ToVECTOR(),
		Vector3::UP.ToVECTOR()
	);

	// 設定したカメラのビュー行列と射影行列を取得しておく
	lightCameraViewMatrix = GetCameraViewMatrix();
	lightCameraProjectionMatrix = GetCameraProjectionMatrix();

	// 深度記録画像への描画用のピクセルシェーダーをセット
	SetUsePixelShader(assetsManager->GetPixelShader(PSKind::ShadowMap)->Handle);
}

void GameScene::PostProcessing()
{
	VERTEX2DSHADER Vert[4];
	unsigned short Index[6];


	// ２ポリゴン分の頂点のデータをセットアップ
	Vert[0].pos = VGet(0.0f, 0.0f, 0.0f);
	Vert[0].rhw = 1.0f;
	Vert[0].dif = GetColorU8(255, 255, 255, 255);
	Vert[0].spc = GetColorU8(0, 0, 0, 0);
	Vert[0].u = 0.0f;
	Vert[0].v = 0.0f;
	Vert[0].su = 0.0f;
	Vert[0].sv = 0.0f;

	Vert[1].pos = VGet(WindowSettings::WindowWidth, 0.0f, 0.0f);
	Vert[1].rhw = 1.0f;
	Vert[1].dif = GetColorU8(255, 255, 255, 255);
	Vert[1].spc = GetColorU8(0, 0, 0, 0);
	Vert[1].u = 1.0f;
	Vert[1].v = 0.0f;
	Vert[1].su = 1.0f;
	Vert[1].sv = 0.0f;

	Vert[2].pos = VGet(0.0f, WindowSettings::WindowHeight, 0.0f);
	Vert[2].rhw = 1.0f;
	Vert[2].dif = GetColorU8(255, 255, 255, 255);
	Vert[2].spc = GetColorU8(0, 0, 0, 0);
	Vert[2].u = 0.0f;
	Vert[2].v = 1.0f;
	Vert[2].su = 0.0f;
	Vert[2].sv = 1.0f;

	Vert[3].pos = VGet(WindowSettings::WindowWidth, WindowSettings::WindowHeight, 0.0f);
	Vert[3].rhw = 1.0f;
	Vert[3].dif = GetColorU8(255, 255, 255, 255);
	Vert[3].spc = GetColorU8(0, 0, 0, 0);
	Vert[3].u = 1.0f;
	Vert[3].v = 1.0f;
	Vert[3].su = 1.0f;
	Vert[3].sv = 1.0f;

	// ２ポリゴン分の頂点番号配列のセットアップ
	Index[0] = 0;
	Index[1] = 1;
	Index[2] = 2;
	Index[3] = 2;
	Index[4] = 1;
	Index[5] = 3;

	// 使用するテクスチャを０番にセット
	SetUseTextureToShader(0, assetsManager->GetTmpScreenHandle());

	// 使用するピクセルシェーダーをセット
	int ps_handle = assetsManager->GetPixelShader(PSKind::PostEffect)->Handle;
	SetUsePixelShader(ps_handle);


	FLOAT4 radial_parms = { 0.0f, 0.0f, 0.0f, intensity };
	SetPSConstF(23, radial_parms);

	// シェーダーを使用した２Ｄの２ポリゴンの描画
	DrawPolygonIndexed2DToShader(Vert, 4, Index, 2);

	// テクスチャを解除
	SetUseTextureToShader(0, -1);
}

void GameScene::GenerateObjects()
{
	auto player = objectFactory->CreatePlayer(playerEventNotifier, cameraManager);

	auto enemy = objectFactory->CreateEnemy();

	objectFactory->CreateStage();

	auto camera_tps = objectFactory->CreateCameraTPS();
	camera_tps->SetTarget(player->GetComponent<Transform>());
	cameraManager->RegisterCamera(camera_tps);
}

void GameScene::FinishJustAvoid()
{
	if (objectManager)
	{
		objectManager->SetMonochrome(0.0f, ObjectBase::Tag::Stage);
		objectManager->SetLocalTimeScale(1.0f, ObjectBase::Tag::Enemy);
	}

	intensity = 0.0f;

	isJustAvoidTime = false;
}