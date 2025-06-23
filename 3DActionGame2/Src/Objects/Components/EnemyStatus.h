#pragma once
#include "CharacterStatus.h"

class EnemyStatus : public CharacterStatus
{
public:
	void OnTriggerEnter(Collider* other_) override;
};

