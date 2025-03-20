#pragma once

struct Transform;
class Animator;

class Renderer
{
public:
	Renderer(int model_handle_, int vertex_shader_handle_, int pixel_shader_handle_);

public:
	int GetModelHandle();
	int GetVSHandle();
	int GetPSHandle();

public:
	void Render(Transform& transform_);
	void Render(Transform& transform_, Animator& animator_);

	void SetupModelInfo(Transform& transform_);
	void SetupModelInfo(Transform& transform_, Animator& animator_);

private:
	const int modelHandle;

	const int vertexShaderHandle;
	const int pixelShaderHandle;
};