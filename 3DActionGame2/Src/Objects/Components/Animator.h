#pragma once
#include <unordered_map>
#include "ComponentBase.h"

class Animator : public ComponentBase
{
public:
	enum class AnimType
	{
		Idle,
		WalkF,
		WalkL,
		WalkR,
		WalkB,
		Run,
		Attack00,
		Attack01,
		Attack02,
		Attack03,
		Attack04,
		Attack05,
		Attack06,
		Damage,
		Down
	};

public:
	Animator(int idle_anim_handle_);

public:
	// animHandles�ɃL���b�V���Ƃ��ēo�^
	void RegisterAnim(AnimType anim_type_, int anim_handle_);

	void SetNextAnim(AnimType anim_type_, bool is_loop_ = false);
	/// <summary>
	/// �A�j���[�V�����J�ڐ���Z�b�g
	/// </summary>
	/// <param name="anim_type_">�J�ڐ�̎��</param>
	/// <param name="start_changing_time_">�J�ڂ��J�n����A�j���[�V��������</param>
	/// <param name="changing_time_">�J�ڂ��I���܂ł̎���</param>
	/// <param name="is_loop_">���̃A�j���[�V���������[�v�����邩�ۂ�</param>
	void SetNextAnim(AnimType anim_type_, float start_changing_time_, float changing_time_, bool is_loop_ = false);

	void SetAnimSpeed(float anim_speed_);

public:

	void SetupRenderAnim(int model_handle_);
	void DetachAnim(int model_handle_);

	void Update(float elapsed_time_);

private:
	// AnimType���Ƃ̃n���h��
	std::unordered_map<AnimType, int> animHandles;

	// nextXX�̓A�j���[�V�����J�ڐ��\�񂷂邽�߂Ɏg�p
	// (�A�j���[�V�����̐��`��Ԃ̂���)

	// �Đ�����A�j���[�V�����̃n���h��
	// AnimType���T���Ă������@�ł���������
	int currentAnimHandle = -1;
	int nextAnimHandle = -1;

	int currentAttachIndex = -1;
	int nextAttachIndex = -1;

	float currentAnimTimer = 0.0f;
	float nextAnimTimer = 0.0f;

	// ���݃A�j���[�V�����̃g�[�^������
	float animTime = 0.0f;

	// ��U�Œ�l
	// TODO �A�j���[�V�������Ƃɐݒ�ł���悤�ɂ���
	float animSpeed = 30.0f;

	float startChangingTime = 0.0f;
	float changingTime = 0.1f;

	bool isLoop = true;
	bool nextIsLoop = false;

	// �ؑւ̎n��/�I����ʒm���邽��
	bool isStartChangeAnimNow = false;
	bool isFinishChangeAnimNow = false;

private:
	static constexpr float minChangingTime = 0.001f;

};