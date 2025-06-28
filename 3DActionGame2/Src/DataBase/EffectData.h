#pragma once
#include <utility>
#include "DataKind.h"

// �搶���璸�����\�[�X���
// Atlas(����)�e�N�X�`��
struct AtlasTexture
{
	const char* fileName;	// �t�@�C����
	int num;				// ������
	int xNum;				// X���̕�����
	int yNum;				// Y���̕�����
	int width;				// X���̕����T�C�Y
	int height;				// Y���̕����T�C�Y

	AtlasTexture(const char* file_name_, int num_, int x_num_, int y_num_, int width_, int height_) :
		fileName(file_name_),
		num(num_),
		xNum(x_num_),
		yNum(y_num_),
		width(width_),
		height(height_) {}
};

// �t�@�C�����Ƃ��̊Ǘ��͂����Ƃ���������͍�������
// csv����̓ǂݍ��݂�VBA���g���Đ�������Ȃ�
struct EffectFileList
{
	using EffectList = std::pair<EKind, AtlasTexture>;
	EffectList  EffectFiles[1] =
	{
		std::make_pair(
			EKind::HitSlash,
			AtlasTexture(
				"Res/Effect/tktk_Impact_33.png",	// �t�@�C����
				12,							// �����摜���̉摜��
				5,							// �����̐�
				3,							// �c���̐�
				192,						// �����̃T�C�Y(�ꖇ��)
				192							// �c���̃T�C�Y(�ꖇ��)
		))
	};
};
