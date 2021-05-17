#include "AudioEngine.h"

#include "PuffErrors.h"


namespace PuffEngine {

void SoundEffect::play(int loops /*= 0*/) {
	/// channel(max 8 channel- 8 sound play one time) -1(first channel)
	/// which type of loop you want
	if (Mix_PlayChannel(-1, chunk_, loops) == -1) {
		// may be loop trough all channells and play it in empty ones
		if (Mix_PlayChannel(0, chunk_, loops) == -1) {
			fatalError("Mix_PlayChannel error:" + std::string(Mix_GetError()));
		}
	}
}

void Music::play(int loops /*= 1*/) {
	/// plays the music file
	if (Mix_PlayMusic(music_, loops) == -1) {
		fatalError("Mix_PlayChannel error:" + std::string(Mix_GetError()));
	}
}

void Music::pause() {
	Mix_PauseMusic();
}

void Music::stop() {
	Mix_HaltMusic();
}

void Music::resume() {
	Mix_ResumeMusic();
}

//Engine
AudioEngine::AudioEngine() {
}


AudioEngine::~AudioEngine() {
	destroy();
}

void AudioEngine::init() {
	if (isInitialzed) {
		fatalError("Tried to initialize Audio Engine twice\n");
	}
	/// bit wise operation between 
	/// MIX_INIT_FAC, MIX_INIT_MOD, MIX_INIT_MP3, MIX_INIT_OGG
	if (Mix_Init(MIX_INIT_MP3 | MIX_INIT_OGG) == -1) { //call after engine init
		fatalError("Mix_Init error:" + std::string(Mix_GetError()));
	}

	/// initialize frequency, format
	/// inits channel(stereo(2), mono(1)), 
	/// inits number of bytes play it one time
	/// for mp3 * 2 frequency
	if (Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 2, 1024) == -1) {
		fatalError("Mix_OpenAudio error:" + std::string(Mix_GetError()));
	}

	isInitialzed = true;
}

void AudioEngine::destroy() {
	if (isInitialzed) {
		isInitialzed = false;
		/*
		/// do it with for
		for (auto it = effectMap_.begin(); it != effectMap_.end(); it++) {
			Mix_FreeChunk(it->second);
		}
		*/
		/// do it with foreach
		for (auto& it : effectMap_) { ///< put things which have begin and end 
			Mix_FreeChunk(it.second);
		}
		/// do it with foreach
		for (auto& it : musicMap_) { ///< put things which have begin and end 
			Mix_FreeMusic(it.second);
		}

		effectMap_.clear();
		musicMap_.clear();

		/// quit audio engine
		Mix_CloseAudio(); ///< opposite of mix_openAudio
		Mix_Quit; ///< opposite of mix_init
	}
}

SoundEffect AudioEngine::loadSoundEffect(const std::string& filePath) {
	SoundEffect soundEffect;
	//try to find audio in the cache
	auto it = effectMap_.find(filePath);
	if (it == effectMap_.end()) {
		//failed to find it must be loaded first time
		Mix_Chunk* chunk = Mix_LoadWAV(filePath.c_str());// it is not just load wav it is for all of them
		if (chunk == nullptr) {//errors
			fatalError("Mix_LoadWAV error:" + std::string(Mix_GetError()));
		}
		//load sound effect to cahce
		soundEffect.chunk_ = chunk;
		effectMap_[filePath] = chunk;//map
	}
	else {
		// it is already cahced
		soundEffect.chunk_ = it->second;
	}

	return	soundEffect;
}
Music AudioEngine::loadMusic(const std::string& filePath) {
	Music music;
	//try to find audio in the cache
	auto it = musicMap_.find(filePath);
	if (it == musicMap_.end()) {
		//failed to find it must be loaded first time
		Mix_Music* mixMusic = Mix_LoadMUS(filePath.c_str());// it is not just load wav it is for all of them
		if (mixMusic == nullptr) {//errors
			fatalError("Mix_LoadMUS error:" + std::string(Mix_GetError()));
		}
		//load sound effect to cahce
		music.music_ = mixMusic;
		musicMap_[filePath] = mixMusic;//map
	}
	else {
		// it is already cahced
		music.music_ = it->second;
	}

	return	music;
}

}