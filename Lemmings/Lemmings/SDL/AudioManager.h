#ifndef AUDIOMANAGER_H
#define AUDIOMANAGER_H

#include "SDL.h"

class AudioManager
{
private:
	AudioManager();

	static AudioManager* aInstance;

public:
	~AudioManager();

	static AudioManager* getInstanceAudio();

	Sint32 addAudio(const char* file);
	Sint32 getAudioID(const char* file);
	void playMusic(int audio);
	void playMusic(int audio, int loop);
	void playSound(int audio);
	void playSound(int audio, int loop);
	void pauseMusic();
	void pauseSound();
	bool isMusicPaused();
	bool isSoundPaused();
	void resumeMusic();
	void resumeSound();
};

#endif