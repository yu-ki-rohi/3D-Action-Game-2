#pragma once
#include "ObjectBase.h"
#include <memory>

class TimerBase;

namespace EnemyAI
{
	class EnemyCommanderBrain;
	class EnemyDirective;
}

class EnemyCommander : public ObjectBase
{
public:
	EnemyCommander(std::shared_ptr<EnemyAI::EnemyDirective> directives_);

public:
	Tag GetTag() const override;


	void SetMonochrome(float rate_) override {};
	void ChangeMonochromeRequest(float initial_rate_, float target_rate_, float duration_) override {};

	void SetLocalTimeScale(float time_scale_) override;
	void MultiplyLocalTimeScaleBy(float multiplier_) override;

	void Awake() {}

	void Start() override;

	void FixedUpdate() override {}
	void Update(float elapsed_time) override;
	void Render() override {}
	void RenderShadow() override {}

private:


private:
	std::shared_ptr<EnemyAI::EnemyCommanderBrain> brain;
	std::shared_ptr<TimerBase> commandTimer;
};