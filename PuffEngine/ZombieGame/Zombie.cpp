#include "Zombie.h"
#include "Human.h"

#include <PuffEngine\ResourceManager.h>

Zombie::Zombie() {
}


Zombie::~Zombie() {
}

void Zombie::init(float speed, glm::vec2 pos) {
	health_ = 150;
	speed_ = speed;
	position_ = pos;
	color_ = PuffEngine::ColorRGBA8(255, 255, 255, 255);
	textureID_ = PuffEngine::ResourceManager::getTexture("Textures/zombie.png").id;

}

void Zombie::update(const std::vector<std::string>& levelData,
					std::vector<Human*>& humans,
					std::vector<Zombie*>& zombies,
					float deltaTime) {

	collideWithLevel(levelData);
	Human* closestHuman = getNearestHuman(humans);

	if (closestHuman != nullptr) {
		direction_ = glm::normalize(closestHuman->getPosition() - position_);
		position_ += direction_ * speed_ * deltaTime;
	}

}

Human * Zombie::getNearestHuman(std::vector<Human*>& humans) {
	Human* closestHuman = nullptr;
	float smallestDistance = 99999.0f;

	for (int i = 0; i < humans.size(); i++) {
		glm::vec2 distVec = humans[i]->getPosition() - position_;
		float distance = glm::length(distVec);

		if (distance < smallestDistance) {
			smallestDistance = distance;
			closestHuman = humans[i];
		}
	}
	
	return closestHuman;
}
