#pragma once
#include "CharacterStatus.h"

class PlayerEventNotifier;

class PlayerStatus : public CharacterStatus
{
public:
	PlayerStatus(std::shared_ptr<PlayerEventNotifier> notifier_);

public:
	void Damage(int attack_) override;
	void OnTriggerEnter(Collider* other_) override;

private:
	std::weak_ptr<PlayerEventNotifier> notifier;
};

