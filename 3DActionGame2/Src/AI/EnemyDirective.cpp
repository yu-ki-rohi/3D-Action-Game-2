#include "EnemyDirective.h"
#include "../Mathmatics/Vector3.h"
#include "../Debug/DebugMessenger.h"
#include "WorldBlackboard.h"
#include "../Objects/Components/Transform.h"

// directives[n]内のビット割り当て内訳
#define DIRECTION_BIT_NUM 4
#define COMBAT_RANGE_BIT_NUM 2
#define ACTION_BIT_NUM 2


#define SIN_TABLE_NUM 5
// SIN_A_B: sin(A / B * pi)の値
#define SIN_0_8 0.0000000
#define SIN_1_8 0.3826834
#define SIN_2_8 0.7071067
#define SIN_3_8 0.9238795
#define SIN_4_8 1.0000000

namespace
{
	// 待機距離の値
	constexpr float inFightDistance = 15.0f;
	constexpr float closeRangeDistance = 50.0f;
	constexpr float middleRangeDistance = 70.0f;
	constexpr float outRangeDistance = 120.0f;

	// 検討事項：マスクの定義の仕方
	enum Mask
	{
		NONE = 0x00,
		DIRECTION = 0x0f,	// 方位指定部分
		COMBAT_RANGE = 0x30,	// 距離指定部分
		ACTION = 0xc0,	// 行動内容指定部分
		ALL = 0xff
	};
}

using namespace EnemyAI;

EnemyDirective::EnemyDirective(std::shared_ptr<const WorldBlackboard> world_blackboard_, std::shared_ptr<EnemyReportHandler> report_handler_) :
	worldBrackbord(world_blackboard_),
	reportHandler(report_handler_)
{

}

unsigned char EnemyDirective::MakeDirective(int direction_, CombatRange range_, Actions action_)
{
	unsigned char direction = static_cast<unsigned char>(direction_) & Mask::DIRECTION;
	unsigned char range = (static_cast<unsigned char>(range_) << DIRECTION_BIT_NUM) & Mask::COMBAT_RANGE;
	unsigned char action = static_cast<unsigned char>(action_) << (DIRECTION_BIT_NUM + COMBAT_RANGE_BIT_NUM);
	return direction | range | action;
}

unsigned char EnemyDirective::MakeDirective(DirectiveInfo directive_info_)
{
	return EnemyDirective::MakeDirective(directive_info_.Direction, directive_info_.Range, directive_info_.Action);
}

Vector3 EnemyDirective::GetDirectedPosition(int id_) const
{
	return GetDirectedRelativePositionToPlayer(id_) + GetPlayerPosition();
}

namespace
{
	void ToDirection(float& x, float& z, unsigned char directive);

	void ToClockPosition(float& x, float& z, const Vector3& position_of_representative_enemy, const Vector3& player_position);

	void ReflectDistance(float& x, float& z, unsigned char directive);
}

Vector3 EnemyDirective::GetDirectedRelativePositionToPlayer(int id_) const
{
	if (id_ < 0 || id_ >= directives.size())
	{
		DebugMessenger::LogError("配列の範囲外が指定されました");
		return Vector3::ZERO;
	}
	if (worldBrackbord == nullptr)
	{
		DebugMessenger::LogError("worldBrackbordがnullptrです");
		return Vector3::ZERO;
	}

	// 現状ステージ内に高低差が存在しないので、yは0.0固定
	float x, y = 0.0f, z;

	// 方位変換
	ToDirection(x, z, directives[id_]);

	// 基準となるエネミーのTransformがあるなら、プレイヤーからそのエネミーへの向きのクロックポジションに変換
	if (transformOfRepresentativeEnemy != nullptr)
	{
		ToClockPosition(x, z, transformOfRepresentativeEnemy->Position, GetPlayerPosition());
	}

	// 距離反映
	ReflectDistance(x, z, directives[id_]);

	return Vector3(x, y, z);
}

