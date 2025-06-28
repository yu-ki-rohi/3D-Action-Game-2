#pragma once
#include "CharacterStatus.h"

class Enemy;

class EnemyStatus : public CharacterStatus
{
public:
	EnemyStatus(std::shared_ptr<Enemy> owner_);

public:
	void OnTriggerEnter(Collider* other_) override;

private:
	std::weak_ptr<Enemy> owner;
};

