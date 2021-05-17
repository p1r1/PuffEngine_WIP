#include "App.h"
#include <PuffEngine\ScreenList.h>


App::App() {
}


App::~App() {
	
}

void App::onInit() {
}

void App::addScreens() {
	mainMenuScreen_ = std::make_unique<MainMenuScreen>(&window_);
	gamePlayScreen_ = std::make_unique<GamePlayScreen>(&window_);
	editorScreen_ = std::make_unique<EditorScreen>(&window_);
	
	//addscreen need pointer
	screenList_->addScreen(mainMenuScreen_.get());  // index 0 = mainmenu 
	screenList_->addScreen(gamePlayScreen_.get());	// 1 = gameplayscreen
	screenList_->addScreen(editorScreen_.get());	// 2 = editor

	// starting screen
	screenList_->setScreen(mainMenuScreen_->getScreenIndex());
}

void App::onExit() {
}
