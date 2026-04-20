#pragma once
#include<memory>

class IEnemyReportReader;

class TimerBase;

// エネミーに指令を出すクラス

namespace EnemyAI
{
	class EnemyDirective;

	class EnemyCommander : public std::enable_shared_from_this<EnemyCommander>
	{
	public:
		EnemyCommander(std::shared_ptr<EnemyDirective> directives_);

	public:
		void Command();

	private:
		std::shared_ptr<EnemyDirective> directives;
		std::shared_ptr<TimerBase> commandTimer;

	};
}