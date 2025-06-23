#pragma once
#include "ComponentBase.h"

class Transform;
class Animator;

// 名前の割りに汎用性が低そう
// モデルだけでも成立させるようにした方がいい
// シェーダーを渡さないパターンも用意

// シェーダーの使用はRenderer側で判断したほうがいいかも

class Renderer : public ComponentBase
{
public:
	Renderer(int model_handle_, int vertex_shader_handle_, int pixel_shader_handle_, int shadow_vs_handle_);

public:
	int GetModelHandle();

	int GetVSHandle();
	int GetPSHandle();

	int GetShadowVSHandle();

public:
	void Render(const Transform& transform_);
	void Render(const Transform& transform_, Animator& animator_);

	void SetupModelInfo(const Transform& transform_);
	void SetupModelInfo(const Transform& transform_, Animator& animator_);

public:
	bool IsRenderingShadow;

private:
	const int modelHandle;

	const int vertexShaderHandle;
	const int pixelShaderHandle;

	const int shadowVSHandle;
};