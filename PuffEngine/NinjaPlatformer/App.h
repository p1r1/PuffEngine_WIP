#pragma once
#include "MainMenuScreen.h"
#include "GamePlayScreen.h"
#include "EditorScreen.h"
#include <PuffEngine\IMainGame.h>

//inherits from engines maingame interface class
class App : public PuffEngine::IMainGame{
public:
	App();
	~App();

	// Callde on initialization
	virtual void onInit() override;
	// For adding all screens
	virtual void addScreens() override;
	// Called when exiting
	virtual void onExit() override;

private:
	std::unique_ptr<MainMenuScreen> mainMenuScreen_ = nullptr;
	std::unique_ptr<GamePlayScreen> gamePlayScreen_ = nullptr;
	std::unique_ptr<EditorScreen> editorScreen_ = nullptr;

};

