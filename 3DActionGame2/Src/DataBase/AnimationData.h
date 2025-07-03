#pragma once
#include <utility>
#include "DataKind.h"

#define ANIMATION_NUM 12

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
		std::make_pair(MKind::Player,std::make_pair(AKind::Attack02,"Res/Models/Player/Player_Attack3.mv1")),
		std::make_pair(MKind::Player,std::make_pair(AKind::Attack03,"Res/Models/Player/Player_Attack4.mv1")),
		std::make_pair(MKind::Player,std::make_pair(AKind::Attack01,"Res/Models/Player/Player_Attack5.mv1")),
		std::make_pair(MKind::Enemy,std::make_pair(AKind::Idle,"Res/Models/Enemy/Enemy_Idle.mv1")),
		std::make_pair(MKind::Enemy,std::make_pair(AKind::Attack00,"Res/Models/Enemy/Enemy_Attack1.mv1")),
		std::make_pair(MKind::Enemy,std::make_pair(AKind::Attack01,"Res/Models/Enemy/Enemy_Attack2.mv1")),
		std::make_pair(MKind::Enemy,std::make_pair(AKind::Attack02,"Res/Models/Enemy/Enemy_Attack3.mv1"))
	};
};

struct AnimationSpeedList
{
	using AnimationList = std::pair<MKind, std::pair<AKind, float>>;
	AnimationList AnimationSpeeds[ANIMATION_NUM] =
	{
	std::make_pair(MKind::Player,std::make_pair(AKind::Idle, 30.0f)),
	std::make_pair(MKind::Player,std::make_pair(AKind::WalkF, 30.0f)),
	std::make_pair(MKind::Player,std::make_pair(AKind::Run, 30.0f)),
	std::make_pair(MKind::Player,std::make_pair(AKind::Avoid, 35.0f)),
	std::make_pair(MKind::Player,std::make_pair(AKind::Attack00, 35.0f)),
	std::make_pair(MKind::Player,std::make_pair(AKind::Attack02, 45.0f)),
	std::make_pair(MKind::Player,std::make_pair(AKind::Attack03, 42.0f)),
	std::make_pair(MKind::Player,std::make_pair(AKind::Attack01, 42.0f)),
	std::make_pair(MKind::Enemy,std::make_pair(AKind::Idle, 30.0f)),
	std::make_pair(MKind::Enemy,std::make_pair(AKind::Attack00, 30.0f)),
	std::make_pair(MKind::Enemy,std::make_pair(AKind::Attack01, 30.0f)),
	std::make_pair(MKind::Enemy,std::make_pair(AKind::Attack02, 30.0f))
	};
};