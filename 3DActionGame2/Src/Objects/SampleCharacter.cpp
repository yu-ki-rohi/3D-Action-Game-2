#include "SampleCharacter.h"
#include <DxLib.h>
#include "../Input/InputManager.h"
#include "../Systems/SimpleObserver.h"
#include "../Systems/MemberFunctionPointerContainer.h"
#include "../Systems/TimerFactory.h"
#include "../Common.h"
#include "SampleCamera.h"

SampleCharacter::SampleCharacter(Vector3 position_, Vector3 scale_, int model_handle_, int vertex_shader_handle_, int pixel_shader_handle_, int shadow_vs_handle_, int idle_anim_handle_, std::shared_ptr<SampleCamera> sample_camera_) :
	transform(Transform(position_, scale_)),
	renderer(Renderer(model_handle_, vertex_shader_handle_, pixel_shader_handle_, shadow_vs_handle_)),
	animator(Animator(idle_anim_handle_)),
	attackCollider(Vector3(53.0f, 40.0f, 5.0f), Vector3(135.0f, 18.0f, 12.0f), Vector3(10.0f, -8.0f,30.0f))
{
	so = std::make_shared<SimpleObserver>();
	InputManager::Instance().AddObserver(InputManager::Stick::Left, so);
	
	InputManager::Instance().RegisterBehave(
		InputManager::Map::Menu,
		XINPUT_BUTTON_A,
		InputManager::State::Press,
		std::make_shared<MemberFunctionPointerContainer<SampleCharacter>>(this, &SampleCharacter::Attack));
	InputManager::Instance().RegisterBehave(
		InputManager::Map::Menu,
		InputManager::Stick::Left,
		InputManager::State::Press,
		std::make_shared<MemberFunctionPointerContainer<SampleCharacter>>(this, &SampleCharacter::Move));
	InputManager::Instance().RegisterBehave(
		InputManager::Map::Menu,
		InputManager::Stick::Left,
		InputManager::State::Release,
		std::make_shared<MemberFunctionPointerContainer<SampleCharacter>>(this, &SampleCharacter::Stop));

	InputManager::Instance().RegisterBehave(
		InputManager::Map::Menu,
		XINPUT_BUTTON_Y,
		InputManager::State::Hold,
		std::make_shared<MemberFunctionPointerContainer<SampleCharacter>>(this, &SampleCharacter::IncreaseMonochrome));
	InputManager::Instance().RegisterBehave(
		InputManager::Map::Menu,
		XINPUT_BUTTON_X,
		InputManager::State::Hold,
		std::make_shared<MemberFunctionPointerContainer<SampleCharacter>>(this, &SampleCharacter::DecreaseMonochrome));

	transform.RotateAxisY(-160.0f);
	if (sample_camera_ != nullptr)
	{
		sample_camera_->SetTarget(&transform);
		sampleCamera = sample_camera_;
	}
}

Vector3 SampleCharacter::GetPosition()
{
	return transform.Position;
}

void SampleCharacter::Start()
{
	attackCollider.SetOwner(shared_from_this());
}

void SampleCharacter::FixedUpdate()
{

}

void SampleCharacter::Update(float elapsed_time_)
{
	animator.Update(elapsed_time_);
	std::shared_ptr<SampleCamera> sample_camera = sampleCamera.lock();
	if (sample_camera != nullptr)
	{
		Vector3 forward = sample_camera->GetForward();
		Vector3 right = sample_camera->GetRight();

		forward.y = 0.0f;
		forward = forward.Normalize();

		float x, y;
		so->GetFloatx2(x, y);

		float speed = 30.0f;
		Vector3 move_vec = forward * y + right * x;
		transform.Position += move_vec * elapsed_time_ * speed;

		transform.SetForward(-move_vec);

		renderer.SetupModelInfo(transform, animator);
		MATRIX frame_matrix = MV1GetFrameLocalWorldMatrix(renderer.GetModelHandle(), 34);
		attackCollider.UpdateFromParentMat(frame_matrix);
		animator.DetachAnim(renderer.GetModelHandle());
		//MV1SetMatrix(renderer.GetModelHandle(), MGetIdent());
	}
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
	attackCollider.DebugDrow();

	std::shared_ptr<SampleCamera> sample_camera = sampleCamera.lock();
	if (sample_camera != nullptr)
	{
		Vector3 vector = Vector3::BasisTransformation(-transform.Position, transform.GetForward(), transform.GetUp(), transform.GetRight());
		DrawFormatString(0, 660, GetColor(255, 255, 255), "Position : ( %f, %f, %f)", transform.Position.x, transform.Position.y, transform.Position.z);
		DrawFormatString(0, 680, GetColor(255, 255, 255), "Origin : ( %f, %f, %f)", vector.x, vector.y, vector.z);
		vector = transform.GetRight() * vector.x + transform.GetUp() * vector.y + transform.GetForward() * vector.z + transform.Position;
		DrawFormatString(0, 700, GetColor(255, 255, 255), "Check : ( %f, %f, %f)", vector.x , vector.y, vector.z);
	}
#endif
}

void SampleCharacter::RenderShadow()
{
	SetUseVertexShader(renderer.GetShadowVSHandle());
	renderer.Render(transform, animator);
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