#pragma once
#include <utility>
#include "DataKind.h"

// ファイル名とかの管理はもっといいやり方を模索したい
// csvからの読み込みやVBAを使って生成するなど
struct ModelFileList
{
	using ModelList = std::pair<MKind, const char*>;

	ModelList ModelFiles[3] = {
		std::make_pair(MKind::Player, "Res/Models/Player/Player_Model.mv1"),
		std::make_pair(MKind::Enemy, "Res/Models/Enemy/Enemy_Model.mv1"),
		std::make_pair(MKind::Stage, "Res/Models/Stage/StageA01.mv1")
	};
};