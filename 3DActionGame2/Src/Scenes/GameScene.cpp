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
#include "../Systems/Time.h"
#include "../Systems/MFPCFactory.h"
#include "PlayerEventNotifier.h"

#include "../Objects/Player.h"
#include "../Objects/Enemy.h"

#include "../Input/InputManager.h"
#include "../Audio/AudioManager.h"


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
	targetIntensity(0.0f),
	fluctuationSpeedOfIntensity(0.0f),
	monochromeRate(0.0f),
	targetMonochromeRate(0.0f),
	fluctuationSpeedOfMonochromeRate(0.0f),
	monochromeMask(0),
	lightCameraViewMatrix(MGetIdent()),
	lightCameraProjectionMatrix(MGetIdent())
{

}

GameScene::~GameScene()
{

}

bool GameScene::IsJustAvoidTime()
{
	return isJustAvoidTime;
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

		// ��U�x�^��
		// �l�X�g�[�������ʏ���������̂Ō�Ŏ������
		if (intensity != targetIntensity)
		{
			intensity += fluctuationSpeedOfIntensity * elapsed_time_;
			if (fluctuationSpeedOfIntensity > 0)
			{
				if (intensity > targetIntensity)
				{
					intensity = targetIntensity;
				}
			}
			else
			{
				if (intensity < targetIntensity)
				{
					intensity = targetIntensity;
				}
			}
		}

		if (monochromeRate != targetMonochromeRate)
		{
			monochromeRate += fluctuationSpeedOfMonochromeRate * elapsed_time_;
			if (fluctuationSpeedOfMonochromeRate > 0)
			{
				if (monochromeRate > targetMonochromeRate)
				{
					monochromeRate = targetMonochromeRate;
				}
			}
			else
			{
				if (monochromeRate < targetMonochromeRate)
				{
					monochromeRate = targetMonochromeRate;
				}
			}
			objectManager->SetMonochrome(monochromeRate, monochromeMask);
		}

		objectManager->Erase();
	}
}

void GameScene::Render()
{
	// �V���h�E�}�b�v�ւ̕`��
	SetupDepthImage();

	if (objectManager != nullptr)
	{
		objectManager->RenderShadow();
	}

	// �V���h�E�}�b�v�ɂڂ�����������
	GraphFilter(assetsManager->GetShadowMapHandle(), DX_GRAPH_FILTER_GAUSS, 8, 3000);

	// �ꎞ�X�N���[���ւ̕`��
	SetDrawScreen(assetsManager->GetTmpScreenHandle());
	//SetDrawScreen(DX_SCREEN_BACK);
	//SetBackgroundColor(0, 0, 0);
	ClearDrawScreen();
	SetCameraNearFar(1.0f, 2000.0f);

	cameraManager->SetCameraInfo();

	SetVSConstFMtx(44, lightCameraViewMatrix);
	SetVSConstFMtx(48, lightCameraProjectionMatrix);
	SetUseTextureToShader(2, assetsManager->GetShadowMapHandle());

	if (objectManager != nullptr)
	{
		objectManager->Render();
	}

	// �g�p�e�N�X�`���̐ݒ������
	SetUseTextureToShader(2, -1);

	// �ݒ肵���萔������
	ResetVSConstF(43, 8);

	// �o�b�N�o�b�t�@�ւ̕`��
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
		InputManager::Instance().RegisterBehave(
			InputManager::Map::Player,
			XINPUT_BUTTON_START,
			InputManager::State::Press,
			MFPCFactory::CreateMFPC(shared_from_this(), this, &GameScene::ReturnTitle)
		);
		InputManager::Instance().ChangeMap(InputManager::Map::Player);
		AudioManager::Instance().PlayMusic(BGMKind::Main);
		AudioManager::Instance().SetVolume(125, SEKind::HitSlash);

		currentStep = Step::Update;
	}
	else if (GetASyncLoadNum() == 0)
	{
		GenerateObjects();
		assetsManager->PrepareToPlayAudio();
		isReady = true;
	}
}

void GameScene::RenderInLoading()
{
	const int width = (int)(WindowSettings::WindowWidth * 0.8f);
	const int height = (int)(WindowSettings::WindowHeight * 0.01f);
	int x = (int)(WindowSettings::WindowWidth * 0.1f);
	int y = (int)(WindowSettings::WindowHeight * 0.85f);
	unsigned int color = GetColor(0, 0, 255);
	unsigned int guarge_color = GetColor(200, 200, 200);
	float rate = 1.0f;
	if (aSyncLoadNumMax != 0)
	{
		rate = (aSyncLoadNumMax - GetASyncLoadNum()) / (float)aSyncLoadNumMax;
	}
	DrawBox(x, y, x + width, y + height, guarge_color, TRUE);
	DrawBox(x, y, x + (int)(width * rate), y + height, color, TRUE);
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
	InputManager::Instance().Clear();
	return Type::Title;
}

