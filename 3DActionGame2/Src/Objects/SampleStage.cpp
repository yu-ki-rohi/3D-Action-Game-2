#include "SampleStage.h"
#include "../Input/InputManager.h"
#include "../Systems/MemberFunctionPointerContainer.h"

SampleStage::SampleStage(Vector3 position_, int model_handle_, int vertex_shader_handle_, int pixel_shader_handle_, int shadow_vs_handle_) :
	transform(Transform(position_)),
	renderer(Renderer(model_handle_, vertex_shader_handle_, pixel_shader_handle_, shadow_vs_handle_))
{
	InputManager::Instance().RegisterBehave(
		InputManager::Map::Menu,
		XINPUT_BUTTON_RIGHT_SHOULDER,
		InputManager::State::Hold,
		std::make_shared<MemberFunctionPointerContainer<SampleStage>>(this, &SampleStage::IncreaseMonochrome));
	InputManager::Instance().RegisterBehave(
		InputManager::Map::Menu,
		XINPUT_BUTTON_LEFT_SHOULDER,
		InputManager::State::Hold,
		std::make_shared<MemberFunctionPointerContainer<SampleStage>>(this, &SampleStage::DecreaseMonochrome));
}

void SampleStage::FixedUpdate()
{

}

void SampleStage::Update(float elapsed_time_)
{
	
}

void SampleStage::Render()
{
	SetUseVertexShader(renderer.GetVSHandle());
	SetUsePixelShader(renderer.GetPSHandle());

	FLOAT4 monocrome_Rate = { monochromeRate,0.0f,0.0f, 0.0f };
	SetPSConstF(22, monocrome_Rate);

	renderer.Render(transform);
}

void SampleStage::RenderShadow()
{
	SetUseVertexShader(renderer.GetShadowVSHandle());
	renderer.Render(transform);
}

void SampleStage::IncreaseMonochrome()
{
	monochromeRate += 0.01f;
	if (monochromeRate > 1.0f) monochromeRate = 1.0f;
}

void SampleStage::DecreaseMonochrome()
{
	monochromeRate -= 0.01f;
	if (monochromeRate < 0.0f) monochromeRate = 0.0f;
}