#pragma once
#include "ObjectBase.h"
#include <memory>

class SimpleObserver;

class TestObject : public ObjectBase
{
public:
	TestObject();
public:
	void FixedUpdate() override;
	void Update(float elapsed_time) override;
	void Render() override;

private:
	void PressA();
	void ReleaseA();
	void TimerBehave();
private:
	float x = 960.0f;
	float y = 540.0f;

	float speed = 60.0f;

	float range = 5.0f;
	unsigned int color;

	std::shared_ptr<SimpleObserver> so;
};