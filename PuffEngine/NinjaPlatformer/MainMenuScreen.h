#pragma once

#include <PuffEngine\IGameScreen.h>
#include <PuffEngine\Window.h>
#include <PuffEngine\Camera2D.h>
#include <PuffEngine\GUI.h>

#include "ScreenIndices.h"

class MainMenuScreen : public PuffEngine::IGameScreen {
public:
	MainMenuScreen(PuffEngine::Window* window);
	~MainMenuScreen();

	virtual int getNextScreenIndex() const override;

	virtual int getPreviousScreenIndex() const override;

	virtual void build() override;

	virtual void destroy() override;

	virtual void onEntry() override;

	virtual void onExit() override;

	virtual void update() override;

	virtual void draw() override;
private:
	void initUI();
	void checkInput();

	bool onNewGameClicked(const CEGUI::EventArgs& e);
	bool editorClicked(const CEGUI::EventArgs& e);
	bool onExitClicked(const CEGUI::EventArgs& e);
	
	int nextScreenIndex_ = SCREEN_INDEX_GAMEPLAY;

	PuffEngine::Window* window_;
	PuffEngine::Camera2D camera_;
	PuffEngine::GUI gui_;
};

