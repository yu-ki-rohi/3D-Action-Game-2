#include "SimpleObserver.h"

unsigned char SimpleObserver::GetUnsignedChar()
{
	return uc;
}

void SimpleObserver::GetFloatx2(float& f1_, float& f2_)
{
	f1_ = f1;
	f2_ = f2;
}

	
void SimpleObserver::Update()
{

}
	
void SimpleObserver::Update(unsigned char uc_) 
{
	uc = uc_;
}
	
void SimpleObserver::Update(float f1_, float f2_)
{
	f1 = f1_;
	f2 = f2_;
}
