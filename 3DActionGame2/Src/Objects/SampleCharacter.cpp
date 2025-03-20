#include "SampleCharacter.h"
#include <DxLib.h>
#include "../Input/InputManager.h"
#include "../Systems/SimpleObserver.h"
#include "../Systems/MemberFunctionPointerContainer.h"
#include "../Systems/TimerFactory.h"
#include "../Common.h"
#include "SampleCamera.h"

SampleCharacter::SampleCharacter(Vector3 position_, Vector3 scale_, int model_handle_, int vertex_shader_handle_, int pixel_shader_handle_, int idle_anim_handle_, std::shared_ptr<SampleCamera> sample_camera_) :
	transform(Transform(position_, scale_)),
	renderer(Renderer(model_handle_, vertex_shader_handle_, pixel_shader_handle_)),
	animator(Animator(idle_anim_handle_))
{
	so = std::make_shared<SimpleObserver>();
	InputManager::Instance().AddObserver(InputManager::Stick::Left, so);
	
	InputManager::Instance().RegisiterBehave(
		InputManager::Map::Menu,
		XINPUT_BUTTON_A,
		InputManager::State::Press,
		std::make_shared<MemberFunctionPointerContainer<SampleCharacter>>(this, &SampleCharacter::Attack));
	InputManager::Instance().RegisiterBehave(
		InputManager::Map::Menu,
		InputManager::Stick::Left,
		InputManager::State::Press,
		std::make_shared<MemberFunctionPointerContainer<SampleCharacter>>(this, &SampleCharacter::Move));
	InputManager::Instance().RegisiterBehave(
		InputManager::Map::Menu,
		InputManager::Stick::Left,
		InputManager::State::Release,
		std::make_shared<MemberFunctionPointerContainer<SampleCharacter>>(this, &SampleCharacter::Stop));

	InputManager::Instance().RegisiterBehave(
		InputManager::Map::Menu,
		XINPUT_BUTTON_Y,
		InputManager::State::Hold,
		std::make_shared<MemberFunctionPointerContainer<SampleCharacter>>(this, &SampleCharacter::IncreaseMonochrome));
	InputManager::Instance().RegisiterBehave(
		InputManager::Map::Menu,
		XINPUT_BUTTON_X,
		InputManager::State::Hold,
		std::make_shared<MemberFunctionPointerContainer<SampleCharacter>>(this, &SampleCharacter::DecreaseMonochrome));

	transform.RotateAxisY(30.0f);
	if (sample_camera_ != nullptr)
	{
		sample_camera_->SetTarget(&transform);
	}
}

void SampleCharacter::FixedUpdate()
{

}

void SampleCharacter::Update(float elapsed_time_)
{
	animator.Update(elapsed_time_);
}

void SampleCharacter::Render()
{
	SetUseVertexShader(renderer.GetVSHandle());
	SetUsePixelShader(renderer.GetPSHandle());

	FLOAT4 monocrome_Rate = { monochromeRate,0.0f,0.0f, 0.0f };
	SetPSConstF(22, monocrome_Rate);

	renderer.Render(transform, animator);

#ifdef DEBUG
	transform.DebugDrow();
#endif
}

void SampleCharacter::RegisterAnim(Animator::AnimType anim_type_, int anim_handle_)
{
	animator.RegisterAnim(anim_type_, anim_handle_);
}

void SampleCharacter::Attack()
{
	animator.SetNextAnim(Animator::AnimType::Attack00, 0.0f, 4.6f);
}

void SampleCharacter::Move()
{
	animator.SetNextAnim(Animator::AnimType::WalkF, 0.0f, 4.6f, true);
}

void SampleCharacter::Stop()
{
	animator.SetNextAnim(Animator::AnimType::Idle, 0.0f, 4.6f, true);
}


void SampleCharacter::IncreaseMonochrome()
{
	monochromeRate += 0.01f;
	if (monochromeRate > 1.0f) monochromeRate = 1.0f;
}

void SampleCharacter::DecreaseMonochrome()
{
	monochromeRate -= 0.01f;
	if (monochromeRate < 0.0f) monochromeRate = 0.0f;
}