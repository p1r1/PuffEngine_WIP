#include "MainZombieGame.h"

//for glm rotate
#define GLM_ENABLE_EXPERIMENTAL

#include <SDL/SDL.h>
#include <iostream>
#include <ctime>
#include <random>
#include <algorithm>
#include <glm\gtx\rotate_vector.hpp>

#include <PuffEngine\PuffEngine.h>
#include <PuffEngine\Timing.h>
#include <PuffEngine\PuffErrors.h>
#include <PuffEngine\ResourceManager.h>

#include "Zombie.h"
#include "Gun.h"

const float HUMAN_SPEED = 1.0f;
const float ZOMBIE_SPEED = 1.3f;
const float PLAYER_SPEED = 10.0f;
const float BULLET_SPEED = 20.0f;
const float CAMERA_SCALE = 1.0f / 3.0f;

MainZombieGame::MainZombieGame() :
	screenWidth_(1024),
	screeHeight_(600),
	fps_(0.0f),
	gameState_(GameState::PLAY),
	player_(nullptr),
	numHumansKilled_(0),
	numZombiesKilled_(0){
    // Empty
}

MainZombieGame::~MainZombieGame() {
	//free the memory
	for (int i = 0; i < levels_.size(); i++) {
		delete levels_[i];
	}

}

void MainZombieGame::run() {
	initSystems();
	initLevel();

	PuffEngine::Music music = audioEngine_.loadMusic("Sound/XYZ.ogg");
	music.play(-1);//loop forever
	
	gameLoop();
}
/* /// use lambda
void updateBloodParticle(PuffEngine::Particle2D& particle, float deltaTime) {
	/// this is a custom function for function pointer
	particle.position += particle.velocity * deltaTime;
	particle.color.a = (GLubyte)(particle.life * 255.0f);//convert glubyte

}
*/
void MainZombieGame::initSystems() {
	//init game system
	PuffEngine::init();
	
	// init sound must happen after the engine init
	audioEngine_.init();

	//crate game rendering window
	window_.create("ZombiGame", screenWidth_, screeHeight_, 0);
	
	// grey background
	glClearColor(0.7f, 0.7f, 0.7f, 1.0f);
	
	// init shaders
	initShaders();
	
	// init spritebatches
	agentSpriteBatch_.init();
	hudSpriteBathch_.init();

	// init sprite font (after the sdl-init and opengl-init)
	spriFont_ = new PuffEngine::SpriteFont("Fonts/chintzy.ttf", 64);

	// set up cameras
	camera_.init(screenWidth_, screeHeight_);
	hudCamera_.init(screenWidth_, screeHeight_);
	hudCamera_.setPosition(glm::vec2(screenWidth_ / 2, screeHeight_ / 2));

	// Initialize particles
	bloodParticleBatch = new PuffEngine::ParticleBatch2D;
	/*/// func pointer call
	bloodParticleBatch->init(1000, 
							 0.05f, 
							 PuffEngine::ResourceManager::getTexture("Textures/particle.png"),
							 updateBloodParticle);///< func pointer call
	*/
	bloodParticleBatch->init(1000,
							 0.05f,
							 PuffEngine::ResourceManager::getTexture("Textures/particle.png"),
							 [](PuffEngine::Particle2D& particle, float deltaTime) {
		/// this is a lambda function for function pointer
		particle.position += particle.velocity * deltaTime;
		particle.color.a = (GLubyte)(particle.life * 255.0f);//convert glubyte
	});///< func pointer call with lambda

	particleEngine_.addParticleBatch(bloodParticleBatch);
}

