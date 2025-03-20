#pragma once
#include <vector>
#include <memory>

class ObserverBase;
class Subject
{
public:
	void AddObserver(std::weak_ptr<ObserverBase> observer_);
	void RemoveObserver(std::shared_ptr<ObserverBase> observer_);

	void NotifyObservers();
	void NotifyObservers(unsigned char uc_);
	void NotifyObservers(float f1_, float f2_);

	void Clear();
private:
	std::vector<std::weak_ptr<ObserverBase>> observers;
};