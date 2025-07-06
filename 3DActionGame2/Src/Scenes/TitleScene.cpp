#include "TitleScene.h"
#include "../Objects/ObjectManager.h"
#include "../Objects/ObjectFactory.h"
#include "../Input/InputManager.h"
#include "../Systems/TimerManager.h"
#include "../Systems/TimerFactory.h"
#include "../Systems/MFPCFactory.h"
#include "../Systems/SimpleObserver.h"
#include "../Common.h"
#include "../DataBase/DataKind.h"
#include "../Audio/AudioManager.h"
#include "../Assets/AudioResource.h"
#include <DxLib.h>

// 一旦処理を雑にベタ書きしているので、今後もう少し整理・切り分けをしていきたい

TitleScene::TitleScene() :
	choicesIndex(0),
	choicesNum(titleChoicesNum),
	state(State::Title),
	configTag(KeyConfig::Tag::Attack)
{

}

TitleScene::~TitleScene()
{

}


void TitleScene::FixedUpdate()
{
	
}

void TitleScene::TitleScene::Update(float elapsed_time_)
{
	
}

void TitleScene::TitleScene::Render()
{
	// 一旦ベタ書き
	if (state == State::Title)
	{
		int title_x, title_y;
		unsigned int color;
		title_x = (int)(WindowSettings::WindowWidth * 0.4f);
		title_y = (int)(WindowSettings::WindowHeight * 0.3f);
		color = GetColor(255, 255, 255);

		DrawString(title_x, title_y, "3D Action Game 2", color);

		int choice_x, choice_y;

		choice_x = (int)(WindowSettings::WindowWidth * 0.4f);
		choice_y = (int)(WindowSettings::WindowHeight * 0.6f);
		int space = 30;

		DrawString(choice_x, choice_y, "Game Start", color);
		DrawString(choice_x, choice_y + space, "Option", color);
		DrawString(choice_x - space, choice_y + space * choicesIndex, "->", color);
	}
	else
	{
		int choice_x, choice_y;
		unsigned int color = GetColor(255, 255, 255);

		choice_x = (int)(WindowSettings::WindowWidth * 0.3f);
		choice_y = (int)(WindowSettings::WindowHeight * 0.2f);
		int space = 30;
		const char* option_text[optionChoicesNum] = {
			"Flip Camera rotation up and down",
			"Flip Camera rotation left and right",
			"Swap the OK and CANCEL buttons",
			"Attack Button",
			"Avoid Button",
			"Return"
		};
		// テキスト
		for (int i = 0; i < optionChoicesNum; i++)
		{
			DrawString(choice_x, choice_y + space * i, option_text[i], color);
		}
		DrawString(choice_x - space, choice_y + space * choicesIndex, "->", color);

		// 値
		choice_x += 350;
		using InputManagerBoolFunc = bool(InputManager::*)();
		const int bool_value_num = 3;
		InputManagerBoolFunc input_manager_is_xxx[bool_value_num] =
		{
			&InputManager::IsReverseVertical,
			&InputManager::IsReverseHorizontal,
			&InputManager::IsSwitchedConfigDecideAndCancel
		};

		for (int i = 0; i < bool_value_num; i++)
		{
			if ((InputManager::Instance().*input_manager_is_xxx[i])())
			{
				DrawString(choice_x, choice_y + space * i, "Invert", color);
			}
			else
			{
				DrawString(choice_x, choice_y + space * i, "Normal", color);
			}
		}

		const char* button_name[BUTTON_NUM] =
		{
			"DPAD_UP",
			"DPAD_DOWN",
			"DPAD_LEFT",
			"DPAD_RIGHT",
			"START",
			"BACK",
			"LEFT_THUMB",
			"RIGHT_THUMB",
			"LEFT_SHOULDER",
			"RIGHT_SHOULDER",
			"LEFT_TRIGGER",
			"RIGHT_TRIGGER",
			"A",
			"B",
			"X",
			"Y",
		};
		
		DrawString(choice_x, choice_y + space * 3, button_name[InputManager::Instance().GetConfig(KeyConfig::Tag::Attack)], color);
		DrawString(choice_x, choice_y + space * 4, button_name[InputManager::Instance().GetConfig(KeyConfig::Tag::Avoid)], color);
	}
}

