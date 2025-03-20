#pragma once
#include <memory>

class ObjectBase;
class ObjectManager;

class SampleCharacter;
class SampleCamera;

struct Vector3;

class ObjectFactory
{
public:
	ObjectFactory(std::shared_ptr<ObjectManager> objManager_);

public:
	std::shared_ptr<ObjectBase> Create();
	std::shared_ptr<ObjectBase> Create(Vector3 position_, int model_handle_, int vertex_shader_handle_, int pixel_shader_handle_);

	std::shared_ptr<SampleCamera> CreateCamera(Vector3 position_);
	std::shared_ptr<SampleCharacter> CreateCharacter(Vector3 position_, Vector3 scale_, int model_handle_, int vertex_shader_handle_, int pixel_shader_handle_, int idle_anim_handle_, std::shared_ptr<SampleCamera> sample_camera_);

private:
	std::weak_ptr<ObjectManager> objectManager;
};