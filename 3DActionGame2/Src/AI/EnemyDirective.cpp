#include "EnemyDirective.h"
#include "../Mathmatics/Vector3.h"
#include "../Debug/DebugMessenger.h"
#include "WorldBlackboard.h"

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

using namespace EnemyAI;

EnemyDirective::EnemyDirective(std::shared_ptr<const WorldBlackboard> world_blackboard_, std::shared_ptr<EnemyReportHandler> report_handler_) :
	worldBrackbord(world_blackboard_),
	reportHandler(report_handler_)
{

}

unsigned char EnemyDirective::MakeDirective(Direction direction_, CombatRange range_, Actions action_)
{
	if (direction_ == Direction::End || range_ == CombatRange::End || action_ == Actions::End)
	{
		DebugMessenger::LogError("無効な値が入力されました");
		return 0;
	}
	unsigned char direction = (unsigned char)direction_;
	unsigned char range = (unsigned char)range_ << DIRECTION_BIT_NUM;
	unsigned char action = (unsigned char)action_ << (DIRECTION_BIT_NUM + COMBAT_RANGE_BIT_NUM);
	return direction | range | action;
}

Vector3 EnemyDirective::GetDirectedPosition(int id_) const
{
	return GetDirectedRelativePositionToPlayer(id_) + worldBrackbord->GetPlayerPosition();
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
	// TODO: 長いので関数を分ける

	// 現状ステージ内に高低差が存在しないので、yは0.0固定
	float x, y = 0.0f, z;

	// 方位確認
	{
		// 方位情報の抜き出し
		unsigned char direction = directives[id_] & (unsigned char)Mask::DIRECTION;

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



		// 符号の抽出
		float sign = (float)(direction >> (DIRECTION_BIT_NUM - 1));
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

	// 距離確認
	{
		// 距離情報の抜き出し
		unsigned char range = directives[id_] & (unsigned char)Mask::COMBAT_RANGE;
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

	// TODO: プレイヤーの座標を足す
	return Vector3(x, y, z);
}
Actions EnemyDirective::GetDirectedAciton(int id_) const
{
	if (id_ < 0 || id_ >= directives.size())
	{
		DebugMessenger::LogError("配列の範囲外が指定されました");
		return Actions::STAND_BY;
	}

	// 検討事項：このキャストの妥当性
	return (Actions)(directives[id_] >> (DIRECTION_BIT_NUM + COMBAT_RANGE_BIT_NUM));
}

Vector3 EnemyDirective::GetPlayerPosition() const
{
	return worldBrackbord->GetPlayerPosition();
}

int EnemyDirective::GetDirectivesNum() const
{
	return directives.size();
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


void EnemyDirective::SetDirective(int id_, Direction direction_, CombatRange range_, Actions action_)
{
	SetDirective(id_, MakeDirective(direction_, range_, action_));
}

void EnemyDirective::SetDirective(int id_, Direction direction_)
{
	if (id_ < 0 || id_ >= directives.size())
	{
		DebugMessenger::LogError("配列の範囲外が指定されました");
		return;
	}
	if (direction_ == Direction::End)
	{
		DebugMessenger::LogError("無効な値が入力されました");
		return;
	}
	directives[id_] &= ~(unsigned char)Mask::DIRECTION;
	directives[id_] |= (unsigned char)direction_;
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
	directives[id_] &= ~(unsigned char)Mask::COMBAT_RANGE;
	unsigned char range = (unsigned char)range_ << DIRECTION_BIT_NUM;
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
	directives[id_] &= ~(unsigned char)Mask::ACTION;
	unsigned char action = (unsigned char)action_ << (DIRECTION_BIT_NUM + COMBAT_RANGE_BIT_NUM);
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

}

int EnemyDirective::AddDirective(unsigned char directive_)
{
	directives.push_back(directive_);
	return (int)directives.size() - 1;
}