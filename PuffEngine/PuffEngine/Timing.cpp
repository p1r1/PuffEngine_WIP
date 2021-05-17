#include "Timing.h"
#include <SDL/SDL.h>

namespace PuffEngine {
FpsLimiter::FpsLimiter() {
}

void FpsLimiter::init(float maxFPS) {
	setMaxFPS(maxFPS);
}


void FpsLimiter::setMaxFPS(float maxFPS) {
	maxFPS_ = maxFPS;
}

void FpsLimiter::begin() {
	startTicks_ = SDL_GetTicks();
}

float FpsLimiter::end() {
	calculateFPS();
	//limit fps to the max fps FPS LIMITER
	float frameTicks = SDL_GetTicks() - startTicks_;
	if ((1000.0f / maxFPS_) > frameTicks) {
		SDL_Delay((1000.0f / maxFPS_) - frameTicks);
	}
	return fps_;
}

void FpsLimiter::calculateFPS() {
	//static -> initialize once
	static const int NUM_SAMPLES = 10;
	static float frameTimes[NUM_SAMPLES];
	static int currentFrame = 0;

	static float prevTicks = SDL_GetTicks();

	float curentTicks;
	curentTicks = SDL_GetTicks();

	frameTime_ = curentTicks - prevTicks;
	//fps = 1000/frametime_ - if not 0
	frameTimes[currentFrame % NUM_SAMPLES] = frameTime_;
	prevTicks = curentTicks;

	int count;
	currentFrame++;
	if (currentFrame < NUM_SAMPLES) {
		count = currentFrame;
	}
	else {
		count = NUM_SAMPLES;
	}

	float frameTimeAverage = 0;
	for (int i = 0; i < count; i++) {
		frameTimeAverage += frameTimes[i];
	}
	frameTimeAverage /= count;
	if (frameTimeAverage > 0) {
		fps_ = 1000.0f / frameTimeAverage;
	}
	else {
		fps_ = 60.0f;
	}
}

}