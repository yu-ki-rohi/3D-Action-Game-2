#include "TestObject.h"
#include <DxLib.h>
#include "../Input/InputManager.h"
#include "../Systems/SimpleObserver.h"
#include "../Systems/MemberFunctionPointerContainer.h"
#include "../Systems/TimerFactory.h"

TestObject::TestObject()
{
	color = GetColor(0, 0, 255);
	so = std::make_shared<SimpleObserver>();
	InputManager::Instance().AddObserver(InputManager::Stick::Left, so);
	InputManager::Instance().RegisiterBehave(
		InputManager::Map::Menu,
		XINPUT_BUTTON_A,
		InputManager::State::Press,
		std::make_shared<MemberFunctionPointerContainer<TestObject>>(this, &TestObject::PressA));
	InputManager::Instance().RegisiterBehave(
		InputManager::Map::Menu,
		XINPUT_BUTTON_A,
		InputManager::State::Release,
		std::make_shared<MemberFunctionPointerContainer<TestObject>>(this, &TestObject::ReleaseA));
	TimerFactory::CreateTimer(5.0f, this, &TestObject::TimerBehave);
}

void TestObject::FixedUpdate()
{

}

void TestObject::Update(float elapsed_time)
{
	float a, b;
	so->GetFloatx2(a, b);
	x += a * speed * elapsed_time;
	y -= b * speed * elapsed_time;
}

void TestObject::Render()
{
	DrawCircle(x, y, range, color, true);
}

void TestObject::RenderShadow()
{
	DrawCircle(x, y, range, color, true);
}

void TestObject::PressA()
{
	color = GetColor(255, 0, 0);
}

void TestObject::ReleaseA()
{
	color = GetColor(0, 0, 255);
}

void TestObject::TimerBehave()
{
	color = GetColor(0, 255, 0);
}