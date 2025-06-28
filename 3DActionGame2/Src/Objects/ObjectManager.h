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
	void SetLocalTimeScale(float time_scale_);
	void SetLocalTimeScale(float time_scale_, int mask_);

private:
	std::vector<std::shared_ptr<ObjectBase>> objects;
	std::vector<std::shared_ptr<ObjectBase>> pendingStartObjects;
};