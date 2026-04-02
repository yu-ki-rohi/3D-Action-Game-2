#pragma once

// 時間関係のパラメータを管理
// staticメンバのみ

class Time
{
public:
	static float TimeScale;

	static constexpr int FixedFrame = 60;
	static constexpr float FixedDeltaTime = 1.0f / (float)FixedFrame;
};