namespace
{
	void ToDirection(float& x, float& z, unsigned char directive)
	{
		// 三角関数を使う方が実装が楽かつ変更させやすいが、今回は別の方法を検討
		// 現状は高々数パターンしか存在せず、また今後ここの分割を増やすとは思えないため
		// 三角関数でいうと下記のような結果にしたい
		// x = sin(2 * pi / pow(2, DIRECTION_BIT_NUM) * direction);	z = cos((2 * pi / pow(2, DIRECTION_BIT_NUM) * direction) 

		// 【方針】
		// xz平面について考えたとき(z北向き、x東向きで考える)
		// 原点の周囲16方位に対して北から時計回りの順番で0～15の番号を振る
		// すると方位を表す4bitのうち、上位2bitは下記のような対応になる
		// 北東領域：00,	南東領域：01,	南西領域：10,	北西領域：11
		// ※但し北は00、東は01、南は10、西は11とする
		// 半径1の円のとの交点座標を0～pi/2の間のsinで表す
		// 並びと符号を上手く対応させる

		// 方位情報の抜き出し
		unsigned char direction = directive & Mask::DIRECTION;

		// 符号の抽出
		float sign = static_cast<float>(direction >> (DIRECTION_BIT_NUM - 1));
		sign = sign * 2.0f * -1.0f + 1.0f;	// 0 → 1, 1 → -1 に変換

		// 方位情報から符号情報を除去
		direction &= ~(1 << (DIRECTION_BIT_NUM - 1));

		// sin cos の反転 and zの符号反転 を行うか判定するためのbitを抜き出し
		unsigned char invert = direction >> (DIRECTION_BIT_NUM - 2);

		// 方位情報から上記情報を除去
		direction &= ~(1 << (DIRECTION_BIT_NUM - 2));

		float sin_table[SIN_TABLE_NUM] = {
			SIN_0_8,
			SIN_1_8,
			SIN_2_8,
			SIN_3_8,
			SIN_4_8,
		};


		// 方位情報をセット
		switch (invert)
		{
		case 0:
			x = sin_table[direction];
			z = sin_table[SIN_TABLE_NUM - 1 - direction];
			break;

		case 1:
			x = sin_table[SIN_TABLE_NUM - 1 - direction];
			z = -sin_table[direction];
			break;

		default:
			x = 0;
			z = 0;
		}
		// ※sin_tableの範囲保証について
		// これが範囲内であること <=> directionの値が4以下であること は 下記の2点に依拠している
		//	・DIRECT_BIT_NUM が 4 であること
		//	・ここまでの処理で方位指定部の上位2bitを0にしていること
		// 以上のことから direction の上位6bitは 0 であり、従って最大値は3である

		// 符号反映
		x *= sign;
		z *= sign;
	}

	void ToClockPosition(float& x, float& z, const Vector3& position_of_representative_enemy, const Vector3& player_position)
	{
		Vector3 base = (position_of_representative_enemy - player_position).Normalize();
		float tmp_x = x;
		float tmp_z = z;

		z = base.z * tmp_z + base.x * tmp_x;
		x = -base.z * tmp_x + base.x * tmp_z;
	}

	void ReflectDistance(float& x, float& z, unsigned char directive)
	{
		// 距離情報の抜き出し
		unsigned char range = directive & Mask::COMBAT_RANGE;
		range >>= DIRECTION_BIT_NUM;

		float distance = 0.0f;
		// 検討事項：このキャストの妥当性
		switch ((CombatRange)range)
		{
		case CombatRange::IN_FIGHT:
			distance = inFightDistance;
			break;
		case CombatRange::CLOSE_RANGE:
			distance = closeRangeDistance;
			break;

		case CombatRange::MIDDLE_RANGE:
			distance = middleRangeDistance;
			break;

		case CombatRange::OUT_RANGE:
			distance = outRangeDistance;
			break;
		}

		x *= distance;
		z *= distance;
	}
}


