#pragma once

#include <PuffEngine\IGameScreen.h>
#include <PuffEngine\Window.h>
#include <PuffEngine\Camera2D.h>
#include <PuffEngine\GUI.h>
#include <PuffEngine\GLSLProgram.h>
#include <PuffEngine\SpriteBatch.h>
#include <PuffEngine\SpriteFont.h>
#include <PuffEngine\GLTexture.h>

#include "ScreenIndices.h"

enum class PhysicsMode {
	RIGID,
	DYNAMIC
};

class EditorScreen : public PuffEngine::IGameScreen {
public:
	EditorScreen(PuffEngine::Window* window);
	~EditorScreen();

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

	bool onExitClicked(const CEGUI::EventArgs& e);
	bool onColorPickerRedChange(const CEGUI::EventArgs& e);
	bool onColorPickerGreenChange(const CEGUI::EventArgs& e);
	bool onColorPickerBlueChange(const CEGUI::EventArgs& e);
	bool onRigidMouseClick(const CEGUI::EventArgs& e);
	bool onDynamicMouseClick(const CEGUI::EventArgs& e);

	CEGUI::Slider* rSlider_ = nullptr;
	CEGUI::Slider* gSlider_ = nullptr;
	CEGUI::Slider* bSlider_ = nullptr;
	CEGUI::RadioButton* rigidRadioButton_ = nullptr;
	CEGUI::RadioButton* dynamicRadioButton_ = nullptr;
	
	float colorPickerRed_ = 255.0f;
	float colorPickerGreen_ = 255.0f;
	float colorPickerBlue_ = 255.0f;
	
	PuffEngine::Window* window_;
	PuffEngine::Camera2D camera_;
	PuffEngine::GUI gui_;
	PuffEngine::GLSLProgram textureProgram_;
	PuffEngine::SpriteBatch spriteBatch_;
	PuffEngine::GLTexture blankTexture_;
	PuffEngine::SpriteFont spriteFont_;

	PhysicsMode physicsMode = PhysicsMode::RIGID;

};


