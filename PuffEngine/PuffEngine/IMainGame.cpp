#include "IMainGame.h"
#include "Timing.h"
#include "IGameScreen.h"
#include "ScreenList.h"

namespace PuffEngine {

IMainGame::IMainGame() {
	screenList_ = std::make_unique<ScreenList>(this);
}


IMainGame::~IMainGame() {
	//empty
}

void IMainGame::run() {
	if (!init()) return;

	//game loop
	FpsLimiter fpsLimiter;
	fpsLimiter.setMaxFPS(60.0f);

	isRunning_ = true;
	while (isRunning_) {
		fpsLimiter.begin();
		
		//call custom update and draw methods
		inputManager.update();
		update();
		if (isRunning_){
			draw();

			fps_ = fpsLimiter.end();
			window_.swapBuffer();
		}
		
	}

}

void IMainGame::exitGame() {
	currentScreen_->onExit();
	if (screenList_) {
		screenList_->destroy();
		screenList_.reset();
	}
	isRunning_ = false;
}

void IMainGame::update() {
	if (currentScreen_) {
		switch (currentScreen_->getState()) {
		case ScreenState::RUNNING:
			currentScreen_->update();
			break;
		case ScreenState::CHANGE_NEXT:
			currentScreen_->onExit();
			currentScreen_ = screenList_->moveNext();
			if (currentScreen_) {
				currentScreen_->setRunning();
				currentScreen_->onEntry();
			}
			break;
		case ScreenState::CHANGE_PREVIOUS:
			currentScreen_->onExit();
			currentScreen_ = screenList_->movePrevious();
			if (currentScreen_) {
				currentScreen_->setRunning();
				currentScreen_->onEntry();
			}
			break;
		case ScreenState::EXIT_APPLICATION:
			exitGame();
			break;
		default:
			break;
		}
	}
	else {
		exitGame();
	}
}

void IMainGame::draw() {
	glViewport(0, 0, window_.getScreenWidth(), window_.getScreenHeight());
	if (currentScreen_ && currentScreen_->getState() == ScreenState::RUNNING) {
		currentScreen_->draw();
	}
	
}

void IMainGame::onSDLEvent(SDL_Event& evnt) {
	switch (evnt.type) {
	case SDL_QUIT:
		exitGame();
		break;
	case SDL_MOUSEMOTION:
		inputManager.setMouseCoords((float)evnt.motion.x, (float)evnt.motion.y);
		break;
	case SDL_KEYDOWN:
		inputManager.pressKey(evnt.key.keysym.sym);
		break;
	case SDL_KEYUP:
		inputManager.releaseKey(evnt.key.keysym.sym);
		break;
	case SDL_MOUSEBUTTONDOWN:
		inputManager.pressKey(evnt.button.button);
		break;
	case SDL_MOUSEBUTTONUP:
		inputManager.releaseKey(evnt.button.button);
		break;
	}
}

bool IMainGame::init() {
	PuffEngine::init();
	SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
	if (!initSystems()) return false;

	onInit();

	addScreens();
	currentScreen_ = screenList_->getCurrent();
	currentScreen_->onEntry();
	currentScreen_->setRunning();

	return true;
}

bool IMainGame::initSystems() {
	window_.create("Default", 1024, 600, 0);
	return true;
}

}
