#include "Window.h"
#include "PuffErrors.h"

namespace PuffEngine {
Window::Window() {
}


Window::~Window() {
}

int Window::create(std::string windowName, int screenWidth, int screenHeight, unsigned int currentFlags) {
	Uint32 flags = SDL_WINDOW_OPENGL;
	screenWidth_ = screenWidth;
	screenHeight_ = screenHeight;

	if (currentFlags & INVISIBLE) {
		flags |= SDL_WINDOW_HIDDEN;
	}
	if (currentFlags & FULLSCREEN) {
		flags |= SDL_WINDOW_FULLSCREEN_DESKTOP;
	}
	if (currentFlags & BORDERLESS) {
		flags |= SDL_WINDOW_BORDERLESS;
	}

	//pointing to graphics window 
	sdlWindow_ = SDL_CreateWindow(windowName.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, screenWidth, screenHeight, flags);
	if (sdlWindow_ == nullptr) {
		fatalError("SDL window could not be created!");
	}

	SDL_GLContext glContex = SDL_GL_CreateContext(sdlWindow_);
	if (glContex == nullptr) {
		fatalError("SDL_GL contex xould not be created!");
	}

	GLenum error = glewInit();
	if (error != GLEW_OK) {
		fatalError("Glew could not initialzed!");
	}

	//check the opengl version
	printf("*** OPENGL VERSION %s ***\n", glGetString(GL_VERSION));

	glClearColor(0.0f, 0.0f, 1.0f, 1.0f); // background color rgba

	//VSYNC off -> 0, on->1
	SDL_GL_SetSwapInterval(0);

	//enable ALPHA blending
	glEnable(GL_BLEND);
	//1-> source factor 
	//2 -> destination factor
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	return 0;
}
void Window::swapBuffer() {
	//Swap window (doublebuffer) for smoothness
	SDL_GL_SwapWindow(sdlWindow_);
}

}