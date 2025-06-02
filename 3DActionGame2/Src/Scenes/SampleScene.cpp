#include "SampleScene.h"
#include "../Objects/ObjectManager.h"
#include "../Objects/ObjectFactory.h"
#include "../Objects/SampleCharacter.h"
#include "../Objects/SampleCamera.h"
#include "../Input/InputManager.h"
#include "../Systems/TimerManager.h"
#include "../Systems/MemberFunctionPointerContainer.h"
#include "../Mathmatics/Vector3.h"

SampleScene::SampleScene() :
	objectManager(std::make_shared<ObjectManager>()),
	objectFactory(std::make_shared<ObjectFactory>(objectManager))
{
	InputManager::Instance().RegisiterBehave(
		InputManager::Map::Menu,
		InputManager::Trigger::Right,
		InputManager::State::Hold,
		std::make_shared<MemberFunctionPointerContainer<SampleScene>>(this, &SampleScene::IncreaseIntensity));
	InputManager::Instance().RegisiterBehave(
		InputManager::Map::Menu,
		InputManager::Trigger::Left,
		InputManager::State::Hold,
		std::make_shared<MemberFunctionPointerContainer<SampleScene>>(this, &SampleScene::DecreaseIntensity));

	lightCameraProjectionMatrix = MGetIdent();
	lightCameraViewMatrix = MGetIdent();
}

SampleScene::~SampleScene()
{
	DeleteHandles();
}


void SampleScene::FixedUpdate()
{
	if (objectManager != nullptr)
	{
		objectManager->FixedUpdate();
	}
}

void SampleScene::SampleScene::Update(float elapsed_time_)
{
	if (objectManager != nullptr)
	{
		objectManager->Update(elapsed_time_);
	}
}

void SampleScene::SampleScene::Render()
{
	SetupDepthImage();

	if (objectManager != nullptr)
	{
		objectManager->RenderShadow();
	}

	GraphFilter(shadowMapHandle00, DX_GRAPH_FILTER_GAUSS, 8, 3000);

	SetDrawScreen(tmpScreenHandle);
	ClearDrawScreen();
	SetCameraNearFar(1.0f, 2000.0f);

	sampleCamera->SetCameraInfo();

	SetVSConstFMtx(44, lightCameraViewMatrix);
	SetVSConstFMtx(48, lightCameraProjectionMatrix);
	SetUseTextureToShader(2, shadowMapHandle00);

	if (objectManager != nullptr)
	{
		objectManager->Render();
	}

	// �g�p�e�N�X�`���̐ݒ������
	SetUseTextureToShader(2, -1);

	// �ݒ肵���萔������
	ResetVSConstF(43, 8);

	SetDrawScreen(DX_SCREEN_BACK);


	// �e�X�g�I�ɍ���Ă���̂Œ�����
	// �{�Ԃ͈ȉ����֐��ɂ���
	
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
	SetUseTextureToShader(0, tmpScreenHandle);

	// �g�p����s�N�Z���V�F�[�_�[���Z�b�g
	SetUsePixelShader(blurPSHandle);


	FLOAT4 radial_parms = { 0.0f, 0.0f, 0.0f, intensity };
	SetPSConstF(23, radial_parms);

	// �V�F�[�_�[���g�p�����Q�c�̂Q�|���S���̕`��
	DrawPolygonIndexed2DToShader(Vert, 4, Index, 2);
	SetUseTextureToShader(0, -1);

#ifdef DEBUG
	DrawFormatString(0, 50, GetColor(255, 255, 255), "Intensity : %f", intensity);
	DrawExtendGraph(10, 80, 90, 160, shadowMapHandle00, TRUE);
	//TestDrawShadowMap(shadowMapHandle00, 10, 80, 90, 160);
#endif

}

void SampleScene::SampleScene::Initialize()
{
	Load();

	SetUseZBuffer3D(true);
	SetWriteZBuffer3D(true);

	SetupLight();

	SetCameraNearFar(0.1f, 20.0f);


	MV1SetUseOrigShader(TRUE);

	if (objectFactory != nullptr)
	{
		// �J�����쐬
		sampleCamera = objectFactory->CreateCamera(Vector3(0.0f, 10.0f, 0.0f));

		// �X�e�[�W�쐬
		objectFactory->Create(Vector3(), stageHandle, normalVertexShaderHandle, pixelShaderHandle, skinnedShadowMapVSHandle);

		// �L�����쐬
		sampleChara = objectFactory->CreateCharacter(Vector3(), Vector3(0.1f, 0.1f, 0.1f), charaHandle, skinnedVertexShaderHandle, pixelShaderHandle, skinnedShadowMapVSHandle, idleHandle, sampleCamera);
		std::shared_ptr<SampleCharacter> sampleEnemy = objectFactory->CreateCharacter(Vector3(20.0f, 0.0f, 10.0f), Vector3(0.15f, 0.15f, 0.15f), enemyHandle, skinnedVertexShaderHandle, pixelShaderHandle, skinnedShadowMapVSHandle, idleHandle, nullptr);

		sampleChara->RegisterAnim(Animator::AnimType::Attack00, attackHandle);
		sampleChara->RegisterAnim(Animator::AnimType::WalkF, walkHandle);

		sampleEnemy->RegisterAnim(Animator::AnimType::Attack00, attackHandle);
		sampleEnemy->RegisterAnim(Animator::AnimType::WalkF, walkHandle);

	}

#ifdef DEBUG


#endif

	currentStep = Step::Update;
}

