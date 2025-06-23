#pragma once
#include <utility>
#include "DataKind.h"

// �t�@�C�����Ƃ��̊Ǘ��͂����Ƃ���������͍�������
// csv����̓ǂݍ��݂�VBA���g���Đ�������Ȃ�
struct ModelFileList
{
	using ModelList = std::pair<MKind, const char*>;

	ModelList ModelFiles[3] = {
		std::make_pair(MKind::Player, "Res/Models/Player/Player_Model.mv1"),
		std::make_pair(MKind::Enemy, "Res/Models/Enemy/Enemy_Model.mv1"),
		std::make_pair(MKind::Stage, "Res/Models/Stage/StageA01.mv1")
	};
};