void MainZombieGame::initLevel() {
	//level1
	levels_.push_back(new Level("Levels/level1.txt"));
	currentLevel_ = 0;

	//add player
	player_ = new Player();
	// dependency injections camera and inputmanager
	player_->init(PLAYER_SPEED, levels_[currentLevel_]->getStartPlayerPos(), &inputManager_, &camera_, &bullets_);
	humans_.push_back(player_);

	//add all random humans
	std::mt19937 randomEngine;
	randomEngine.seed(time(nullptr));
	std::uniform_int_distribution<int> randX(2, levels_[currentLevel_]->getWidth() - 2);
	std::uniform_int_distribution<int> randY(2, levels_[currentLevel_]->getHeight() - 2);

	for (int i = 0; i < levels_[currentLevel_]->getNumHumans(); i++) {
		humans_.push_back(new Human);
		glm::vec2 pos(randX(randomEngine) * TILE_WIDTH, randY(randomEngine) * TILE_WIDTH);
		humans_.back()->init(HUMAN_SPEED, pos);
	}

	//add all zombies
	const std::vector<glm::vec2>& zombiePositions = levels_[currentLevel_]->getZombieStartPositions();
	for (int i = 0; i < zombiePositions.size(); i++) {
		zombies_.push_back(new Zombie);
		zombies_.back()->init(ZOMBIE_SPEED, zombiePositions[i]);
	}

	//set up player guns
	player_->addGun(new Gun("Magnum", 10, 1, 5.0f, 30, BULLET_SPEED, audioEngine_.loadSoundEffect("Sound/shots/pistol.wav")));
	player_->addGun(new Gun("Shotgun", 30, 12, 20.0f, 4, BULLET_SPEED, audioEngine_.loadSoundEffect("Sound/shots/shotgun.wav")));
	player_->addGun(new Gun("MP%", 2, 1, 10.0f, 20, BULLET_SPEED, audioEngine_.loadSoundEffect("Sound/shots/cg1.wav")));

}

void MainZombieGame::initShaders() {
    // Compile our color shader
	textureProgram_.compileShaders("Shaders/textureShading.vert", "Shaders/textureShading.frag");
	textureProgram_.addAttribute("vertexPosition");
	textureProgram_.addAttribute("vertexColor");
	textureProgram_.addAttribute("vertexUV");
	textureProgram_.linkShaders();
}

void MainZombieGame::gameLoop() {
	const float DESIRED_FPS = 60.0f;
	const int MAX_PHYSICS_STEPS = 6; ///< for spiral of death

	//limit the fps
	PuffEngine::FpsLimiter fpsLimiter;
	fpsLimiter.setMaxFPS(DESIRED_FPS);

	//adjst zoom level of the camera
	camera_.setScale(CAMERA_SCALE);

	//timesteps
	const float MS_PER_SECONDS = 1000.0f;
	const float DESIRED_FRAMETIME = MS_PER_SECONDS / DESIRED_FPS;
	const float MAX_DELTA_TIME = 1.0f; //< 60/60(fps wise)
	float previousTicks = SDL_GetTicks();

	//ana loop
	while (gameState_ == GameState::PLAY) {
		fpsLimiter.begin();
		
		float newTicks = SDL_GetTicks();
		float frameTime = newTicks - previousTicks;
		previousTicks = newTicks;///< loopy
		float totalDeltaTime = frameTime / DESIRED_FRAMETIME; ///< frametime = 1/fps

		checkVictory();
		inputManager_.update();
		processInput();

		//time steps and spiral of death
		int i = 0;
		while (totalDeltaTime > 0.0f && i < MAX_PHYSICS_STEPS) { ///< semifixed
			float deltaTime = std::min(totalDeltaTime, MAX_DELTA_TIME);
			updateAgents(deltaTime);
			updateBullets(deltaTime);

			//update particle engine
			particleEngine_.update(deltaTime);

			totalDeltaTime -= deltaTime;
			i++;///< for spiral of death
		}
		
		//Cameras setup and update 
		camera_.setPosition(player_->getPosition());
		camera_.update();
		hudCamera_.update();
		
		drawGame();

		fps_ = fpsLimiter.end();
		std::cout << fps_ << std::endl;
	}
}

