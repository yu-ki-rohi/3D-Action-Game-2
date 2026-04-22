#pragma once

// オブザーバーパターンの実装のため
// 観察者

class ObserverBase
{
public:
	virtual ~ObserverBase() = default;

public:
	virtual void Update() = 0;
	virtual void Update(unsigned char uc_) = 0;
	virtual void Update(float f1_, float f2_) = 0;

private:
};
