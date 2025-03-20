#include "ObjectManager.h"
#include "ObjectBase.h"

void ObjectManager::Register(std::shared_ptr<ObjectBase> obj_)
{
	objects.push_back(obj_);
}

void ObjectManager::Erase()
{
	for (auto itr = objects.begin(); itr != objects.end(); )
	{
		if (*itr != nullptr && (*itr)->IsActive() == false)
		{
			itr = objects.erase(itr);
		}
		else
		{
			itr++;
		}
	}
}

void ObjectManager::EraseAll()
{
	objects.clear();
}

void ObjectManager::FixedUpdate()
{
	for (auto& obj : objects)
	{
		obj->FixedUpdate();
	}
}

void ObjectManager::Update(float elapsed_time)
{
	for (auto& obj : objects)
	{
		obj->Update(elapsed_time);
	}
}

void ObjectManager::Render()
{
	for (auto& obj : objects)
	{
		obj->Render();
	}
}