void GameScene::SuccessJustAvoid()
{
	if (isJustAvoidTime) { return; }

	if (objectManager)
	{
		int mask = (int)ObjectBase::Tag::Stage;
		objectManager->SetMonochrome(0.9f, mask);
		mask = (int)ObjectBase::Tag::Enemy | (int)ObjectBase::Tag::Effect;
		objectManager->MultiplyLocalTimeScaleBy(JustAvoidLocalTimeScale, mask);
	}

	if (objectFactory)
	{
		objectFactory->SetIsJustAvoidTime(true);
	}

	monochromeMask = (int)ObjectBase::Tag::Stage;
	targetMonochromeRate = 0.9f;
	float changing_time = 0.05f;
	float diff_monochrome = targetMonochromeRate - monochromeRate;
	fluctuationSpeedOfMonochromeRate = diff_monochrome / changing_time;

	targetIntensity = 0.05f;
	changing_time = 0.025f;
	float diff_intensity = targetIntensity - intensity;
	fluctuationSpeedOfIntensity = diff_intensity / changing_time;

	AudioManager::Instance().SetVolume(155, BGMKind::Main);

	const float time_to_reset = 0.08f;
	const float change_monochrome_time = 2.0f;
	const float just_avoid_bonus_time = 5.0f;

	Time::TimeScale = 0.1f;
	TimerFactory::CreateTimer<GameScene>(time_to_reset, shared_from_this(), this, &GameScene::ResetTimeScale);
	TimerFactory::CreateTimer<GameScene>(time_to_reset + just_avoid_bonus_time - change_monochrome_time, shared_from_this(), this, &GameScene::FinishJustAvoidEffect);
	TimerFactory::CreateTimer<GameScene>(time_to_reset + just_avoid_bonus_time, shared_from_this(), this, &GameScene::FinishJustAvoidTime);

	isJustAvoidTime = true;
}

void GameScene::SetupLight()
{
	/*
		���C�g�̎g�p��ON�ɂ���
	*/
	SetUseLighting(true);

	/*
		���C�e�B���O�̐ݒ��ON�ɂ���
		���̐ݒ��ON�ɂ��Ȃ��ƃf�B���N�V���i�����C�g���̌v�Z���s���Ȃ�
	*/
	SetLightEnable(true);

	/*
		�O���[�o��Ambient(����)�̐ݒ������
	*/
	SetGlobalAmbientLight(GetColorF(0.6f, 0.6f, 0.6f, 1.0f));

	/*
		�ݒ肳��Ă��郉�C�g�̕����ݒ�
			���f�t�H���g�̓f�B���N�V���i�����C�g���ݒ肳��Ă���
	*/
	VECTOR light_dir = VGet(0.8f, -0.7f, 0.4f);
	ChangeLightTypeDir(light_dir);

	/*
		���C�g�̊g�U���̐F��ݒ肷��
	*/
	SetLightDifColor(GetColorF(0.6f, 0.6f, 0.65f, 1.0f));

	/*
		���C�g�̊����̐F��ݒ肷��
	*/
	SetLightAmbColor(GetColorF(0.4f, 0.4f, 0.5f, 1.0f));
}

void GameScene::ASyncLoad()
{
	assetsManager->Load();
	// �����菇�̕��̕\���̂��߁A1�����Ă���
	aSyncLoadNumMax = GetASyncLoadNum() + 1;
}

