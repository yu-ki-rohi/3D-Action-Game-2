#pragma once
#include "ComponentBase.h"

class Transform;
class Animator;

// ���O�̊���ɔėp�����Ⴛ��
// ���f�������ł�����������悤�ɂ�����������
// �V�F�[�_�[��n���Ȃ��p�^�[�����p��

// �V�F�[�_�[�̎g�p��Renderer���Ŕ��f�����ق�����������

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