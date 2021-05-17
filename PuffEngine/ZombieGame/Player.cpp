#include "Player.h"

#include "Gun.h" ///< need for declaration on header file because is just pointers 

#include <SDL\SDL.h>

#include <PuffEngine\ResourceManager.h>

Player::Player(): 
	currentGunIndex(-1) {
	//Empty
}


Player::~Player() {
	//Empty
}

void Player::init(float speed, 
				  glm::vec2 pos, 
				  PuffEngine::InputManager* inputManager, 
				  PuffEngine::Camera2D* camera,
				  std::vector<ZombieBullet>* bullets) {
	health_ = 120;
	speed_ = speed;
	position_ = pos;
	color_.r = 255;
	color_.g = 255;
	color_.b = 185;
	color_.a = 255;

	inputManager_ = inputManager;
	camera_ = camera;
	bullets_ = bullets;
	textureID_ = PuffEngine::ResourceManager::getTexture("Textures/player.png").id;

}

void Player::addGun(Gun * gun) {
	guns_.push_back(gun);
	//if no gun equipt equipt that gun
	if (currentGunIndex = -1) {
		currentGunIndex = 0;
	}
}

void Player::update(const std::vector<std::string>& levelData,
					std::vector<Human*>& humans,
					std::vector<Zombie*>& zombies,
					float deltaTime){
	if (inputManager_->isKeyDown(SDLK_w)) {
		position_.y += speed_ * deltaTime;
	}
	if (inputManager_->isKeyDown(SDLK_s)) {
		position_.y -= speed_ * deltaTime;
	}

	if (inputManager_->isKeyDown(SDLK_a)) {
		position_.x -= speed_ * deltaTime;
	}
	if(inputManager_->isKeyDown(SDLK_d)) {
		position_.x += speed_ * deltaTime;
	}

	//GUNS
	if (inputManager_->isKeyDown(SDLK_1) && guns_.size() >= 0) {
		currentGunIndex = 0;
	}
	else if (inputManager_->isKeyDown(SDLK_2) && guns_.size() >= 1) {
		currentGunIndex = 1;
	}
	else if (inputManager_->isKeyDown(SDLK_3) && guns_.size() >= 2) {
		currentGunIndex = 2;
	}

	// get mouse coords
	glm::vec2 mouseCoords = inputManager_->getMouseCoords();
	mouseCoords = camera_->convertScreenToWorlds(mouseCoords);
	
	//set center pos and direction
	glm::vec2 centerPosition = position_ + glm::vec2(AGENT_RADIUS);
	direction_ = glm::normalize(mouseCoords - centerPosition);

	if (currentGunIndex != -1) {
		guns_[currentGunIndex]->update(inputManager_->isKeyDown(SDL_BUTTON_LEFT), 
									   centerPosition,
									   direction_,
									   *bullets_,
									   deltaTime);//bullet is refrence again
	}
	collideWithLevel(levelData);
}
