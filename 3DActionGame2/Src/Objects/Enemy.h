#pragma once
#include "CharacterBase.h"
#include "../Collision/BoxCollider.h"

class Enemy : public CharacterBase
{
public:
	Enemy();

public:
	Tag GetTag() const override;

	void Start() override;

	void FixedUpdate() override;
	void Render() override;

protected:
	virtual void UpdateBehavior(float elapsed_time_) override;
	void UpdateCollider() override;

private:
	BoxCollider attackCollider;
	BoxCollider bodyCollider;

};