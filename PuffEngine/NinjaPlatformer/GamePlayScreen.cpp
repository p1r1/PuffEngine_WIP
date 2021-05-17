#include "GamePlayScreen.h"
#include "Light.h"
#include "ScreenIndices.h"

#include <PuffEngine\IMainGame.h>
#include <PuffEngine\ResourceManager.h>

#include <SDL\SDL.h>

#include <iostream>
#include <random>

#define DEBUG_RENDER

GamePlayScreen::GamePlayScreen(PuffEngine::Window* window) : 
	window_(window){
	screenIndex_ = SCREEN_INDEX_GAMEPLAY;
}


GamePlayScreen::~GamePlayScreen() {
	// bos
}

int GamePlayScreen::getNextScreenIndex() const {
	return SCREEN_INDEX_NO_SCREEN;
}

int GamePlayScreen::getPreviousScreenIndex() const {
	return SCREEN_INDEX_MAINMENU;
}

void GamePlayScreen::build() {
	 
}

void GamePlayScreen::destroy() {
	 
}

void GamePlayScreen::onEntry() {
	// Set up gravity ~-9.8 for earth
	b2Vec2 gravity(0.0f, -15.81f);
	world_ = std::make_unique<b2World>(gravity);

	// init debug renderer
	debugRenderer_.init();

	//Make the ground
	// you need to have body and shape all box2d objects
	b2BodyDef groundBodyDef;
	groundBodyDef.position.Set(0.0f, -15.0f);

	b2Body* groundBody = world_->CreateBody(&groundBodyDef);
	
	//fixture , shape
	b2PolygonShape groundBox;
	groundBox.SetAsBox(50.f, 10.0f);

	groundBody->CreateFixture(&groundBox, 0.0f);

	//load the texture
	texture_ = PuffEngine::ResourceManager::getTexture("Assets/bricks_top.png");
	
	//beaweare of the box position may be off the screen
	std::mt19937 randGen;
	std::uniform_real_distribution<float> xPos(-10.0f, 10.0f);
	std::uniform_real_distribution<float> yPos(-5.0f, 15.0f);
	std::uniform_real_distribution<float> size(0.5f, 2.5f);
	std::uniform_int_distribution<int> color(0, 255);
	const int numBoxes = 10;
	for (int i = 0; i < numBoxes; i++) {
		PuffEngine::ColorRGBA8 randColor;
		randColor.r = color(randGen);
		randColor.g = color(randGen);
		randColor.b = color(randGen);
		randColor.a = 255;
	
		Box newBox;
		newBox.init(world_.get(), 
					glm::vec2(xPos(randGen), yPos(randGen)), 
					glm::vec2(size(randGen), size(randGen)), 
					texture_,
					randColor, 
					false);
		boxes_.push_back(newBox);
	}

	//initialize spritebatch
	spriteBatch_.init();

	//shader init
	// Compile our color shader
	textureProgram_.compileShaders("Shaders/textureShading.vert", "Shaders/textureShading.frag");
	textureProgram_.addAttribute("vertexPosition");
	textureProgram_.addAttribute("vertexColor");
	textureProgram_.addAttribute("vertexUV");
	textureProgram_.linkShaders();

	// compile light shader
	lightProgram_.compileShaders("Shaders/lightShading.vert", "Shaders/lightShading.frag");
	lightProgram_.addAttribute("vertexPosition");
	lightProgram_.addAttribute("vertexColor");
	lightProgram_.addAttribute("vertexUV");
	lightProgram_.linkShaders();


	//init camera
	camera_.init(window_->getScreenWidth(), window_->getScreenHeight());
	camera_.setScale(32.0f);//32 pixel for meter

	// Init player
	player_.init(world_.get(), 
				 glm::vec2(0.0f, 15.0f), 
				 glm::vec2(2.0f),
				 glm::vec2(1.0f, 1.8f),
				 PuffEngine::ColorRGBA8(255, 255, 255, 255));

	initUI();
}

void GamePlayScreen::onExit() {
	debugRenderer_.dispose();
}
void GamePlayScreen::update() {
	camera_.update();
	checkInput();
	player_.update(game_->inputManager);

	//update physics simulation
	// TODO: add time step
	world_->Step(1.0f / 60.0f, 6, 2);

}

