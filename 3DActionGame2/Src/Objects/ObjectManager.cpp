#include "ObjectManager.h"

void ObjectManager::Register(std::shared_ptr<ObjectBase> obj_)
{
	pendingStartObjects.push_back(obj_);
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

void ObjectManager::Start()
{
	for (auto& obj : pendingStartObjects)
	{
		if (obj == nullptr) { continue; }
		obj->Start();
		objects.push_back(obj);
	}
	pendingStartObjects.clear();
}

void ObjectManager::FixedUpdate()
{
	for (auto& obj : objects)
	{
		if (obj == nullptr || !obj->IsActive()) { continue; }
		obj->FixedUpdate();
	}
}

void ObjectManager::Update(float elapsed_time)
{
	for (auto& obj : objects)
	{
		if (obj == nullptr || !obj->IsActive()) { continue; }
		obj->Update(elapsed_time);
	}
}

void ObjectManager::Render()
{
	for (auto& obj : objects)
	{
		if (obj == nullptr || !obj->IsActive()) { continue; }
		obj->Render();
	}
}

void ObjectManager::RenderShadow()
{
	for (auto& obj : objects)
	{
		if (obj == nullptr || !obj->IsActive()) { continue; }
		obj->RenderShadow();
	}
}

void ObjectManager::SetMonochrome(float rate_)
{
	for (auto& obj : objects)
	{
		obj->SetMonochrome(rate_);
	}
}
void ObjectManager::SetMonochrome(float rate_, int mask_)
{
	for (auto& obj : objects)
	{
		if (((int)obj->GetTag() & mask_) == 0) { continue; }
		obj->SetMonochrome(rate_);
	}
}

void ObjectManager::SetLocalTimeScale(float time_scale_)
{
	for (auto& obj : objects)
	{
		obj->SetLocalTimeScale(time_scale_);
	}
}

void ObjectManager::SetLocalTimeScale(float time_scale_, int mask_)
{
	for (auto& obj : objects)
	{
		if (((int)obj->GetTag() & mask_) == 0) { continue; }
		obj->SetLocalTimeScale(time_scale_);
	}
}

void ObjectManager::MultiplyLocalTimeScaleBy(float multiplier_)
{
	for (auto& obj : objects)
	{
		obj->MultiplyLocalTimeScaleBy(multiplier_);
	}
}

void ObjectManager::MultiplyLocalTimeScaleBy(float multiplier_, int mask_)
{
	for (auto& obj : objects)
	{
		if (((int)obj->GetTag() & mask_) == 0) { continue; }
		obj->MultiplyLocalTimeScaleBy(multiplier_);
	}
}