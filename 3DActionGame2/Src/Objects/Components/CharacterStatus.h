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

	// “–‚½‚èŽž
	virtual void OnTriggerEnter(Collider* other_) override;

protected:
	bool isInvincible;
};