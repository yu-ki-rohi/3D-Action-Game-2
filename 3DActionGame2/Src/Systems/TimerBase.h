#pragma once

class TimerBase
{
public:
	TimerBase(const float time_) :
		TIME(time_)
	{

	}

#pragma region アクセサ

	// 経過時間
	float GetElapsedTime()
	{
		return elapsedTime;
	}

	// 残り時間
	float GetRemainingTime()
	{
		return TIME - elapsedTime;
	}

	bool IsActive()
	{
		return isActive;
	}

#pragma endregion

	virtual void Update(float elapsedTime_) = 0;

	/// <summary>
	/// Manager側に破棄することを伝える
	/// 使用者側でも破棄してください
	/// </summary>
	void Erase()
	{
		isActive = false;
	}

public:
	bool IsStop = false;

protected:
	bool isActive = true;

	const float TIME;
	float elapsedTime = 0.0f;
};