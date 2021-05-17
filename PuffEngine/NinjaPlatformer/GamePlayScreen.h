#pragma once

#include "Box.h"
#include "PlayerNinja.h"

#include <PuffEngine\IGameScreen.h>
#include <PuffEngine\SpriteBatch.h>
#include <PuffEngine\GLSLProgram.h>
#include <PuffEngine\Camera2D.h>
#include <PuffEngine\GLTexture.h>
#include <PuffEngine\Window.h>
#include <PuffEngine\DebugRenderer.h>
#include <PuffEngine\GUI.h>

#include <vector>

#include <Box2D\Box2D.h>


//inherits from igamescreens
class GamePlayScreen : public PuffEngine::IGameScreen{
public:
	GamePlayScreen(PuffEngine::Window* window);
	~GamePlayScreen();

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


	bool onMainMenuClicked(const CEGUI::EventArgs& e);
	bool onExitClicked(const CEGUI::EventArgs& e);

	PuffEngine::GLSLProgram textureProgram_;
	PuffEngine::GLSLProgram lightProgram_;
	PuffEngine::Camera2D camera_;
	PuffEngine::SpriteBatch spriteBatch_;
	PuffEngine::GLTexture texture_;
	PuffEngine::Window* window_;
	PuffEngine::DebugRenderer debugRenderer_;
	PuffEngine::GUI gui_;

	PlayerNinja player_;

	std::unique_ptr<b2World> world_;
	std::vector<Box> boxes_;

	bool renderDebug_ = false;
};

