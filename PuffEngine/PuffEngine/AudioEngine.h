#pragma once

/*
basic audio man use fmod but see lisans aggrement
Mix_chunk is for sound effect(you can have multiple effects at once)
Mix_Music is for music (you can only lay one at a time)
*/

#include <MIX\SDL_mixer.h>

#include <string>
#include <map>

namespace PuffEngine {

class SoundEffect {
public:
	/// it can change / access this class privates
	friend class AudioEngine;
	/// @param loops: if loops == -1 loops forever
	/// otherwise play loop + 1 times
	void play(int loops = 0);
private:
	Mix_Chunk* chunk_ = nullptr;// header init
};

class Music {
public:
	/// it can change / access this class privates
	friend class AudioEngine;
	/// @param loops: if loops == -1 loops forever
	/// otherwise play it once
	void play(int loops = 1);

	/// they like global funtion 
	static void pause();
	static void stop();
	static void resume();
private:
	Mix_Music* music_ = nullptr;
};

class AudioEngine {
public:
	AudioEngine();
	~AudioEngine();

	void init();
	void destroy();

	/// first call time load then other calls give them pointers
	SoundEffect loadSoundEffect(const std::string& filePath);
	Music loadMusic(const std::string& filePath);

private:
	//cache
	std::map<std::string, Mix_Chunk*> effectMap_;
	std::map<std::string, Mix_Music*> musicMap_;

	bool isInitialzed = false;
};
}
