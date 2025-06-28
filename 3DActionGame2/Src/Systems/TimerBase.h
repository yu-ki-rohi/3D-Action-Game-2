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

	// �o�ߎ���
	float GetElapsedTime()
	{
		return elapsedTime;
	}

	// �c�莞��
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
	/// Manager���ɔj�����邱�Ƃ�`����
	/// �g�p�ґ��ł��j�����Ă�������
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