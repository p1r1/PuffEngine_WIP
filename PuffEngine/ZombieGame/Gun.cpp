#include "Gun.h"

#include <random>
#include <ctime>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm\gtx\rotate_vector.hpp>


Gun::Gun(std::string name, int fireRate, int bulletPerShot, float spread, 
		 float bulletDamage, float bulletSpeed, PuffEngine::SoundEffect fireEffect) :
	name_(name),
	fireRate_(fireRate),
	bulletPerShot_(bulletPerShot),
	spread_(spread),
	bulletDamage_(bulletDamage),
	bulletSpeed_(bulletSpeed),
	frameCounter(0),
	fireEffect_(fireEffect){
		/// use inititalization list man for a little bit of speed
}


Gun::~Gun() {
}

void Gun::update(bool isMouseDown, const glm::vec2& position, const glm::vec2& direction, std::vector<ZombieBullet>& bullets, float deltaTime) {
	frameCounter += 1.0f * deltaTime;
	if (frameCounter >= fireRate_ && isMouseDown) {
		fire(direction, position, bullets);
		frameCounter = 0;
	}
}

void Gun::fire(const glm::vec2& direction, const glm::vec2& position, std::vector<ZombieBullet>& bullets) {
	static std::mt19937 randomEngine(time(nullptr));
	std::uniform_real_distribution<float> randRotate(-spread_, spread_);
	
	//play guns sound effect
	fireEffect_.play();

	for (int i = 0; i < bulletPerShot_; i++) {
		bullets.emplace_back(position, 
							 glm::rotate(direction, randRotate(randomEngine) * (3.141592f / 180.0f)),
							 bulletDamage_, 
							 bulletSpeed_);
	}

}