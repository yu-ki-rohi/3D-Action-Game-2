#include "Subject.h"
#include "ObserverBase.h"

void Subject::AddObserver(std::weak_ptr<ObserverBase> observer_)
{
	observers.push_back(observer_);
}

void Subject::RemoveObserver(std::shared_ptr<ObserverBase> observer_)
{
	for (auto itr = observers.begin(); itr != observers.end(); )
	{
		std::shared_ptr<ObserverBase> observer = itr->lock();
		if (observer != nullptr && observer == observer_)
		{
			itr = observers.erase(itr);
		}
		else
		{
			itr++;
		}
	}
}

void Subject::NotifyObservers()
{
	for (auto& weakObserver : observers) 
	{
		std::shared_ptr<ObserverBase> observer = weakObserver.lock();
		if (observer == nullptr) continue;
		observer->Update();
	}
}

void Subject::NotifyObservers(unsigned char uc_)
{
	for (auto& weakObserver : observers) 
	{
		std::shared_ptr<ObserverBase> observer = weakObserver.lock();
		if (observer == nullptr) continue;
		observer->Update(uc_);
	}
}

void Subject::NotifyObservers(float f1_, float f2_)
{
	for (auto& weakObserver : observers) 
	{
		std::shared_ptr<ObserverBase> observer = weakObserver.lock();
		if (observer == nullptr) continue;
		observer->Update(f1_, f2_);
	}
}

void Subject::Clear()
{
	observers.clear();
}