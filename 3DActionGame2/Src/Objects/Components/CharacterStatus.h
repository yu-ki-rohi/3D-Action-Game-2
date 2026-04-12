#pragma once
#include "ComponentBase.h"
#include <memory>

struct CharaceterParameters;

class CharacterStatus : public ComponentBase
{
public:
	CharacterStatus();

public:
	void SetIsInvincible(bool is_invincible_);

	virtual int Damage(int attack_);

	// “–‚½‚è
	virtual void OnTriggerEnter(std::shared_ptr<Collider> other_) override;

protected:
	// –³“Gó‘Ô‚©‚Ç‚¤‚©
	bool isInvincible;
};