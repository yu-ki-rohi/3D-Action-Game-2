#pragma once

class ObserverBase
{
public:
	virtual void Update() = 0;
	virtual void Update(unsigned char uc_) = 0;
	virtual void Update(float f1_, float f2_) = 0;

private:
};