void TitleScene::UpdateInLoading(float elapsed_time_)
{
	// 一旦ベタ書き
	const int sound_num = 3;
	std::pair<SEKind, int> kind_and_handles[sound_num] = {
		std::make_pair<SEKind, int>(SEKind::SystemDecide,LoadSoundMem("Res/Audio/Sound/maou_se_system37.mp3")),
		std::make_pair<SEKind, int>(SEKind::SystemCancel,LoadSoundMem("Res/Audio/Sound/maou_se_system19.mp3")),
		std::make_pair<SEKind, int>(SEKind::SystemSelect,LoadSoundMem("Res/Audio/Sound/maou_se_system48.mp3"))
	};

	auto se_map = std::make_shared<std::unordered_map<SEKind, std::shared_ptr<SoundResource>>>();

	for (const auto& kind_and_handle : kind_and_handles)
	{
		auto sound_resource = std::make_shared<SoundResource>();
		sound_resource->Handle = kind_and_handle.second;
		(*se_map)[kind_and_handle.first] = sound_resource;
	}

	AudioManager::Instance().RegisterSound(se_map);

	AudioManager::Instance().SetVolume(125, SEKind::SystemDecide);
	AudioManager::Instance().SetVolume(125, SEKind::SystemCancel);
	AudioManager::Instance().SetVolume(125, SEKind::SystemSelect);

	currentStep = Step::Update;
}

void TitleScene::RenderInLoading()
{

}

void TitleScene::TitleScene::Initialize()
{
	InputManager::Instance().ChangeMap(InputManager::Map::Menu);

	RegisterInputBehavior();

	currentStep = Step::Load;
}

SceneBase::Type TitleScene::TitleScene::Delete()
{
	InputManager::Instance().Clear();
	TimerManager::Instance().EraseAll();

	InitSoundMem();
	AudioManager::Instance().DestructResource();
	return Type::Game;
}

void TitleScene::FinishScene()
{
	currentStep = Step::Finish;
}

void TitleScene::OnDicede()
{
	if (choicesIndex < 5)
	{
		AudioManager::Instance().PlaySoundEffect(SEKind::SystemDecide);
	}

	if (state == State::Title)
	{
		switch (choicesIndex)
		{
		case 0:	// Game Start
			InputManager::Instance().ChangeMap(InputManager::Map::None);
			TimerFactory::CreateTimer(1.0f, shared_from_this(), this, &TitleScene::FinishScene);
			break;
		case 1:	// Option
			state = State::Option;
			choicesIndex = 0;
			choicesNum = optionChoicesNum;
			break;
		}
	}
	else if (state == State::Option)
	{
		switch (choicesIndex)
		{
		case 0:	// カメラ上下反転
			InputManager::Instance().SwitchIsReverseVertical();
			break;
		case 1:	// カメラ左右反転
			InputManager::Instance().SwitchIsReverseHorizontal();
			break;
		case 2:	// 決定、キャンセルの反転
			InputManager::Instance().SwitchConfigDecideAndCancel();
			InputManager::Instance().Clear();
			RegisterInputBehavior();
			break;
		case 3:	// 攻撃ボタン
			configTag = KeyConfig::Tag::Attack;
			InputManager::Instance().ChangeMap(InputManager::Map::Player);
			break;
		case 4:	// 回避ボタン
			configTag = KeyConfig::Tag::Avoid;
			InputManager::Instance().ChangeMap(InputManager::Map::Player);
			break;
		case 5: //戻る
			OnCancel();
			break;
		}
	}
}
void TitleScene::OnCancel()
{
	if (state == State::Option)
	{
		AudioManager::Instance().PlaySoundEffect(SEKind::SystemCancel);
		state = State::Title;
		choicesIndex = 0;
		choicesNum = titleChoicesNum;
	}
}
void TitleScene::ChangeMenuIndex()
{
	if (!so) { return; }
	float x, y;
	so->GetFloatx2(x, y);

	if (y > 0.0f)
	{
		DecreaseMenuIndex();
	}
	else
	{
		IncreaseMenuIndex();
	}
}

void TitleScene::IncreaseMenuIndex()
{
	choicesIndex++;
	choicesIndex %= choicesNum;
	AudioManager::Instance().PlaySoundEffect(SEKind::SystemSelect);
}

void TitleScene::DecreaseMenuIndex()
{
	choicesIndex--;
	if (choicesIndex < 0)
	{
		choicesIndex = choicesNum - 1;
	}
	AudioManager::Instance().PlaySoundEffect(SEKind::SystemSelect);
}


