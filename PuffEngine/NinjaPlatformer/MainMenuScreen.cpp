#include "MainMenuScreen.h"

MainMenuScreen::MainMenuScreen(PuffEngine::Window* window) : 
	window_(window){
	screenIndex_ = SCREEN_INDEX_MAINMENU;
}


MainMenuScreen::~MainMenuScreen() {
}

int MainMenuScreen::getNextScreenIndex() const {
	return nextScreenIndex_;
}

int MainMenuScreen::getPreviousScreenIndex() const {
	return SCREEN_INDEX_NO_SCREEN;
}

void MainMenuScreen::build() {

}

void MainMenuScreen::destroy() {

}

void MainMenuScreen::onEntry() {
	//init camera
	camera_.init(window_->getScreenWidth(), window_->getScreenHeight());
	camera_.setScale(32.0f);//32 pixel for meter

	initUI();
}

void MainMenuScreen::onExit() {
	gui_.destroy();
}

void MainMenuScreen::update() {
	camera_.update();
	checkInput();
}

void MainMenuScreen::draw() {
	// clear buffers and set background
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0f, 0.0f, 0.2f, 1.0f);

	gui_.draw();
}

void MainMenuScreen::initUI() {
	// Init the UI
	gui_.init("GUI"); //static class (not any more)
	gui_.loadScheme("TaharezLook.scheme");
	// do not include .font
	gui_.setFont("DejaVuSans-10"); 

	// new game button setup
	CEGUI::PushButton* newGameButton = static_cast<CEGUI::PushButton*>(gui_.createWidget("TaharezLook/Button", glm::vec4(0.45f, 0.5f, 0.1f, 0.05f), glm::vec4(0.0f), "NewGameButton"));
	newGameButton->setText("New Game");
	// set the event to be called when clicked
	newGameButton->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&MainMenuScreen::onNewGameClicked, this));

	// editor button setup
	CEGUI::PushButton* editorButton = static_cast<CEGUI::PushButton*>(gui_.createWidget("TaharezLook/Button", glm::vec4(0.45f, 0.65f, 0.1f, 0.05f), glm::vec4(0.0f), "EditorButton"));
	editorButton->setText("Level Editor");
	// set the event to be called when clicked
	editorButton->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&MainMenuScreen::editorClicked, this));

	// exit button setup
	CEGUI::PushButton* exitButton = static_cast<CEGUI::PushButton*>(gui_.createWidget("TaharezLook/Button", glm::vec4(0.45f, 0.75f, 0.1f, 0.05f), glm::vec4(0.0f), "ExitButton"));
	exitButton->setText("ExitGame");
	// set the event to be called when clicked
	exitButton->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&MainMenuScreen::onExitClicked, this));

	//mouse cursor setup
	gui_.setMouseCursor("TaharezLook/MouseArrow");
	gui_.showMouseCursor();
	SDL_ShowCursor(0);
}

void MainMenuScreen::checkInput() {
	SDL_Event evnt;
	while (SDL_PollEvent(&evnt)) {
		gui_.onSDLEvent(evnt);

		switch (evnt.type) {
		case SDL_QUIT:
			onExitClicked(CEGUI::EventArgs());
		default:
			break;
		}
	}
}

bool MainMenuScreen::onNewGameClicked(const CEGUI::EventArgs & e) {
	nextScreenIndex_ = SCREEN_INDEX_GAMEPLAY;
	currentState_ = PuffEngine::ScreenState::CHANGE_NEXT;
	return true;
}

bool MainMenuScreen::editorClicked(const CEGUI::EventArgs & e) {
	nextScreenIndex_ = SCREEN_INDEX_EDITOR;
	currentState_ = PuffEngine::ScreenState::CHANGE_NEXT;
	return true;
}

bool MainMenuScreen::onExitClicked(const CEGUI::EventArgs& e) {
	currentState_ = PuffEngine::ScreenState::EXIT_APPLICATION;
	return true;
}
