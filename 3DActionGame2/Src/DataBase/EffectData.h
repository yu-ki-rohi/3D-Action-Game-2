#pragma once
#include <utility>
#include "DataKind.h"

// 先生から頂いたソースより
// Atlas(統合)テクスチャ
struct AtlasTexture
{
	const char* fileName;	// ファイル名
	int num;				// 分割数
	int xNum;				// X軸の分割数
	int yNum;				// Y軸の分割数
	int width;				// X軸の分割サイズ
	int height;				// Y軸の分割サイズ

	AtlasTexture(const char* file_name_, int num_, int x_num_, int y_num_, int width_, int height_) :
		fileName(file_name_),
		num(num_),
		xNum(x_num_),
		yNum(y_num_),
		width(width_),
		height(height_) {}
};

// ファイル名とかの管理はもっといいやり方を模索したい
// csvからの読み込みやVBAを使って生成するなど
struct EffectFileList
{
	using EffectList = std::pair<EKind, AtlasTexture>;
	EffectList  EffectFiles[1] =
	{
		std::make_pair(
			EKind::HitSlash,
			AtlasTexture(
				"Res/Effect/tktk_Impact_33.png",	// ファイル名
				12,							// 統合画像内の画像数
				5,							// 横軸の数
				3,							// 縦軸の数
				192,						// 横軸のサイズ(一枚分)
				192							// 縦軸のサイズ(一枚分)
		))
	};
};
