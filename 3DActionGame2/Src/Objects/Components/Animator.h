#pragma once
#include <unordered_map>

class Animator
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
	void SetNextAnim(AnimType anim_type_, float start_changing_time_, float changing_time_, bool is_loop_ = false);

	void SetAnimSpeed(float anim_speed_);

public:

	void SetupRenderAnim(int model_handle_);
	void DetachAnim(int model_handle_);

	void Update(float elapsed_time_);

private:
	// AnimType���Ƃ̃n���h�����L���b�V��
	std::unordered_map<AnimType, int> animHandles;

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
	float ChangingTime = 0.1f;

	bool isLoop = true;
	bool nextIsLoop = false;

	bool isStartChangeAnimNow = false;
	bool isFinishChangeAnimNow = false;

private:
	static constexpr float minChangingTime = 0.001f;

};