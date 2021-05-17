#include "MainGameTutorials.h"

#include <PuffEngine/PuffEngine.h>
#include <PuffEngine/PuffErrors.h>
#include <PuffEngine/ResourceManager.h>
#include <iostream>
#include <string>

MainGame::MainGame() :
	screenWidth_(640),
	screenHeight_(480),
	gameState_(GameState::PLAY),
	time_(0.0f),
	maxFPS_(60.0f) 
{
	camera_.init(screenWidth_, screenHeight_);
}


MainGame::~MainGame() {
}

void MainGame::run() {
	initSystem();
	gameLoop();
}

void MainGame::initSystem() {
	PuffEngine::init();
	window_.create("Game Engine", screenWidth_, screenHeight_, 0);
	initShaders();
	spriteBatch_.init();
	fpsLimiter_.init(maxFPS_);
}

void MainGame::initShaders() {
	colorProgram_.compileShaders("Shaders/colorShading.vert", "Shaders/colorShading.frag");
	//name of the variable in the GLSL vertex progam
	colorProgram_.addAttribute("vertexPosition");
	colorProgram_.addAttribute("vertexColor");
	colorProgram_.addAttribute("vertexUV");
	colorProgram_.linkShaders();
}

void MainGame::gameLoop() {

	while (gameState_ != GameState::EXIT) {
		fpsLimiter_.begin();

		processInput();
		time_ += 0.01;

		camera_.update();
		//if we say i++ in for dying bullets not update
		for (int i = 0; i < bullets_.size();) {
			if (bullets_[i].update() == true) {
				bullets_[i] = bullets_.back();
				bullets_.pop_back();
			}
			else {
				i++;
			}
		}

		drawGame();

		fps_ = fpsLimiter_.end();
		//print every 10 frame
		static 	int frameCounter = 0;
		frameCounter++;
		if (frameCounter == 10000) {
			std::cout << fps_ << std::endl;
			frameCounter = 0;
		}

	}
}

void MainGame::processInput() {
	SDL_Event evnt;
	const float CAMERASPEED = 2.0f;
	const float	SCALESPEED = 0.05f;

	while (SDL_PollEvent(&evnt)) {//addres of evnt = pointer
		switch (evnt.type) {
		case SDL_QUIT:
			gameState_ = GameState::EXIT;
			break;
		case SDL_KEYDOWN:
			inputManager_.pressKey(evnt.key.keysym.sym);
			break;
		case SDL_KEYUP:
			inputManager_.releaseKey(evnt.key.keysym.sym);
			break;
		case SDL_MOUSEBUTTONDOWN:
			inputManager_.pressKey(evnt.button.button);
			break;
		case SDL_MOUSEBUTTONUP:
			inputManager_.releaseKey(evnt.button.button);
			break;
		case SDL_MOUSEMOTION:
			//std::cout << evnt.motion.x << "," << evnt.motion.y << std::endl;
			inputManager_.setMouseCoords(evnt.motion.x, evnt.motion.y);
			break;
		default:
			break;
		}
	}

	if (inputManager_.isKeyDown(SDLK_w)) {
		camera_.setPosition(camera_.getPosition() + glm::vec2(0.0f, CAMERASPEED));
	}
	if (inputManager_.isKeyDown(SDLK_s)) {
		camera_.setPosition(camera_.getPosition() + glm::vec2(0.0f, -CAMERASPEED));
	}
	if (inputManager_.isKeyDown(SDLK_a)) {
		camera_.setPosition(camera_.getPosition() + glm::vec2(-CAMERASPEED, 0.0f));
	}
	if (inputManager_.isKeyDown(SDLK_d)) {
		camera_.setPosition(camera_.getPosition() + glm::vec2(CAMERASPEED, 0.0f));
	}
	if (inputManager_.isKeyDown(SDLK_q)) {
		camera_.setScale(camera_.getScale() + SCALESPEED);
	}
	if (inputManager_.isKeyDown(SDLK_e)) {
		camera_.setScale(camera_.getScale() - SCALESPEED);
	}

	///mouse coords
	if (inputManager_.isKeyDown(SDL_BUTTON_LEFT)) {
		glm::vec2 mouseCoords = inputManager_.getMouseCoords();
		mouseCoords = camera_.convertScreenToWorlds(mouseCoords);
		
		glm::vec2 playerPosition(0.0f);
		glm::vec2 direction = mouseCoords - playerPosition;
		//make division calculate hypotenus and unit lengthand all that jazz
		direction = glm::normalize(direction);

		bullets_.emplace_back(playerPosition, direction, 5.01f, 1000);

	}
}


void MainGame::drawGame() {
	glClearDepth(1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // bitwise
	/*
	//immidiate mode very bad! do not use it!
	glEnableClientState(GL_COLOR_ARRAY);
	glBegin(GL_TRIANGLES);
	glColor3f(1.0f, 0.0f, 0.0f);
	glVertex2f(0.0f, 0.0f);
	glVertex2f(0.0f, 200.0f);
	glVertex2f(200.0f, 1000.0f);
	glEnd();
	//immidiate mode ends
	*/
	//set the gpu program --> glsl program
	colorProgram_.use();

	//TEXTURES
	//use first texture there will be multitexturing
	glActiveTexture(GL_TEXTURE0);

	//UNIFORMS
	//texture
	GLint textureLocation = colorProgram_.getUniformLocation("mySampler");
	glUniform1i(textureLocation, 0);// 0 -> GL_TEXTURE0
	//time
	//GLint timeLocation = colorProgram_.getUniformLocation("time");
	//glUniform1f(timeLocation, time_);
	//camera matrix
	GLint pLocation = colorProgram_.getUniformLocation("P");
	glm::mat4 cameraMatrix = camera_.getCameraMatrix();
	glUniformMatrix4fv(pLocation, 1, GL_FALSE, &(cameraMatrix[0][0]));
	
	spriteBatch_.begin();
	
	glm::vec4 pos(0.0f, 0.0f, 50.0f, 50.0f);
	glm::vec4 uv(0.0f, 0.0f, 1.0f, 1.0f);
	//bad practice to have static texture
	static PuffEngine::GLTexture texture = PuffEngine::ResourceManager::getTexture("Textures/jimmyJump_pack/\PNG/CharacterRight_Standing.png");
	PuffEngine::ColorRGBA8 color;
	color.r = 255;
	color.g = 255;
	color.b = 255;
	color.a = 255;

	spriteBatch_.draw(pos, uv, texture.id, 0.0f, color);
	
	for (int i = 0; i < bullets_.size(); i++) {
		bullets_[i].draw(spriteBatch_);
	}


	spriteBatch_.end();
	spriteBatch_.renderBatch();

	//unbind texture
	glBindTexture(GL_TEXTURE_2D, 0);
	//unuse the program and free the memmory
	colorProgram_.unUse();
	//Swap window (doublebuffer) for smoothness
	window_.swapBuffer();
}

