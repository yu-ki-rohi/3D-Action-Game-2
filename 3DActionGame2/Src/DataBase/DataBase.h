#pragma once
#include <memory>
#include <vector>

struct AnimationFileList;
struct ModelFileList;
struct EffectFileList;
struct ShaderFileList;

struct AudioFileList;

struct AnimationSpeedList;

// 現状はファイルパスの保持に使用
class DataBase
{
public:
	std::shared_ptr<AnimationFileList> GetAnimationFileList();
	std::shared_ptr<ModelFileList> GetModelFileList();
	std::shared_ptr<EffectFileList> GetEffectFileList();
	std::shared_ptr<ShaderFileList> GetShaderFileList();
	
	std::shared_ptr<AudioFileList> GetAudioFileList();

	std::shared_ptr<AnimationSpeedList> GetAnimationSpeedList();

private:



#pragma region シングルトン登録
public:
	static DataBase& Instance()
	{
		static DataBase instance;
		return instance;
	}

	DataBase(const DataBase&) = delete;
	DataBase& operator=(const DataBase&) = delete;
	DataBase(DataBase&&) = delete;
	DataBase& operator=(DataBase&&) = delete;

private:
	DataBase() = default;
	~DataBase() = default;
#pragma endregion

};