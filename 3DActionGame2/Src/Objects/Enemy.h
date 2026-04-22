#pragma once
#include "CharacterBase.h"
#include "../Collision/BoxCollider.h"



namespace EnemyAI
{
	class EnemyBrain;
	class IEnemyDirectiveReader;
	class EnemyReportHandler;
}

class JustAvoidIgnition;
class TimerBase;


class Enemy : public CharacterBase
{
public:
	Enemy(int id_, std::shared_ptr<EnemyAI::EnemyReportHandler> report_handler, std::shared_ptr<EnemyAI::IEnemyDirectiveReader> directive_);
	~Enemy();

public:
	Tag GetTag() const override;

	virtual void SetLocalTimeScale(float time_scale_) override;
	virtual void MultiplyLocalTimeScaleBy(float multiplier_) override;

	void Start() override;

	void FixedUpdate() override;
	void Render() override;

protected:
	virtual void UpdateBehavior(float elapsed_time_) override;
	void UpdateCollider() override;

private:
	void EnableAttackCollider();
	void DisableAttackCollider();

	// NOTE: オブジェクトの生成とコンポーネントアタッチに時差があるため用意
	// HACK: 設計の見直しを検討
	void SetupBrain();

	void SetupColliders();

private:
	const std::shared_ptr<BoxCollider> attackCollider;
	const std::shared_ptr<BoxCollider> bodyCollider;
	const std::shared_ptr<BoxCollider> justAvoidIgnitionCollider;

	std::shared_ptr<JustAvoidIgnition> justAvoidIgnition;

	// 
	const std::unique_ptr<EnemyAI::EnemyBrain> brain;
	
};