void GamePlayScreen::draw() {
	// SETUP DRAWING STUFF
	// clear buffers and set background
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	// use shader and fragment programs
	textureProgram_.use();
	
	//upload texture uniform
	GLint textureUniform = textureProgram_.getUniformLocation("mySampler");
	glUniform1i(textureUniform, 0);
	glActiveTexture(GL_TEXTURE0);

	//camera matrix
	glm::mat4 projectionMatrix = camera_.getCameraMatrix();
	GLint pUniform = textureProgram_.getUniformLocation("P");
	glUniformMatrix4fv(pUniform, 1, GL_FALSE, &projectionMatrix[0][0]);

	spriteBatch_.begin();
	
	// DRAWING ALL BOXES 
	for (auto& b : boxes_) {
		b.draw(spriteBatch_);
	}
	
	//draw player
	player_.draw(spriteBatch_);

	spriteBatch_.end();
	spriteBatch_.renderBatch();

	textureProgram_.unUse();

	// DEBUG RENDERING
	if (renderDebug_ == true) {
		glm::vec4 destRect;
		for (auto& b : boxes_) {
			//circle or box pick one

			// draw box for collision
			destRect.x = b.getBody()->GetPosition().x - b.getDimension().x / 2.0f;
			destRect.y = b.getBody()->GetPosition().y - b.getDimension().y / 2.0f;
			destRect.z = b.getDimension().x;
			destRect.w = b.getDimension().y;
			debugRenderer_.drawBox(destRect, PuffEngine::ColorRGBA8(255, 255, 255, 255), b.getBody()->GetAngle());
			
			//circle debug
			/*
			// draw circles for collision
			debugRenderer_.drawCircle(glm::vec2(b.getBody()->GetPosition().x, b.getBody()->GetPosition().y),
									  PuffEngine::ColorRGBA8(255, 255, 255, 255),
									  b.getDimension().x / 2.0f);
									  
			*/
		}
		// debug render player
		player_.drawDebug(debugRenderer_);

		debugRenderer_.end();
		debugRenderer_.render(projectionMatrix, 2.0f);
	}

	// RENDER LIGHT
	// TODO: dont hacode this
	// TODO: make spotlight
	Light playerLight;
	playerLight.color = PuffEngine::ColorRGBA8(255, 255, 255, 128);
	playerLight.position = player_.getPosition();
	playerLight.size = 20.0f;

	Light mouseLight;
	mouseLight.color = PuffEngine::ColorRGBA8(255, 0, 128, 150);
	mouseLight.position = camera_.convertScreenToWorlds(game_->inputManager.getMouseCoords());
	mouseLight.size = 30.0f;

	lightProgram_.use();
	//camera
	pUniform = textureProgram_.getUniformLocation("P");
	glUniformMatrix4fv(pUniform, 1, GL_FALSE, &projectionMatrix[0][0]);

	// Additive blending
	//	       source factor,  
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	
	spriteBatch_.begin();
	//draw
	playerLight.draw(spriteBatch_);
	mouseLight.draw(spriteBatch_);
	spriteBatch_.end();
	spriteBatch_.renderBatch();

	lightProgram_.unUse();

	// Reset to regular alpha blending
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// draw UI
	gui_.draw();

}

void GamePlayScreen::initUI() {
	// Init the UI
	gui_.init("GUI");//static class (not any more)
	gui_.loadScheme("TaharezLook.scheme");
	gui_.setFont("DejaVuSans-10"); // do not include .font
	
	// button setup
	CEGUI::PushButton* testButton = static_cast<CEGUI::PushButton*>(gui_.createWidget("TaharezLook/Button", glm::vec4(0.88f, 0.0f, 0.1f, 0.05f), glm::vec4(0.0f), "TestButton"));
	testButton->setText("ExitGame");
	// set the event to be called when clicked
	testButton->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&GamePlayScreen::onExitClicked, this));

	// TODO:: Fix this (pause and capture current game state)
	// Main Menu button setup
	CEGUI::PushButton* mainMenuButton = static_cast<CEGUI::PushButton*>(gui_.createWidget("TaharezLook/Button", glm::vec4(0.0f, 0.0f, 0.1f, 0.05f), glm::vec4(0.0f), "MainMenuButton"));
	mainMenuButton->setText("Main Menu_Broken");
	// set the event to be called when clicked
	mainMenuButton->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&GamePlayScreen::onMainMenuClicked, this));

	// textbox setup
	CEGUI::Combobox* textCombobox = static_cast<CEGUI::Combobox*>(gui_.createWidget("TaharezLook/Combobox", glm::vec4(0.2f, 0.2f, 0.1f, 0.05f), glm::vec4(0.0f), "TestCombobox"));

	//mouse cursor setup
	gui_.setMouseCursor("TaharezLook/MouseArrow");
	gui_.showMouseCursor();
	SDL_ShowCursor(0);
}

void GamePlayScreen::checkInput() {
	SDL_Event evnt;
	while (SDL_PollEvent(&evnt)) {
		game_->onSDLEvent(evnt);
		gui_.onSDLEvent(evnt);
	}

	switch (evnt.type) {
	case SDL_QUIT:
		onExitClicked(CEGUI::EventArgs());
	default:
		break;
	}
}

bool GamePlayScreen::onMainMenuClicked(const CEGUI::EventArgs & e) {
	currentState_ = PuffEngine::ScreenState::CHANGE_PREVIOUS;
	return false;
}

bool GamePlayScreen::onExitClicked(const CEGUI::EventArgs& e) {
	currentState_ = PuffEngine::ScreenState::EXIT_APPLICATION;
	return true;
}
