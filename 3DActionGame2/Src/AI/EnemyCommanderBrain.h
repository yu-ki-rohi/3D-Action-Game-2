#pragma once
#include<memory>


class TimerBase;

// エネミーに指令を出すクラス

namespace EnemyAI
{
	class EnemyDirective;
	class EnemyReportHandler;

	class EnemyCommanderBrain
	{
	public:
		EnemyCommanderBrain(std::shared_ptr<EnemyDirective> directives_, std::shared_ptr<EnemyAI::EnemyReportHandler> report_handler);

	public:
		int AllocateId();

		void Command();
	private:
		std::shared_ptr<EnemyDirective> directives;
		std::shared_ptr<EnemyReportHandler> reportHandler;

	};
}