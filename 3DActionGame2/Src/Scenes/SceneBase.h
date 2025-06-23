#pragma once
#include <memory>

class SceneBase : public std::enable_shared_from_this<SceneBase>
{
public:
	enum class Step
	{
		Init,
		Load,
		Update,
		Finish
	};

	enum class Type
	{
		Title,
		Game,
		Sample,
		Finish
	};
public:
	Step GetCurrentStep()
	{
		return currentStep;
	}

public:
	// �eframe�̍ŏ��ɍs���鏈��
	virtual void Start() = 0;

	virtual void FixedUpdate() = 0;
	virtual void Update(float elapsed_time_) = 0;
	virtual void Render() = 0;

	virtual void UpdateInLoading(float elapsed_time_) = 0;
	virtual void RenderInLoading() = 0;

	// Scene�̏���������
	virtual void Initialize() = 0;

	/// <summary>
	/// ����Scene�̍폜����
	/// </summary>
	/// <returns>���̃V�[��Type</returns>
	virtual Type Delete() = 0;

	virtual void SuccessJustAvoid(){}

protected:
	Step currentStep = Step::Init;

};