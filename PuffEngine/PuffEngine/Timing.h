#pragma once

namespace PuffEngine {


class FpsLimiter {
public:
	FpsLimiter();

	void init(float maxFPS);
	void setMaxFPS(float maxFPS);
	void begin();
	float end(); // return fps


private:
	void calculateFPS();

	float fps_;
	float maxFPS_;
	float frameTime_;
	unsigned int startTicks_;
};

}