SceneBase::Type SampleScene::SampleScene::Delete()
{
	InputManager::Instance().Clear();
	TimerManager::Instance().EraseAll();

	DeleteHandles();

	return nextType;
}

void SampleScene::SetupLight()
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
	VECTOR light_dir = VGet(0.8f, -0.5f, 0.4f);
	ChangeLightTypeDir(light_dir);

	/*
		���C�g�̊g�U���̐F��ݒ肷��
	*/
	SetLightDifColor(GetColorF(0.7f, 0.7f, 0.8f, 1.0f));

	/*
		���C�g�̊����̐F��ݒ肷��
	*/
	SetLightAmbColor(GetColorF(0.4f, 0.4f, 0.5f, 1.0f));

}

void SampleScene::Load()
{
	// TODO
	// File��
	stageHandle = MV1LoadModel("Res/Models/Stage/StageA01.mv1");
	charaHandle = MV1LoadModel("Res/Models/Player/Player_Model.mv1");
	idleHandle = MV1LoadModel("Res/Models/Player/Player_Idle.mv1");
	walkHandle = MV1LoadModel("Res/Models/Player/Player_Walk.mv1");
	attackHandle = MV1LoadModel("Res/Models/Player/Player_Attack1.mv1");
	enemyHandle = MV1LoadModel("Res/Models/Enemy/Enemy_Model.mv1");

	normalVertexShaderHandle = LoadVertexShader("Res/Shader/Test/TestVertex07.vso");
	skinnedVertexShaderHandle = LoadVertexShader("Res/Shader/Test/TestVertex08.vso");
	/*normalPixelShaderHandle = LoadPixelShader("Res/Shader/Test/TestPixel00.pso");
	skinnedPixelShaderHandle = LoadPixelShader("Res/Shader/Test/TestPixel03.pso");*/
	pixelShaderHandle = LoadPixelShader("Res/Shader/Test/TestPixel05.pso");

	normalShadowMapVSHandle = LoadVertexShader("Res/Shader/Test/TestShadowMapVS01.vso");
	skinnedShadowMapVSHandle = LoadVertexShader("Res/Shader/Test/TestShadowMapVS00.vso");
	shadowMapPSHandle = LoadPixelShader("Res/Shader/Test/TestShadowMapPS00.pso");

	blurPSHandle = LoadPixelShader("Res/Shader/Test/RadialBlurPS.pso");

	tmpScreenHandle = MakeScreen(WindowSettings::WindowWidth, WindowSettings::WindowHeight, FALSE);

	// �쐬����摜�̃t�H�[�}�b�g��s�������_�^�łP�`�����l���A�P�U�r�b�g�ɂ���
	SetDrawValidFloatTypeGraphCreateFlag(TRUE);
	SetCreateDrawValidGraphChannelNum(1);
	SetCreateGraphColorBitDepth(16);
	shadowMapHandle00 = MakeScreen(2048, 2048, FALSE);

	// �ݒ�����ɖ߂�
	SetDrawValidFloatTypeGraphCreateFlag(FALSE);
	SetCreateDrawValidGraphChannelNum(4);
	SetCreateGraphColorBitDepth(32);
	
}

void SampleScene::DeleteHandles()
{
	int handles[] = {
		stageHandle,
		charaHandle,
		idleHandle,
		walkHandle,
		attackHandle,
		enemyHandle
	};

	for (auto handle : handles)
	{
		MV1DeleteModel(handle);
	}

	InitShader();
	
	DeleteGraph(tmpScreenHandle);
	DeleteGraph(shadowMapHandle00);
}

void SampleScene::IncreaseIntensity()
{
	intensity += 0.001f;
	if (intensity > 1.0f) { intensity = 1.0f; }
}

void SampleScene::DecreaseIntensity()
{
	intensity -= 0.001f;
	if (intensity < 0.0f) { intensity = 0.0f; }
}

void SampleScene::SetupDepthImage()
{
	// �`�����e�p�[�x�L�^�摜�ɂ���
	SetDrawScreen(shadowMapHandle00);

	// �e�p�[�x�L�^�摜��^�����ɃN���A
	//SetBackgroundColor(255, 255, 255);
	ClearDrawScreen();
	SetBackgroundColor(0, 0, 0);


	// �J�����̃^�C�v�𐳎ˉe�^�C�v�ɃZ�b�g�A�`��͈͂��w��
	SetupCamera_Ortho(200.0f);

	// �`�悷�鉜�s���͈͂��Z�b�g
	SetCameraNearFar(1.0f, 500.0f);

	//sampleCamera->SetCameraInfo();

	// �J�����̌����̓��C�g�̌���
	Vector3 light_direction = Vector3::ConvertFromVECTOR(GetLightDirection());

	Vector3 target_position = sampleCamera->GetNearShadowAreaPos();
	Vector3 camera_position = target_position - light_direction * 100.0f;

	Vector3 up = Vector3::UP;
	SetCameraPositionAndTargetAndUpVec(
		camera_position.ToVECTOR(),
		target_position.ToVECTOR(),
		up.ToVECTOR()
	);

	// �ݒ肵���J�����̃r���[�s��Ǝˉe�s����擾���Ă���
	lightCameraViewMatrix = GetCameraViewMatrix();
	lightCameraProjectionMatrix = GetCameraProjectionMatrix();

	// �[�x�L�^�摜�ւ̕`��p�̃s�N�Z���V�F�[�_�[���Z�b�g
	SetUsePixelShader(shadowMapPSHandle);

}