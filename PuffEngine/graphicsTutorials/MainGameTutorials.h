#pragma once // prevent circlar includes
#include <SDL/SDL.h>
#include <GL/glew.h>
#include <vector>

#include <PuffEngine/Sprite.h>
#include <PuffEngine/GLSLProgram.h>
#include <PuffEngine/GLTexture.h>
#include <PuffEngine/Window.h>
#include <PuffEngine/Camera2D.h>
#include <PuffEngine/SpriteBatch.h>
#include <PuffEngine/InputManager.h>
#include <PuffEngine/Timing.h>

#include "Bullet.h"

enum class GameState { PLAY, EXIT };

class MainGame {
public:
	MainGame();
	~MainGame();

	void run();

private:
	void initSystem();
	void initShaders();
	void gameLoop();
	void processInput();
	void drawGame();

	PuffEngine::Window window_;
	int screenWidth_;
	int screenHeight_;
	//play, exit
	GameState gameState_;

	PuffEngine::Camera2D camera_;
	PuffEngine::SpriteBatch spriteBatch_;
	PuffEngine::InputManager inputManager_;
	PuffEngine::FpsLimiter fpsLimiter_;
	PuffEngine::GLSLProgram colorProgram_;
	
	float time_;
	float fps_;
	float maxFPS_;

	std::vector<Bullet> bullets_;

};
