#pragma once
#include <unordered_map>
#include <memory>
#include "ComponentBase.h"
#include "../../DataBase/DataKind.h"

struct AnimationResource;

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
	Animator(std::shared_ptr<AnimationResource> anim_resource_);

public:
	// �A�j���[�V�������ǂ��܂Ői��ł��邩���擾
	float GetAnimationProgressPercentage();

	// �A�j���[�V�����̑S�̂̎��ԂɈ������|�������̂��擾
	float GetAnimationTimeByNormalizedValue(float normalized_value_);

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

	void SetNextAnim(AKind anim_kind_, float start_changing_time_, float changing_time_, bool is_loop_ = false);

	void SetAnimTimerAdjuster(float value_);


public:

	void SetupRenderAnim(int model_handle_);
	void DetachAnim(int model_handle_);

	void Update(float elapsed_time_);

public:
	// Animation�̑J�ڂ𑦍��ɊJ�n����鎞�̂��߂�
	// �v�������������ɐ؂�ւ������Ƃ������Ƃ̕������������̂ŗp��
	static constexpr float Immediately = 0.0f;

private:
	// AnimType���Ƃ̃n���h��
	std::unordered_map<AnimType, int> animHandles;

	std::shared_ptr<AnimationResource> animResource;

	// nextXX�̓A�j���[�V�����J�ڐ��\�񂷂邽�߂Ɏg�p
	// (�A�j���[�V�����̐��`��Ԃ̂���)

	// �Đ�����A�j���[�V�����̃n���h��
	int currentAnimHandle = -1;
	int nextAnimHandle = -1;

	int currentAttachIndex = -1;
	int nextAttachIndex = -1;

	float currentAnimTimer = 0.0f;
	float nextAnimTimer = 0.0f;

	// �A�j���[�V�����̍Đ����Ԃ����炷�ۂɎg�p
	// �����蔻��̕t������������̍Đ����Ԃɂ���A�Ȃǂ̗p�r
	float animTimerAdjuster = 0.0f;

	// ���݃A�j���[�V�����̃g�[�^������
	float animTime = 0.0f;

	// ��U�Œ�l
	// TODO �A�j���[�V�������Ƃɐݒ�ł���悤�ɂ���
	float currentAnimSpeed = 30.0f;
	float nextAnimSpeed = 30.0f;

	// �A�j���[�V�����J�ڂ��n�߂鎞��
	float startChangingTime = 0.0f;
	// �A�j���[�V�����J�ڂɂ����鎞��
	float changingTime = 0.1f;

	bool isLoop = true;
	bool nextIsLoop = false;

	// �ؑւ̎n��/�I����ʒm���邽��
	bool isStartChangeAnimNow = false;
	bool isFinishChangeAnimNow = false;

private:
	static constexpr float minChangingTime = 0.001f;

};