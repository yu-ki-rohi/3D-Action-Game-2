#include "AudioManager.h"
#include <DxLib.h>
#include "../Assets/AudioResource.h"

void AudioManager::PlayMusic(BGMKind kind_)
{
	if (!musicResource || !(*musicResource)[kind_]) { return; }
	PlaySoundMem((*musicResource)[kind_]->Handle, DX_PLAYTYPE_LOOP);
}

void AudioManager::PlaySoundEffect(SEKind kind_, bool is_loop_)
{
	if (!soundResource || !(*soundResource)[kind_]) { return; }
	if (is_loop_)
	{
		PlaySoundMem((*soundResource)[kind_]->Handle, DX_PLAYTYPE_LOOP);
	}
	else
	{
		PlaySoundMem((*soundResource)[kind_]->Handle, DX_PLAYTYPE_BACK);
	}
}

void AudioManager::SetVolume(int volume_pal_)
{
	if (musicResource)
	{
		for (auto music : *musicResource)
		{
			if (!music.second) { continue; }
			ChangeVolumeSoundMem(volume_pal_, music.second->Handle);
		}
	}

	if (soundResource)
	{
		for (auto sound : *soundResource)
		{
			if (!sound.second) { continue; }
			ChangeVolumeSoundMem(volume_pal_, sound.second->Handle);
		}
	}
}

void AudioManager::SetVolume(int volume_pal_, BGMKind kind_)
{
	if (!musicResource || !(*musicResource)[kind_]) { return; }
	ChangeVolumeSoundMem(volume_pal_, (*musicResource)[kind_]->Handle);
}

void AudioManager::SetVolume(int volume_pal_, SEKind kind_)
{
	if (!soundResource || !(*soundResource)[kind_]) { return; }
	ChangeVolumeSoundMem(volume_pal_, (*soundResource)[kind_]->Handle);
}

void AudioManager::RegisterMusic(std::shared_ptr<std::unordered_map<BGMKind, std::shared_ptr<MusicResource>>> music_resource_)
{
	musicResource = music_resource_;
}

void AudioManager::RegisterSound(std::shared_ptr<std::unordered_map<SEKind, std::shared_ptr<SoundResource>>> sound_resource_)
{
	soundResource = sound_resource_;
}

void AudioManager::DestructResource()
{
	musicResource = nullptr;
	soundResource = nullptr;
}

AudioManager::AudioManager()
{

}