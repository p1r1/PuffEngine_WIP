#pragma once
#include <SDL/SDL.h>
#include <GL/glew.h>
#include <string>

namespace PuffEngine {
/*Bitwise
	1 INVISIBLE
10 FULLSCREEN
100 BORDERLESS
*/
enum WindowFlags { INVISIBLE = 0x1, FULLSCREEN = 0x2, BORDERLESS = 0x4 };

class Window {
public:
	Window();
	~Window();
	int create(std::string windowName, int screenWidth, int screenHeight, unsigned int currentFlags);
	void swapBuffer();

	int getScreenWidth() { return screenWidth_; }
	int getScreenHeight() { return screenHeight_; }

	void setScreenWidth(int screenWidth) { screenWidth_ = screenWidth; }

private:
	SDL_Window* sdlWindow_;
	int screenWidth_, screenHeight_;
};

}