void MainZombieGame::updateAgents(float deltaTime) {
	//update humans and player
	for (int i = 0; i < humans_.size(); i++) {
		humans_[i]->update(levels_[currentLevel_]->getLevelData(),
						   humans_,
						   zombies_,
						   deltaTime);
	}
	//update zombies
	for (int i = 0; i < zombies_.size(); i++) {
		zombies_[i]->update(levels_[currentLevel_]->getLevelData(),
						   humans_,
						   zombies_,
							deltaTime);
	}

	//Update zombie collision
	for (int i = 0; i < zombies_.size(); i++) {
		//collide with other zzombieler
		for (int j = i + 1; j < zombies_.size(); j++) {
			zombies_[i]->collideWithAgent(zombies_[j]);
		}
		//collide with humans
		for (int j = 1; j < humans_.size(); j++) {
			if (zombies_[i]->collideWithAgent(humans_[j])) {
				//add new zombie
				zombies_.push_back(new Zombie);
				zombies_.back()->init(ZOMBIE_SPEED, humans_[j]->getPosition());
				//delete collided human
				//delete jth member, copy last member to jth location, delete last member
				delete(humans_[j]);
				humans_[j] = humans_.back();
				humans_.pop_back();
			}
		}
		//collide with player (humans[0])
		if (zombies_[i]->collideWithAgent(player_)) {
			PuffEngine::fatalError("You loose!");
		}
	}

	//Update human collision
	for (int i = 0; i < humans_.size(); i++) {
		//collide with other humans
		for (int j = i + 1; j < humans_.size(); j++) {
			humans_[i]->collideWithAgent(humans_[j]);
		}
	}

}

void MainZombieGame::updateBullets(float deltaTime) {
	//coolide with world
	for (int i = 0; i < bullets_.size();) {
		// if true bullet collided with wall or world
		if (bullets_[i].update(levels_[currentLevel_]->getLevelData(), deltaTime)) {
			bullets_[i] = bullets_.back();
			bullets_.pop_back();
		}
		else {
			i++;
		}
	}

	bool wasBuletRemoved;
	//collide with humans and zombies
	for (int i = 0; i < bullets_.size(); i++) {
		wasBuletRemoved = false;
		for (int j = 0; j < zombies_.size();) {
			if (bullets_[i].collideWithAgent(zombies_[j])) {
				//add blood effect
				addBlood(bullets_[i].getPosition(), 5);

				//damage zombie or kill
				if (zombies_[j]->applyDamage(bullets_[i].getDamage())) {
					delete(zombies_[j]);
					zombies_[j] = zombies_.back();
					zombies_.pop_back();
					numZombiesKilled_++;
				}
				else {
					j++;
				}
				//remove bullet
				bullets_[i] = bullets_.back();
				bullets_.pop_back();
				wasBuletRemoved = true;
				i--;///< make sure do not skip a bullet
				//bullet died
				break;
			}
			else {
				j++;
			}
		}

		if (wasBuletRemoved == false) {
			for (int j = 1; j < humans_.size();) {
				if (bullets_[i].collideWithAgent(humans_[j])) {
					//add blood effect
					addBlood(bullets_[i].getPosition(), 5);

					//damage human or kill
					if (humans_[j]->applyDamage(bullets_[i].getDamage())) {
						delete(humans_[j]);
						humans_[j] = humans_.back();
						humans_.pop_back();
						numHumansKilled_++;
					}
					else {
						j++;
					}
					//remove bullet
					bullets_[i] = bullets_.back();
					bullets_.pop_back();
					wasBuletRemoved = true;
					i--;///< make sure do not skip a bullet
						//bullet died
					break;
				}
				else {
					j++;
				}
			}

		}

	}
}

void MainZombieGame::checkVictory() {
	// TODO: support for multipe level
	if (zombies_.empty()) {
		std::printf("***You Win!***\nYoukilled %d humans and %d zombies. There are %d/%d humans remaining.",
					numHumansKilled_, numZombiesKilled_, humans_.size() - 1, levels_[currentLevel_]->getNumHumans());
		PuffEngine::fatalError("");
	}
}

