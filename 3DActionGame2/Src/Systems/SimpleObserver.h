#pragma once
#include "ObserverBase.h"

class SimpleObserver : public ObserverBase
{
public:
	unsigned char GetUnsignedChar();
	void GetFloatx2(float& f1_, float& f2_);

public:
	void Update() override;
	void Update(unsigned char uc_) override;
	void Update(float f1_, float f2_) override;

private:
	unsigned char uc = 0;
	float f1 = 0;
	float f2 = 0;
};