Actions EnemyDirective::GetDirectedAciton(int id_) const
{
	if (id_ < 0 || id_ >= directives.size())
	{
		DebugMessenger::LogError("配列の範囲外が指定されました");
		return Actions::STAND_BY;
	}

	// 検討事項：このキャストの妥当性
	return static_cast<Actions>(directives[id_] >> (DIRECTION_BIT_NUM + COMBAT_RANGE_BIT_NUM));
}

Vector3 EnemyDirective::GetPlayerPosition() const
{
	return worldBrackbord->GetPlayerPosition();
}

int EnemyDirective::GetDirectivesNum() const
{
	return static_cast<int>(directives.size());
}

float EnemyDirective::GetMaximumDistance() const
{
	float buffer = 30.0f;
	return outRangeDistance + buffer;
}

void EnemyDirective::SetDirective(int id_, unsigned char directive_)
{
	if (id_ < 0 || id_ >= directives.size())
	{
		DebugMessenger::LogError("配列の範囲外が指定されました");
		return;
	}
	directives[id_] = directive_;
}


void EnemyDirective::SetDirective(int id_, int direction_, CombatRange range_, Actions action_)
{
	SetDirective(id_, MakeDirective(direction_, range_, action_));
}

void EnemyDirective::SetDirective(int id_, DirectiveInfo directive_info_)
{
	SetDirective(id_, MakeDirective(directive_info_));
}

void EnemyDirective::SetDirective(int id_, int direction_)
{
	if (id_ < 0 || id_ >= directives.size())
	{
		DebugMessenger::LogError("配列の範囲外が指定されました");
		return;
	}
	directives[id_] &= ~Mask::DIRECTION;
	directives[id_] |= static_cast<unsigned char>(direction_);
}

void EnemyDirective::SetDirective(int id_, CombatRange range_)
{
	if (id_ < 0 || id_ >= directives.size())
	{
		DebugMessenger::LogError("配列の範囲外が指定されました");
		return;
	}
	if (range_ == CombatRange::End)
	{
		DebugMessenger::LogError("無効な値が入力されました");
		return;
	}
	directives[id_] &= ~Mask::COMBAT_RANGE;
	unsigned char range = static_cast<unsigned char>(range_) << DIRECTION_BIT_NUM;
	directives[id_] |= range;
}

void EnemyDirective::SetDirective(int id_, Actions action_)
{
	if (id_ < 0 || id_ >= directives.size())
	{
		DebugMessenger::LogError("配列の範囲外が指定されました");
		return;
	}
	if (action_ == Actions::End)
	{
		DebugMessenger::LogError("無効な値が入力されました");
		return;
	}
	directives[id_] &= ~Mask::ACTION;
	unsigned char action = static_cast<unsigned char>(action_) << (DIRECTION_BIT_NUM + COMBAT_RANGE_BIT_NUM);
	directives[id_] |= action;
}

// 現在位置から時計回り正で num_ 移動する地点をセット
void EnemyDirective::RotatePosition(int id_, signed char num_)
{
	if (id_ < 0 || id_ >= directives.size())
	{
		DebugMessenger::LogError("配列の範囲外が指定されました");
		return;
	}

	unsigned char directive = directives[id_];

	directives[id_] &= ~Mask::DIRECTION;

	directive += num_;
	directives[id_] |= directive & Mask::DIRECTION;
}


int EnemyDirective::AddDirective(unsigned char directive_)
{
	directives.push_back(directive_);
	return static_cast<int>(directives.size()) - 1;
}


void EnemyDirective::SetTransformOfRepresentativeEnemy(std::shared_ptr<const Transform> transform_of_representative_enemy_)
{
	DebugMessenger::Log("Transformをセットしました");
	transformOfRepresentativeEnemy = transform_of_representative_enemy_;

	if (transformOfRepresentativeEnemy == nullptr)
	{
		DebugMessenger::LogWarning("transformがnullptrです");
	}
}