#pragma once
#include <vector>
#include <memory>
#include "ObjectBase.h"

class ObjectManager
{
public:
	void Register(std::shared_ptr<ObjectBase> obj_);
	void Erase();
	void EraseAll();

	void Start();

	void FixedUpdate();
	void Update(float elapsed_time);
	void Render();
	void RenderShadow();

public:
	void SetMonochrome(float rate_);
	void SetMonochrome(float rate_, int mask_);
	void ChangeMonochromeRequest(float initial_rate_, float target_rate_, float duration_, int mask_);

	void SetLocalTimeScale(float time_scale_);
	void SetLocalTimeScale(float time_scale_, int mask_);
	void MultiplyLocalTimeScaleBy(float multiplier_);
	void MultiplyLocalTimeScaleBy(float multiplier_, int mask_);

private:
	std::vector<std::shared_ptr<ObjectBase>> objects;
	// Start準備中のオブジェクト
	// 生成時はこちらに登録し、Start実行後にobjectsへ移動させる
	std::vector<std::shared_ptr<ObjectBase>> pendingStartObjects;
};