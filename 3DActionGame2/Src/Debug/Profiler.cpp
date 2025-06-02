#include "Profiler.h"
#include "../Systems/Time.h"
#include "../Systems/SystemTimeProcess.h"

void Profiler::Stamp(Type type_)
{
	stampedTime[(int)type_] += SystemTimeProcess::GetElapsedTime(previousSystemTime);
}


void Profiler::Render()
{
	const unsigned int color[STAMP_NUM]{
		GetColor(255,0,0),
		GetColor(0,255,0),
		GetColor(0,0,255),
		GetColor(255,255,0)
	};

	// ï∂éöèÓïÒ
	DrawFormatString(0, 220, color[(int)Type::FixedUpdate], "FixedUpdate : %f", showStampedTime[(int)Type::FixedUpdate]);
	DrawFormatString(0, 240, color[(int)Type::Update],      "Update      : %f", showStampedTime[(int)Type::Update]);
	DrawFormatString(0, 260, color[(int)Type::Render],      "Render      : %f", showStampedTime[(int)Type::Render]);
	DrawFormatString(0, 280, color[(int)Type::VSync],       "Ohters      : %f", showStampedTime[(int)Type::VSync]);


	// ë—ÇÃï\é¶
	const int x[2]{ 10,50 };
	int y[2]{ 300,300 };

	// ë—ÇÃí∑Ç≥
	const int length = 300;


	for (int i = 0; i < STAMP_NUM; i++)
	{
		float ratio = showStampedTime[i] / 1.0f;

		y[1] += (int)(length * ratio);

		DrawBox(x[0], y[0], x[1], y[1], color[i], TRUE);

		y[0] = y[1];
	}

}

void Profiler::Update()
{
	for (int i = 0; i < STAMP_NUM; i++)
	{
		showStampedTime[i] = stampedTime[i];
	}
}

void Profiler::ResetTimes()
{
	for (int i = 0; i < STAMP_NUM; i++)
	{
		stampedTime[i] = 0.0f;
	}
}