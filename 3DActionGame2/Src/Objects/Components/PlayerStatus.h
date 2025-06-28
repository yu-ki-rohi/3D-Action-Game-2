#pragma once
#include "CharacterStatus.h"

class PlayerEventNotifier;
class ObjectFactory;

class PlayerStatus : public CharacterStatus
{
public:
	PlayerStatus(std::shared_ptr<PlayerEventNotifier> notifier_, std::shared_ptr<ObjectFactory> object_factory_);

public:
	int Damage(int attack_) override;
	void OnTriggerEnter(Collider* other_) override;

private:
	std::weak_ptr<PlayerEventNotifier> notifier;
	std::weak_ptr<ObjectFactory> objectFactory;
};

