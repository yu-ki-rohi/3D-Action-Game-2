#pragma once

class SceneBase
{
public:
	enum class Step
	{
		Init,
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
	virtual void FixedUpdate() = 0;
	virtual void Update(float elapsed_time_) = 0;
	virtual void Render() = 0;

	virtual void Initialize() = 0;

	/// <summary>
	/// ����Scene�̍폜����
	/// </summary>
	/// <returns>���̃V�[��Type</returns>
	virtual Type Delete() = 0;

protected:
	Step currentStep = Step::Init;

};