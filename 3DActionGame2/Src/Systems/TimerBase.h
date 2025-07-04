#pragma once

class TimerBase
{
public:
	TimerBase(const float time_) :
		time(time_),
		elapsedTime(0.0f),
		localTimeScale(1.0f)
	{

	}

	// 経過時間
	float GetElapsedTime()
	{
		return elapsedTime;
	}

	// 残り時間
	float GetRemainingTime()
	{
		return time - elapsedTime;
	}

	bool IsActive()
	{
		return isActive;
	}

	void SetLocalTimeScale(float local_time_scale_)
	{
		localTimeScale = local_time_scale_;
	}

	virtual void Update(float elapsedTime_) = 0;

	/// <summary>
	/// Manager側に破棄することを伝える
	/// 使用者側でも破棄してください
	/// </summary>
	void Erase()
	{
		isActive = false;
	}

	void ResizeTime(float time_)
	{
		time = time_;
	}

public:
	bool IsStop = false;

protected:
	bool isActive = true;

	float time;
	float elapsedTime;

	float localTimeScale;
};