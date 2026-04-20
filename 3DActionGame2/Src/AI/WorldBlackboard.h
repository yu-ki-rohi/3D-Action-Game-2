#pragma once

#include <memory>

struct Vector3;
class Transform;

class WorldBlackboard
{
public:
	WorldBlackboard(std::shared_ptr<const Transform> player_transform_);

public:
	const Vector3& GetPlayerPosition() const;


private:
	std::shared_ptr<const Transform> playerTransform;

};