void TitleScene::SetKeyConfigDpadUp()
{
	SetKeyConfig(XINPUT_BUTTON_DPAD_UP);
}
void TitleScene::SetKeyConfigDpadDown()
{
	SetKeyConfig(XINPUT_BUTTON_DPAD_DOWN);
}
void TitleScene::SetKeyConfigDpadLeft()
{
	SetKeyConfig(XINPUT_BUTTON_DPAD_LEFT);
}
void TitleScene::SetKeyConfigDpadRight()
{
	SetKeyConfig(XINPUT_BUTTON_DPAD_RIGHT);
}
void TitleScene::SetKeyConfigLeftThumb()
{
	SetKeyConfig(XINPUT_BUTTON_LEFT_THUMB);
}
void TitleScene::SetKeyConfigRightThumb()
{
	SetKeyConfig(XINPUT_BUTTON_RIGHT_THUMB);
}
void TitleScene::SetKeyConfigLeftShoulder()
{
	SetKeyConfig(XINPUT_BUTTON_LEFT_SHOULDER);
}
void TitleScene::SetKeyConfigRightShoulder()
{
	SetKeyConfig(XINPUT_BUTTON_RIGHT_SHOULDER);
}
void TitleScene::SetKeyConfigA()
{
	SetKeyConfig(XINPUT_BUTTON_A);
}
void TitleScene::SetKeyConfigB()
{
	SetKeyConfig(XINPUT_BUTTON_B);
}
void TitleScene::SetKeyConfigX()
{
	SetKeyConfig(XINPUT_BUTTON_X);
}
void TitleScene::SetKeyConfigY()
{
	SetKeyConfig(XINPUT_BUTTON_Y);
}
void TitleScene::SetKeyConfigLeftTrigger()
{
	SetKeyConfig(XINPUT_BUTTON_LEFT_TRIGGER);
}
void TitleScene::SetKeyConfigRightTrigger()
{
	SetKeyConfig(XINPUT_BUTTON_RIGHT_TRIGGER);
}

void TitleScene::SetKeyConfig(unsigned char button_)
{
	InputManager::Instance().SetConfig(configTag, button_);
	InputManager::Instance().ChangeMap(InputManager::Map::Menu); 
	AudioManager::Instance().PlaySoundEffect(SEKind::SystemDecide);
}

void TitleScene::RegisterInputBehavior()
{

	// Menuへの操作登録
	// Lスティック
	so = std::make_shared<SimpleObserver>();
	InputManager::Instance().AddObserver(
		InputManager::Stick::Left,
		InputManager::Map::Menu,
		so
	);
	InputManager::Instance().RegisterBehave(
		InputManager::Map::Menu,
		InputManager::Stick::Left,
		InputManager::State::Press,
		MFPCFactory::CreateMFPC(shared_from_this(), this, &TitleScene::ChangeMenuIndex)
	);
	// 方向上ボタン
	InputManager::Instance().RegisterBehave(
		InputManager::Map::Menu,
		XINPUT_BUTTON_DPAD_UP,
		InputManager::State::Press,
		MFPCFactory::CreateMFPC(shared_from_this(), this, &TitleScene::DecreaseMenuIndex)
	);
	// 方向下ボタン
	InputManager::Instance().RegisterBehave(
		InputManager::Map::Menu,
		XINPUT_BUTTON_DPAD_DOWN,
		InputManager::State::Press,
		MFPCFactory::CreateMFPC(shared_from_this(), this, &TitleScene::IncreaseMenuIndex)
	);
	// 決定
	InputManager::Instance().RegisterBehave(
		InputManager::Map::Menu,
		KeyConfig::Tag::Decide,
		InputManager::State::Press,
		MFPCFactory::CreateMFPC(shared_from_this(), this, &TitleScene::OnDicede)
	);
	// キャンセル
	InputManager::Instance().RegisterBehave(
		InputManager::Map::Menu,
		KeyConfig::Tag::Cancel,
		InputManager::State::Press,
		MFPCFactory::CreateMFPC(shared_from_this(), this, &TitleScene::OnCancel)
	);

	// コンフィグ割り当てで利用可能なボタンの数
	// Start, Backのボタンはコンフィグの割り当てから外す
	const int available_button_num = BUTTON_NUM + TRIGGER_NUM - 2;

	using MemberFuncPointer = void (TitleScene::*)();
	MemberFuncPointer regiseter_functions[available_button_num] = {
		&TitleScene::SetKeyConfigDpadUp,
		&TitleScene::SetKeyConfigDpadDown,
		&TitleScene::SetKeyConfigDpadLeft,
		&TitleScene::SetKeyConfigDpadRight,
		&TitleScene::SetKeyConfigLeftThumb,
		&TitleScene::SetKeyConfigRightThumb,
		&TitleScene::SetKeyConfigLeftShoulder,
		&TitleScene::SetKeyConfigRightShoulder,
		&TitleScene::SetKeyConfigLeftTrigger,
		&TitleScene::SetKeyConfigRightTrigger,
		&TitleScene::SetKeyConfigA,
		&TitleScene::SetKeyConfigB,
		&TitleScene::SetKeyConfigX,
		&TitleScene::SetKeyConfigY
	};

	for (unsigned char i = 0; i < available_button_num - 2; i++)
	{
		unsigned char button = i;
		// Start, Backのボタンはスキップ
		if (button >= XINPUT_BUTTON_START)
		{
			button += 2;
		}
		InputManager::Instance().RegisterBehave(
			InputManager::Map::Player,
			button,
			InputManager::State::Press,
			MFPCFactory::CreateMFPC(shared_from_this(), this, regiseter_functions[i])
		);
	}
}