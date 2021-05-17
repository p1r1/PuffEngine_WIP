#pragma once

#include "PuffEngine.h"
#include "InputManager.h"
#include "Window.h"

#include <memory>

namespace PuffEngine {
class IGameScreen;
class ScreenList;

class IMainGame {
public:
	IMainGame();
	virtual ~IMainGame();

	// Runs ad initalizes game
	void run();
	// Exits the game
	void exitGame();

	// Callde on initialization
	virtual void onInit() = 0;
	// For adding all screens
	virtual void addScreens() = 0;
	// Called when exiting
	virtual void onExit() = 0;

	void onSDLEvent(SDL_Event& evnt);

	// getters
	const float getFps() const { return fps_; }
	
	InputManager inputManager;

protected:
	// Custom update function
	virtual void update();
	//Custom render function
	virtual void draw();

	bool init();
	bool initSystems();

	std::unique_ptr<ScreenList> screenList_ = nullptr;
	IGameScreen* currentScreen_  = nullptr;
	bool isRunning_ = false;
	float fps_ = 0.0f;
	Window window_;

};


}
