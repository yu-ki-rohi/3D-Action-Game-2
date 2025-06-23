#pragma once
#include <utility>
#include "DataKind.h"

// �t�@�C�����Ƃ��̊Ǘ��͂����Ƃ���������͍�������
// csv����̓ǂݍ��݂�VBA���g���Đ�������Ȃ�
struct AnimationFileList
{
	using AnimationList = std::pair<MKind, std::pair<AKind, const char*>>;
	AnimationList AnimationFiles[6] =
	{
		std::make_pair(MKind::Player,std::make_pair(AKind::Idle,"Res/Models/Player/Player_Idle.mv1")),
		std::make_pair(MKind::Player,std::make_pair(AKind::WalkF,"Res/Models/Player/Player_Walk.mv1")),
		std::make_pair(MKind::Player,std::make_pair(AKind::Avoid,"Res/Models/Player/Player_Roll.mv1")),
		std::make_pair(MKind::Player,std::make_pair(AKind::Attack00,"Res/Models/Player/Player_Attack1.mv1")),
		std::make_pair(MKind::Enemy,std::make_pair(AKind::Idle,"Res/Models/Enemy/Enemy_Idle.mv1")),
		std::make_pair(MKind::Enemy,std::make_pair(AKind::Attack00,"Res/Models/Enemy/Enemy_Attack1.mv1"))
	};
};
