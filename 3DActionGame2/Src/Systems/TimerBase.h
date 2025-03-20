#pragma once

class TimerBase
{
public:
	TimerBase(const float time_) :
		TIME(time_)
	{

	}

#pragma region �A�N�Z�T

	// �o�ߎ���
	float GetElapsedTime()
	{
		return elapsedTime;
	}

	// �c�莞��
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
	/// Manager���ɔj�����邱�Ƃ�`����
	/// �g�p�ґ��ł��j�����Ă�������
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