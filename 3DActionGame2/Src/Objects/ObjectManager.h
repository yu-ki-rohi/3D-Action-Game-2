#pragma once
#include <vector>
#include <memory>

class ObjectBase;

class ObjectManager
{
public:
	void Register(std::shared_ptr<ObjectBase> obj_);
	void Erase();
	void EraseAll();

	void FixedUpdate();
	void Update(float elapsed_time);
	void Render();
private:
	std::vector<std::shared_ptr<ObjectBase>> objects;
};