void MainZombieGame::processInput() {
    SDL_Event evnt;

    //Will keep looping until there are no more events to process
    while (SDL_PollEvent(&evnt)) {
        switch (evnt.type) {
            case SDL_QUIT:
                //Exit the game here!
                break;
            case SDL_MOUSEMOTION:
                inputManager_.setMouseCoords(evnt.motion.x, evnt.motion.y);
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
        }
    
	}

}

void MainZombieGame::drawGame() {
    // Set the base depth to 1.0
    glClearDepth(1.0);
    // Clear the color and depth buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	
	//Draw code goes here
	textureProgram_.use();
	//use texture unit 0
	glActiveTexture(GL_TEXTURE0);

	//get uniform texture mySampler
	GLint textureUniform = textureProgram_.getUniformLocation("mySampler");
	glUniform1i(textureUniform, 0);
	
	//get camera matrix P
	glm::mat4 projectionMatrix = camera_.getCameraMatrix();
	GLint pUniform = textureProgram_.getUniformLocation("P");
	glUniformMatrix4fv(pUniform, 1, GL_FALSE, &projectionMatrix[0][0]); ///< beginnig of array 

	//draw level
	levels_[currentLevel_]->draw();

	//begin drawing agents
	agentSpriteBatch_.begin();
	
	//draw all humans  includes player
	const glm::vec2 agentDims(AGENT_RADIUS / 2.0f);
	for (int i = 0; i < humans_.size(); i++) {
		if (camera_.isBoxInView(humans_[i]->getPosition(), agentDims)) {
			humans_[i]->draw(agentSpriteBatch_);
		}
	}

	//draw all zombies
	for (int i = 0; i < zombies_.size(); i++) {
		if (camera_.isBoxInView(zombies_[i]->getPosition(), agentDims)) {
			zombies_[i]->draw(agentSpriteBatch_);
		}
	}
	
	//draw bullets
	for (int i = 0; i < bullets_.size(); i++) {
		bullets_[i].draw(agentSpriteBatch_);
	}

	agentSpriteBatch_.end();
	agentSpriteBatch_.renderBatch();
	
	//draw render particles
	particleEngine_.draw(&agentSpriteBatch_);

	//render the fonts and hud
	drawHud();

	//end agents drawing
	//release the texture prrogram
	textureProgram_.unUse();

	//draw code ends
	//Swap our buffer and draw everything to the screen!
	window_.swapBuffer();
}

void MainZombieGame::drawHud() {
	char buffer[256];

	//get hud camera matrix P
	glm::mat4 projectionMatrix = hudCamera_.getCameraMatrix();
	GLint pUniform = textureProgram_.getUniformLocation("P");
	glUniformMatrix4fv(pUniform, 1, GL_FALSE, &projectionMatrix[0][0]); ///< beginnig of array 


	hudSpriteBathch_.begin();

	sprintf_s(buffer, "Num Humans %d", humans_.size());

	//for std::string you need to convert to csrtring
	spriFont_->draw(hudSpriteBathch_, buffer, glm::vec2(0, 0), glm::vec2(0.5), 0.0f,
					PuffEngine::ColorRGBA8(255, 255, 255, 255));
	//for zombies
	sprintf_s(buffer, "Num Zombies %d", zombies_.size());
	spriFont_->draw(hudSpriteBathch_, buffer, glm::vec2(0, 32), glm::vec2(0.5), 0.0f,
					PuffEngine::ColorRGBA8(128, 255, 128, 255));

	hudSpriteBathch_.end();
	hudSpriteBathch_.renderBatch();

}

void MainZombieGame::addBlood(const glm::vec2& position, int numParticles) {
	static std::mt19937 randomEngine(time(nullptr));
	static std::uniform_real_distribution<float> randAngle(0.0f, 360.0f);
	glm::vec2 vel(2.0f, 0.0f);
	PuffEngine::ColorRGBA8 bloodColor(255, 0, 70, 255);

	for (int i = 0; i < numParticles; i++) {
		bloodParticleBatch->addParticle(position,
										glm::rotate(vel, randAngle(randomEngine)),
										bloodColor,
										30.0f);
	}
}
