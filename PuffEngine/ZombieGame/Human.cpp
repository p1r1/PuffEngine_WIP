#include "Human.h"

#define GLM_ENABLE_EXPERIMENTAL

#include <random>
#include <ctime>
#include <glm\gtx\rotate_vector.hpp>

#include <PuffEngine\ResourceManager.h>
Human::Human() : 
	frames_(0)
{
}


Human::~Human() {
}

void Human::init(float speed, glm::vec2 pos) {
	health_ = 20;
	speed_ = speed;
	position_ = pos;
	color_.r = 255;
	color_.g = 255;
	color_.b = 255;
	color_.a = 255;


	static std::mt19937 randomEngine(time(nullptr));
	static std::uniform_real_distribution<float> randDir(-1.0f, 1.0f);
	
	direction_ = glm::vec2(randDir(randomEngine), randDir(randomEngine));
	if (direction_.length() == 0)
		direction_ = glm::vec2(1.0f, 0.0f);

	textureID_ = PuffEngine::ResourceManager::getTexture("Textures/human.png").id;
}

void Human::update(const std::vector<std::string>& levelData,
				   std::vector<Human*>& humans,
				   std::vector<Zombie*>& zombies,
				   float deltaTime) {
	//walking
	position_ += direction_ * speed_ * deltaTime;
	
	//random rotate every 20 frames
	static std::mt19937 randomEngine(time(nullptr));
	static std::uniform_real_distribution<float> randRotate(-10.1f, 10.1f);
	if (frames_ == 150) {
		direction_ = glm::rotate(direction_, randRotate(randomEngine));
		frames_ = 0;
	}
	else {
		frames_++;
	}

	//if collide with level true then rotate
	if (collideWithLevel(levelData)) {
		direction_ = glm::rotate(direction_, randRotate(randomEngine));
	}

	
}
