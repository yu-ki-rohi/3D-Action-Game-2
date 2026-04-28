#pragma once
#include<memory>
#include<vector>
#include<unordered_map>


class TimerBase;

// エネミーに指令を出すクラス

namespace EnemyAI
{
	class EnemyDirective;
	class EnemyReportHandler;

	// HACK: エネミーの数が3であることに依存しているので、なるべくもっと汎用的な形にしたい
	class EnemyCommanderBrain
	{
	public:
		EnemyCommanderBrain(std::shared_ptr<EnemyDirective> directives_, std::shared_ptr<EnemyAI::EnemyReportHandler> report_handler);

	public:
		int AllocateId();

		void Start();
		void InitialCommand();
		void Command();

	private:
		void SetupSequences();
		void SetNextCenter(int id_);

	private:
		static constexpr int participatingEnemyNum = 3;
		enum Pos
		{
			Center,
			Left,
			Right
		};
	private:
		std::shared_ptr<EnemyDirective> directives;
		std::shared_ptr<EnemyReportHandler> reportHandler;
		std::vector<std::vector<unsigned int>> sequences;
		unsigned int currentDirective = 0;
		int currentSequence = 0;
		int currentStep = 0;

		int idCenterLeftRight[participatingEnemyNum]{ 0,2,1 };




	};
}