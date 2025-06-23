#pragma once
#include "CameraBase.h"

class CameraTPS;

class CameraManager
{
public:
	CameraManager();

public:
	Vector3 GetCameraPosition() const;
	Vector3 GetTargetPosition() const;

	Vector3 GetNearShadowAreaPos() const;

	void SetCameraInfo();

	void RegisterCamera(std::shared_ptr<CameraBase> camera_);

private:
	// �֐��̋��ʉ��̂��߂ɗp��
	// ���Ƃ��������ɖ��O�����Ă���̂ŁA����ύX����������������
	using GetPositionFunc = Vector3(CameraBase::*)() const;
	Vector3 GetCameraPositionInfo(GetPositionFunc get_position_) const;

private:
	CameraBase::Type currentType;
	CameraBase::Type pastType;

	float mainRate;
	std::vector<std::shared_ptr<CameraBase>> cameras;

};