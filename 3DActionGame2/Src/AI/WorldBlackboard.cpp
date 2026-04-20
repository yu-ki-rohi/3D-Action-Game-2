#include "WorldBlackboard.h"
#include "../Objects/Components/Transform.h"

WorldBlackboard::WorldBlackboard(std::shared_ptr<const Transform> player_transform_) :
	playerTransform(player_transform_)
{

}

const Vector3& WorldBlackboard::GetPlayerPosition() const
{
	return  playerTransform->Position;
}