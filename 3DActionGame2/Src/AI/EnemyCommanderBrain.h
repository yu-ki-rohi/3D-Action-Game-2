#pragma once
#include<memory>


class TimerBase;

// エネミーに指令を出すクラス

namespace EnemyAI
{
	class EnemyDirective;
	class IEnemyReportReader;

	class EnemyCommanderBrain : public std::enable_shared_from_this<EnemyCommanderBrain>
	{
	public:
		EnemyCommanderBrain(std::shared_ptr<EnemyDirective> directives_);

	public:
		void Command();

	private:
		std::shared_ptr<EnemyDirective> directives;

	};
}