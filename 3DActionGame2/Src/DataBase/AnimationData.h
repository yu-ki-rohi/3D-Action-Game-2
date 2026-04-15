#pragma once
#include <utility>
#include <memory>
#include "DataKind.h"

#define ANIMATION_NUM 13

// ファイル名とかの管理はもっといいやり方を模索したい
// csvからの読み込みやVBAを使って生成するなど
struct AnimationFileList
{
	using AnimationList = std::pair<MKind, std::pair<AKind, const char*>>;
	AnimationList AnimationFiles[ANIMATION_NUM] =
	{
		std::make_pair(MKind::Player,std::make_pair(AKind::Idle,"Res/Models/Player/Player_Idle.mv1")),
		std::make_pair(MKind::Player,std::make_pair(AKind::WalkF,"Res/Models/Player/Player_Walk.mv1")),
		std::make_pair(MKind::Player,std::make_pair(AKind::Run,"Res/Models/Player/Player_Dash.mv1")),
		std::make_pair(MKind::Player,std::make_pair(AKind::Avoid,"Res/Models/Player/Player_Roll.mv1")),
		std::make_pair(MKind::Player,std::make_pair(AKind::Attack00,"Res/Models/Player/Player_Attack1.mv1")),
		std::make_pair(MKind::Player,std::make_pair(AKind::Attack01,"Res/Models/Player/Player_Attack5.mv1")),
		std::make_pair(MKind::Player,std::make_pair(AKind::Attack02,"Res/Models/Player/Player_Attack3.mv1")),
		std::make_pair(MKind::Player,std::make_pair(AKind::Attack03,"Res/Models/Player/Player_Attack4.mv1")),
		std::make_pair(MKind::Enemy,std::make_pair(AKind::Idle,"Res/Models/Enemy/Enemy_Idle.mv1")),
		std::make_pair(MKind::Enemy,std::make_pair(AKind::Attack00,"Res/Models/Enemy/Enemy_Attack1.mv1")),
		std::make_pair(MKind::Enemy,std::make_pair(AKind::Attack01,"Res/Models/Enemy/Enemy_Attack2.mv1")),
		std::make_pair(MKind::Enemy,std::make_pair(AKind::Attack02,"Res/Models/Enemy/Enemy_Attack3.mv1")),
		std::make_pair(MKind::Enemy,std::make_pair(AKind::WalkF,"Res/Models/Enemy/Enemy_WalkF.mv1"))
	};
};

struct AnimationParametersList
{
	using AnimationList = std::pair < MKind, std::pair < AKind, std::shared_ptr<AnimationParameters> >> ;
	AnimationList AnimationsParameters[ANIMATION_NUM] = 
	{
	#pragma region Player

		std::make_pair(MKind::Player,std::make_pair(AKind::Idle, 
		std::make_shared<AnimationParameters>(
			5.0f,
			0.3f,
			0.0f,
			0.3f,
			0.0f,
			0.0f,
			0.0f,
			0.0f,
			true
		))),
	std::make_pair(MKind::Player,std::make_pair(AKind::WalkF,
		std::make_shared<AnimationParameters>(
			1.0f,
			0.3f,
			0.0f,
			0.3f,
			0.0f,
			0.0f,
			0.0f,
			0.0f,
			true
		))),
	std::make_pair(MKind::Player,std::make_pair(AKind::Run,
		std::make_shared<AnimationParameters>(
			1.0f,
			0.3f,
			0.0f,
			0.3f,
			0.0f,
			0.0f,
			0.0f,
			0.0f,
			true
		))),
	std::make_pair(MKind::Player,std::make_pair(AKind::Avoid,
		std::make_shared<AnimationParameters>(
			0.8f,
			0.15f,
			0.6f,
			0.2f,
			0.0f,
			0.0f,
			0.0f,
			1.2f,
			false
		))),
	std::make_pair(MKind::Player,std::make_pair(AKind::Attack00,
		std::make_shared<AnimationParameters>(
			0.9f,
			0.3f,
			0.7f,
			0.3f,
			0.0f,
			0.0f,
			0.0f,
			0.0f,
			false
		))),
	std::make_pair(MKind::Player,std::make_pair(AKind::Attack01,
		std::make_shared<AnimationParameters>(
			1.0f,
			0.3f,
			0.7f,
			0.3f,
			0.0f,
			0.0f,
			0.0f,
			0.0f,
			false
		))),
	std::make_pair(MKind::Player,std::make_pair(AKind::Attack02,
		std::make_shared<AnimationParameters>(
			1.1f,
			0.3f,
			0.7f,
			0.3f,
			0.0f,
			0.0f,
			0.0f,
			0.0f,
			false
		))),
	std::make_pair(MKind::Player,std::make_pair(AKind::Attack03,
		std::make_shared<AnimationParameters>(
			1.65f,
			0.3f,
			0.7f,
			0.3f,
			0.0f,
			0.0f,
			0.0f,
			0.0f,
			false
		))),
	#pragma endregion
	#pragma region Enemy
	std::make_pair(MKind::Enemy,std::make_pair(AKind::Idle,
		std::make_shared<AnimationParameters>(
			1.0f,
			1.0f,
			0.0f,
			1.0f,
			0.0f,
			0.0f,
			0.0f,
			0.0f,
			true
		))),
	std::make_pair(MKind::Enemy,std::make_pair(AKind::Attack00,
		std::make_shared<AnimationParameters>(
			2.2f,
			0.3f,
			0.7f,
			0.3f,
			0.0f,
			0.0f,
			0.0f,
			0.0f,
			false
		))),
	std::make_pair(MKind::Enemy,std::make_pair(AKind::Attack01,
		std::make_shared<AnimationParameters>(
			2.7f,
			0.3f,
			0.7f,
			0.3f,
			0.0f,
			0.0f,
			0.0f,
			0.0f,
			false
		))),
	std::make_pair(MKind::Enemy,std::make_pair(AKind::Attack02,
		std::make_shared<AnimationParameters>(
			2.4f,
			0.3f,
			0.7f,
			0.3f,
			0.0f,
			0.0f,
			0.0f,
			0.0f,
			false
		))),
	std::make_pair(MKind::Enemy,std::make_pair(AKind::WalkF,
		std::make_shared<AnimationParameters>(
			1.0f,
			0.3f,
			0.0f,
			0.3f,
			0.0f,
			0.0f,
			0.0f,
			0.0f,
			true
		)))
	};
	#pragma endregion
};