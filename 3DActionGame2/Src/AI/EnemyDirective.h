#pragma once
#include <vector>
#include "IEnemyDirectiveReader.h"
#include "IEnemyDirectiveWriter.h"

struct Vector3;

// 各エネミーに対する指令書のようなもの

class EnemyDirective : public IEnemyDirectiveReader, IEnemyDirectiveWriter
{
public:
	// 方位とバイナリの対応
	enum class Direction
	{
		N	= 0x00,
		NNE	= 0x01,
		NE	= 0x02,
		ENE = 0x03,
		E	= 0x04,
		ESE = 0x05,
		SE	= 0x06,
		SSE = 0x07,
		S	= 0x08,
		SSW	= 0x09,
		SW	= 0x0a,
		WSW	= 0x0b,
		W	= 0x0c,
		WNW	= 0x0d,
		NW	= 0x0e,
		NNW	= 0x0f,
	};

	// プレイヤーとの距離に関するenum
	enum class CombatRange
	{
		IN_FIGHT,
		CLOSE_RANGE,
		MIDDLE_RANGE,
		OUT_RANGE
	};

	// エネミーの行動に関するenum
	enum class Actions
	{
		STAND_BY,
		ATTACK,
		RUSH,
		GUARD
	};

public:
	// TODO: IEnemyDirectiveReaderに純粋仮想関数として宣言
	Vector3 GetDirectedPosition(unsigned char id_) const;
	Actions GetDirectedAciton(unsigned char id_) const;

	// TODO: IEnemyDirectiveWriterに純粋仮想関数として宣言
	void SetDirective(unsigned char id_, unsigned char directive_);
	void SetDirective(unsigned char id_, Direction direction_, CombatRange range_, Actions action_);
	void SetDirective(unsigned char id_, Direction direction_);
	void SetDirective(unsigned char id_, CombatRange range_);
	void SetDirective(unsigned char id_, Actions action_);

	// TODO: 後で実装
	// 現在位置から時計回り正で num_ 移動する地点をセット
	void RotatePosition(unsigned char id_, signed char num_);

private:
	// 検討事項：マスクの定義の仕方
	enum class Mask
	{
		NONE			= 0x00,	
		DIRECTION		= 0x0f,	// 方位指定部分
		COMBAT_RANGE	= 0x30,	// 距離指定部分
		ACTION			= 0xc0,	// 行動内容指定部分
		ALL				= 0xff	
	};

private:
	// 待機距離の値
	static constexpr float inFightDistance = 3.0f;
	static constexpr float closeRangeDistance = 5.0f;
	static constexpr float middleRangeDistance = 8.0f;
	static constexpr float outRangeDistance = 12.0f;


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