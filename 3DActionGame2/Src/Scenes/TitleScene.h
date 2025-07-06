#pragma once
#include "SceneBase.h"
#include <memory>
#include "../Input/KeyConfig.h"

class ObjectManager;
class ObjectFactory;

class SimpleObserver;

class TitleScene : public SceneBase
{
public:
	TitleScene();
	~TitleScene();

public:
	void Start() override {}

	void FixedUpdate() override;
	void Update(float elapsed_time_) override;
	void Render() override;

	void UpdateInLoading(float elapsed_time_) override;
	void RenderInLoading() override;

	void Initialize() override;
	Type Delete() override;

private:
	void FinishScene();

	void OnDicede();
	void OnCancel();
	void ChangeMenuIndex();
	void IncreaseMenuIndex();
	void DecreaseMenuIndex();

	// キーコンフィグ登録用
	// この辺りもっとスマートな方法を探したい
	void SetKeyConfigDpadUp();
	void SetKeyConfigDpadDown();
	void SetKeyConfigDpadLeft();
	void SetKeyConfigDpadRight();
	void SetKeyConfigLeftThumb();
	void SetKeyConfigRightThumb();
	void SetKeyConfigLeftShoulder();
	void SetKeyConfigRightShoulder();
	void SetKeyConfigA();
	void SetKeyConfigB();
	void SetKeyConfigX();
	void SetKeyConfigY();
	void SetKeyConfigLeftTrigger();
	void SetKeyConfigRightTrigger();

	void SetKeyConfig(unsigned char button_);

	void RegisterInputBehavior();

private:
	enum class State
	{
		Title,
		Option
	};

private:
	static constexpr char titleChoicesNum = 2;
	static constexpr char optionChoicesNum = 6;

private:
	char choicesIndex;
	char choicesNum;
	std::shared_ptr<SimpleObserver> so;
	State state;
	KeyConfig::Tag configTag;

};