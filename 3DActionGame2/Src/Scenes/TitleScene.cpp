#include "TitleScene.h"
#include "../Objects/ObjectManager.h"
#include "../Objects/ObjectFactory.h"
#include "../Input/InputManager.h"
#include "../Systems/TimerManager.h"
#include "../Systems/MFPCFactory.h"
#include "../Systems/SimpleObserver.h"
#include "../Common.h"
#include <DxLib.h>

// ��U�������ׂ��������Ă���̂ŁA������������؂蕪���Ă�������

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
	if (state == State::Title)
	{
		int title_x, title_y;
		unsigned int color;
		title_x = WindowSettings::WindowWidth * 0.4f;
		title_y = WindowSettings::WindowHeight * 0.3f;
		color = GetColor(255, 255, 255);

		DrawString(title_x, title_y, "3D Action Game 2", color);

		int choice_x, choice_y;

		choice_x = WindowSettings::WindowWidth * 0.4f;
		choice_y = WindowSettings::WindowHeight * 0.6f;
		int space = 30;

		DrawString(choice_x, choice_y, "Game Start", color);
		DrawString(choice_x, choice_y + space, "Option", color);
		DrawString(choice_x - space, choice_y + space * choicesIndex, "->", color);
	}
	else
	{
		int choice_x, choice_y;
		unsigned int color = GetColor(255, 255, 255);

		choice_x = WindowSettings::WindowWidth * 0.3f;
		choice_y = WindowSettings::WindowHeight * 0.2f;
		int space = 30;
		const char* option_text[optionChoicesNum] = {
			"Flip Camera rotation up and down",
			"Flip Camera rotation left and right",
			"Swap the OK and CANCEL buttons",
			"Attack Button",
			"Avoid Button"
		};
		// �e�L�X�g
		for (int i = 0; i < optionChoicesNum; i++)
		{
			DrawString(choice_x, choice_y + space * i, option_text[i], color);
		}
		DrawString(choice_x - space, choice_y + space * choicesIndex, "->", color);

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
		// ��U�ׂ�����
		DrawString(choice_x, choice_y + space * 3, button_name[InputManager::Instance().GetConfig(KeyConfig::Tag::Attack)], color);
		DrawString(choice_x, choice_y + space * 4, button_name[InputManager::Instance().GetConfig(KeyConfig::Tag::Avoid)], color);
	}
}

void TitleScene::UpdateInLoading(float elapsed_time_)
{

}

void TitleScene::RenderInLoading()
{

}

void TitleScene::TitleScene::Initialize()
{
	InputManager::Instance().ChangeMap(InputManager::Map::Menu);

	RegisterInputBehavior();

	currentStep = Step::Update;
}

SceneBase::Type TitleScene::TitleScene::Delete()
{
	InputManager::Instance().Clear();
	TimerManager::Instance().EraseAll();
	return Type::Game;
}

void TitleScene::OnDicede()
{
	if (state == State::Title)
	{
		switch (choicesIndex)
		{
		case 0:	// Game Start
			currentStep = Step::Finish;
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
		case 0:	// �J�����㉺���]
			InputManager::Instance().SwitchIsReverseVertical();
			break;
		case 1:	// �J�������E���]
			InputManager::Instance().SwitchIsReverseHorizontal();
			break;
		case 2:	// ����A�L�����Z���̔��]
			InputManager::Instance().SwitchConfigDecideAndCancel();
			InputManager::Instance().Clear();
			RegisterInputBehavior();
			break;
		case 3:	// �U���{�^��
			configTag = KeyConfig::Tag::Attack;
			InputManager::Instance().ChangeMap(InputManager::Map::Player);
			break;
		case 4:	// ����{�^��
			configTag = KeyConfig::Tag::Avoid;
			InputManager::Instance().ChangeMap(InputManager::Map::Player);
			break;
		case 5:	// �_�b�V���{�^��
			configTag = KeyConfig::Tag::Dash;
			InputManager::Instance().ChangeMap(InputManager::Map::Player);
			break;
		}
	}
}
void TitleScene::OnCancel()
{
	if (state == State::Option)
	{
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
}

void TitleScene::DecreaseMenuIndex()
{
	choicesIndex--;
	if (choicesIndex < 0)
	{
		choicesIndex = choicesNum - 1;
	}
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
}

void TitleScene::RegisterInputBehavior()
{

	// Menu�ւ̑���o�^
	// L�X�e�B�b�N
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
	// ������{�^��
	InputManager::Instance().RegisterBehave(
		InputManager::Map::Menu,
		XINPUT_BUTTON_DPAD_UP,
		InputManager::State::Press,
		MFPCFactory::CreateMFPC(shared_from_this(), this, &TitleScene::DecreaseMenuIndex)
	);
	// �������{�^��
	InputManager::Instance().RegisterBehave(
		InputManager::Map::Menu,
		XINPUT_BUTTON_DPAD_DOWN,
		InputManager::State::Press,
		MFPCFactory::CreateMFPC(shared_from_this(), this, &TitleScene::IncreaseMenuIndex)
	);
	// ����
	InputManager::Instance().RegisterBehave(
		InputManager::Map::Menu,
		KeyConfig::Tag::Decide,
		InputManager::State::Press,
		MFPCFactory::CreateMFPC(shared_from_this(), this, &TitleScene::OnDicede)
	);
	// �L�����Z��
	InputManager::Instance().RegisterBehave(
		InputManager::Map::Menu,
		KeyConfig::Tag::Cancel,
		InputManager::State::Press,
		MFPCFactory::CreateMFPC(shared_from_this(), this, &TitleScene::OnCancel)
	);

	// �R���t�B�O���蓖�Ăŗ��p�\�ȃ{�^���̐�
	// Start, Back�̃{�^���̓R���t�B�O�̊��蓖�Ă���O��
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
		// Start, Back�̃{�^���̓X�L�b�v
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