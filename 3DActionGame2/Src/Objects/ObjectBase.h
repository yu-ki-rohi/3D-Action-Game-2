#pragma once

class ObjectBase
{
public:
	bool IsActive()
	{
		return isActive;
	}

public:
	virtual void FixedUpdate() = 0;
	virtual void Update(float elapsed_time) = 0;
	virtual void Render() = 0;
protected:
	bool isActive = true;
};