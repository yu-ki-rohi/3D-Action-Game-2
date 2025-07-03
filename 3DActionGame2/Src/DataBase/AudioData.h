#pragma once
#include <utility>
#include "DataKind.h"

#define MUSIC_PATTERN 3

struct AudioFileList
{
	using MusicList = std::pair<BGMKind, const char*>;

	MusicList MusicFiles[MUSIC_PATTERN][1] = {
		{
			std::make_pair(BGMKind::Main, "Res/Audio/Music/IMSLP708961-PMLP8710-04_Allegro_Con_Fuoco.mp3")
		},
		{
			std::make_pair(BGMKind::Main, "Res/Audio/Music/IMSLP725605-PMLP3607-02.03._Waltz_Of_The_Flowers.mp3")
		},
		{
			std::make_pair(BGMKind::Main, "Res/Audio/Music/IMSLP928741-PMLP126435-Vivaldi_Winter_mvt_1_Allegro_non_molto_-_The_USAF_Concert.ogg")
		}
	};

	using SoundList = std::pair<SEKind, const char*>;

	SoundList SoundFiles[1] = {
		std::make_pair(SEKind::HitSlash, "Res/Audio/Sound/ìÅåïÅEéaÇÈ07.mp3")
	};
};