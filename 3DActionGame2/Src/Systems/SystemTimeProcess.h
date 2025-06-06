#pragma once
#include "DxLib.h"

/// <summary>
/// 経過時間を測るための処理を保持するクラス
/// *あくまで処理を保持するだけで比較する時刻は他から持ってくる必要あり
/// 
/// ・・・これTimeクラスにもたせてもいいのでは?
/// </summary>
class SystemTimeProcess
{
public:
	static float GetElapsedTime(LONGLONG& past_system_time_);
};