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

	virtual void Damage(int attack);

	// “–‚½‚è”»’è
	virtual void OnTriggerEnter(Collider* other_) override;

protected:
	bool isInvincible;
};