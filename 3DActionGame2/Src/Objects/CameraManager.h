#pragma once
#include "CameraBase.h"

class CameraTPS;

class CameraManager
{
public:
	CameraManager();

public:
	// 最終的なカメラの座標を取得
	Vector3 GetCameraPosition() const;
	// 最終的なカメラの注視点座標を取得
	Vector3 GetTargetPosition() const;
	// シャドウマップ描画の中心座標を取得
	Vector3 GetNearShadowAreaPos() const;

	void SetCameraInfo();

	void RegisterCamera(std::shared_ptr<CameraBase> camera_);

private:
	// 関数の共通化のために用意
	// 割といい加減に名前をつけているので、今後変更した方がいいかも
	using GetPositionFunc = Vector3(CameraBase::*)() const;
	Vector3 GetCameraPositionInfo(GetPositionFunc get_position_) const;

private:
	CameraBase::Type currentType;
	CameraBase::Type pastType;

	// 現在メインで使用されているカメラの割合
	float mainRate;
	std::vector<std::shared_ptr<CameraBase>> cameras;

};