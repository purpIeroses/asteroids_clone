#pragma once

#include <vector>

struct Sound;
struct Music;
struct SoundImpl;

struct SoundManager
{
	SoundManager(SoundImpl &soundImpl);
	~SoundManager();
	// Play a sound, send -1 for infinite loop
	void PlaySound(const Sound& sound, int loops);
	void SetSoundVolume(const Sound& sound, int volume); 
	void PlayMusic(const Music& music, int loops);
	void StopMusic(const Music& music);
	void SetMusicVolume(const Music& music, int volume); 

	SoundImpl& impl;
};