void GameScene::SetupDepthImage() 
{
	// �`�����e�p�[�x�L�^�摜�ɂ���
	SetDrawScreen(assetsManager->GetShadowMapHandle());

	// �e�p�[�x�L�^�摜��^�����ɃN���A
	SetBackgroundColor(255, 255, 255);
	ClearDrawScreen();
	SetBackgroundColor(0, 0, 0);


	// �J�����̃^�C�v�𐳎ˉe�^�C�v�ɃZ�b�g�A�`��͈͂��w��
	SetupCamera_Ortho(180.0f);

	// �`�悷�鉜�s���͈͂��Z�b�g
	SetCameraNearFar(1.0f, 300.0f);

	//cameraManager->SetCameraInfo();

	// �J�����̌����̓��C�g�̌���
	Vector3 light_direction = Vector3::ConvertFromVECTOR(GetLightDirection());

	Vector3 target_position = cameraManager->GetNearShadowAreaPos();
	float distance = 100.0f;
	Vector3 camera_position = target_position - light_direction * distance;

	SetCameraPositionAndTargetAndUpVec(
		camera_position.ToVECTOR(),
		target_position.ToVECTOR(),
		Vector3::UP.ToVECTOR()
	);

	// �ݒ肵���J�����̃r���[�s��Ǝˉe�s����擾���Ă���
	lightCameraViewMatrix = GetCameraViewMatrix();
	lightCameraProjectionMatrix = GetCameraProjectionMatrix();

	// �[�x�L�^�摜�ւ̕`��p�̃s�N�Z���V�F�[�_�[���Z�b�g
	SetUsePixelShader(assetsManager->GetPixelShader(PSKind::ShadowMap)->Handle);
}

void GameScene::PostProcessing()
{
	VERTEX2DSHADER Vert[4];
	unsigned short Index[6];


	// �Q�|���S�����̒��_�̃f�[�^���Z�b�g�A�b�v
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

	// �Q�|���S�����̒��_�ԍ��z��̃Z�b�g�A�b�v
	Index[0] = 0;
	Index[1] = 1;
	Index[2] = 2;
	Index[3] = 2;
	Index[4] = 1;
	Index[5] = 3;

	// �g�p����e�N�X�`�����O�ԂɃZ�b�g
	SetUseTextureToShader(0, assetsManager->GetTmpScreenHandle());

	// �g�p����s�N�Z���V�F�[�_�[���Z�b�g
	int ps_handle = assetsManager->GetPixelShader(PSKind::PostEffect)->Handle;
	SetUsePixelShader(ps_handle);


	FLOAT4 radial_parms = { 0.0f, 0.0f, 0.0f, intensity };
	SetPSConstF(23, radial_parms);

	// �V�F�[�_�[���g�p�����Q�c�̂Q�|���S���̕`��
	DrawPolygonIndexed2DToShader(Vert, 4, Index, 2);

	// �e�N�X�`��������
	SetUseTextureToShader(0, -1);
}

void GameScene::GenerateObjects()
{
	Vector3 initial_position = Vector3(0.0f, 0.0f, 0.0f);
	Vector3 initial_rotation = Vector3(0.0f, 180.0f, 0.0f);
	auto player = objectFactory->CreatePlayer(initial_position, initial_rotation, playerEventNotifier, cameraManager);
	
	initial_position = Vector3(0.0f, 0.0f, 50.0f);
	initial_rotation = Vector3(0.0f, 0.0f, 0.0f);

	auto enemy = objectFactory->CreateEnemy(initial_position, initial_rotation);

	objectFactory->CreateStage();

	auto camera_tps = objectFactory->CreateCameraTPS();
	camera_tps->SetTarget(player->GetComponent<Transform>());
	cameraManager->RegisterCamera(camera_tps);
}

void GameScene::ResetTimeScale()
{
	Time::TimeScale = 1.0f;
}

void GameScene::FinishJustAvoidEffect()
{
	monochromeMask = (int)ObjectBase::Tag::Stage;
	targetMonochromeRate = 0.0f;
	float changing_time = 2.0f;
	float diff_monochrome = targetMonochromeRate - monochromeRate;
	fluctuationSpeedOfMonochromeRate = diff_monochrome / changing_time;

	targetIntensity = 0.0f;
	changing_time = 2.5f;
	float diff_intensity = targetIntensity - intensity;
	fluctuationSpeedOfIntensity = diff_intensity / changing_time;


}

void GameScene::FinishJustAvoidTime()
{
	if (objectManager)
	{
		int mask = (int)ObjectBase::Tag::Stage;
		objectManager->SetMonochrome(0.0f, mask);
		mask = (int)ObjectBase::Tag::Enemy | (int)ObjectBase::Tag::Effect;
		objectManager->MultiplyLocalTimeScaleBy(1.0f / JustAvoidLocalTimeScale, mask);;
	}

	if (objectFactory)
	{
		objectFactory->SetIsJustAvoidTime(false);
	}
	intensity = 0.0f;

	AudioManager::Instance().SetVolume(255, BGMKind::Main);

	isJustAvoidTime = false;
}

void GameScene::ReturnTitle()
{
	currentStep = Step::Finish;
}