#pragma once
#include "CharacterBase.h"
#include "../Collision/BoxCollider.h"

// todo: 前方宣言で出来ないかどうか調べる
#include "../AI/EnemyBrain.h"
#include "../AI/IEnemyDirectiveReader.h"

class JustAvoidIgnition;
class TimerBase;

//#define TMP_EVAUATION

class Enemy : public CharacterBase
{
public:
	Enemy(int id_, std::shared_ptr<EnemyAI::IEnemyDirectiveReader> directive_);

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
	// HACK: 設計の見直し
	void SetupBrain();

	void SetupColliders();
private:
	std::shared_ptr<BoxCollider> attackCollider;
	std::shared_ptr<BoxCollider> bodyCollider;
	std::shared_ptr<BoxCollider> justAvoidIgnitionCollider;

	std::shared_ptr<JustAvoidIgnition> justAvoidIgnition;

	std::shared_ptr<TimerBase> enableColliderTimer;
	std::shared_ptr<TimerBase> disableColliderTimer;

	bool isChanging;

	// 
	std::unique_ptr<EnemyAI::EnemyBrain> brain;
	
};