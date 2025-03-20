#include "Renderer.h"
#include "Transform.h"
#include "Animator.h"
#include <DxLib.h>

Renderer::Renderer(int model_handle_, int vertex_shader_handle_, int pixel_shader_handle_) :
	modelHandle(model_handle_),
	vertexShaderHandle(vertex_shader_handle_),
	pixelShaderHandle(pixel_shader_handle_)
{

}

int Renderer::GetModelHandle()
{
	return modelHandle;
}

int Renderer::GetVSHandle()
{
	return vertexShaderHandle;
}

int Renderer::GetPSHandle()
{
	return pixelShaderHandle;
}

void Renderer::Render(Transform& transform_)
{
	SetupModelInfo(transform_);
	MV1DrawModel(modelHandle);
}

void Renderer::Render(Transform& transform_, Animator& animator_)
{
	SetupModelInfo(transform_, animator_);
	MV1DrawModel(modelHandle);
	animator_.DetachAnim(modelHandle);
}


void Renderer::SetupModelInfo(Transform& transform_)
{
	MV1SetPosition(modelHandle, transform_.Position.ToVECTOR());
	MV1SetScale(modelHandle, transform_.Scale.ToVECTOR());
	MV1SetRotationXYZ(modelHandle, transform_.GetRotation().ToVECTOR());
}

void Renderer::SetupModelInfo(Transform& transform_, Animator& animator_)
{
	SetupModelInfo(transform_);
	animator_.SetupRenderAnim(modelHandle);
}