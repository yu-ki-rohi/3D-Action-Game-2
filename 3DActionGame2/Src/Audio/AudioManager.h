#pragma once
#include "../DataBase/DataKind.h"
#include <memory>
#include <unordered_map>

struct MusicResource;
struct SoundResource;

// 一旦シングルトンで作る
// 最終的にどうするかは検討

class AudioManager
{
public:
	// 音楽再生
	void PlayMusic(BGMKind kind_);
	void PlaySoundEffect(SEKind kind_, bool is_loop_ = false);

	void SetVolume(int volume_pal_);
	void SetVolume(int volume_pal_, BGMKind kind_);
	void SetVolume(int volume_pal_, SEKind kind_);

	// リソースを登録
	void RegisterMusic(std::shared_ptr<std::unordered_map<BGMKind, std::shared_ptr<MusicResource>>> music_resource_);
	void RegisterSound(std::shared_ptr<std::unordered_map<SEKind, std::shared_ptr<SoundResource>>> sound_resource_);

	// リソースの破棄
	void DestructResource();

private:
	std::shared_ptr<std::unordered_map<BGMKind, std::shared_ptr<MusicResource>>> musicResource;
	std::shared_ptr<std::unordered_map<SEKind, std::shared_ptr<SoundResource>>> soundResource;


#pragma region シングルトン登録
public:
	static AudioManager& Instance()
	{
		static AudioManager instance;
		return instance;
	}

	AudioManager(const AudioManager&) = delete;
	AudioManager& operator=(const AudioManager&) = delete;
	AudioManager(AudioManager&&) = delete;
	AudioManager& operator=(AudioManager&&) = delete;

private:
	AudioManager();
	~AudioManager() = default;
#pragma endregion
};