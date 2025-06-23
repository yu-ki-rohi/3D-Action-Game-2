#include "CameraManager.h"
#include "../Mathmatics/Vector3.h"
#include "CameraTPS.h"

CameraManager::CameraManager() :
	currentType(CameraBase::Type::TPS),
	pastType(CameraBase::Type::TPS),
	mainRate(1.0f)
{

}

Vector3 CameraManager::GetCameraPosition() const
{
	return GetCameraPositionInfo(&CameraBase::GetCameraPosition);
}

Vector3 CameraManager::GetTargetPosition() const
{
	return GetCameraPositionInfo(&CameraBase::GetTargetPosition);
}

Vector3 CameraManager::GetNearShadowAreaPos() const
{
	return GetCameraPositionInfo(&CameraBase::GetNearShadowAreaPos);
}

void CameraManager::SetCameraInfo()
{
	SetCameraPositionAndTargetAndUpVec(
		GetCameraPosition().ToVECTOR(),
		GetTargetPosition().ToVECTOR(),
		Vector3::UP.ToVECTOR()
	);
}

void CameraManager::RegisterCamera(std::shared_ptr<CameraBase> camera_)
{
	cameras.push_back(camera_);
}

Vector3 CameraManager::GetCameraPositionInfo(GetPositionFunc get_position_) const
{
	Vector3 position = Vector3::ZERO;
	for (const auto& camera : cameras)
	{
		// nullチェック
		if (!camera) { continue; }

		if (camera->GetType() == currentType)
		{
			position += (*camera.*get_position_)() * mainRate;
		}
		else if (camera->GetType() == pastType)
		{
			position += (*camera.*get_position_)() * (1.0f - mainRate);
		}
	}
	return position;
}