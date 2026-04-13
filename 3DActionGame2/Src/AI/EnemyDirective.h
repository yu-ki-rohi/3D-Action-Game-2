#pragma once
#include <vector>
#include "IEnemyDirectiveReader.h"
#include "IEnemyDirectiveWriter.h"
#include "EnemyAICommons.h"

struct Vector3;

// 各エネミーに対する指令書のようなもの
namespace EnemyAI
{
	class EnemyDirective : public IEnemyDirectiveReader, IEnemyDirectiveWriter
	{
	public:

	public:
		// TODO: IEnemyDirectiveReaderに純粋仮想関数として宣言
		Vector3 GetDirectedPosition(int id_) const override;
		Actions GetDirectedAciton(int id_) const override;

		// TODO: IEnemyDirectiveWriterに純粋仮想関数として宣言
		void SetDirective(int id_, unsigned char directive_);
		void SetDirective(int id_, Direction direction_, CombatRange range_, Actions action_);
		void SetDirective(int id_, Direction direction_);
		void SetDirective(int id_, CombatRange range_);
		void SetDirective(int id_, Actions action_);

		// TODO: 後で実装
		// 現在位置から時計回り正で num_ 移動する地点をセット
		void RotatePosition(int id_, signed char num_);

		// 戻り値はインデックス
		int AddDirective(unsigned char directive_);

	private:
		// 検討事項：マスクの定義の仕方
		enum class Mask
		{
			NONE = 0x00,
			DIRECTION = 0x0f,	// 方位指定部分
			COMBAT_RANGE = 0x30,	// 距離指定部分
			ACTION = 0xc0,	// 行動内容指定部分
			ALL = 0xff
		};

	private:
		// 待機距離の値
		static constexpr float inFightDistance = 30.0f;
		static constexpr float closeRangeDistance = 70.0f;
		static constexpr float middleRangeDistance = 110.0f;
		static constexpr float outRangeDistance = 170.0f;


	private:
		// directivesの表現方法
		// 上位4bit
		// 上位2bitで指示行動を、下位2ビットで距離を表現
		// 
		// 下位4bit：方位(ワールド座標系）
		// 時計回りで割り当て
		// 
		// (例) directives[0] = 10 とすると
		//     指示内容 距離  方位
		//  0b    00     01   0010 
		// 0番エネミーはプレイヤーに対し北東(NE)の近距離(CLOSE_RANGE)で待機(STAND_BY)という指示になる
		std::vector<unsigned char> directives;


		// TODO：プレイヤーの座標をどのような経由で参照するか決定

	};

}
