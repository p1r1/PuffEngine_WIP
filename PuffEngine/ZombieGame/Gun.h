#pragma once
#include <string>
#include <vector>
#include <glm\glm.hpp>

#include <PuffEngine\AudioEngine.h>

#include "ZombieBullet.h"


class Gun {
public:
	Gun(std::string name, int fireRate, int bulletPerShot, float spread, 
		float bulletDamage, float bulletSpeed, PuffEngine::SoundEffect fireEffect_);
	~Gun();
	
	void update(bool isMouseDown, const glm::vec2& position, const glm::vec2& direction, std::vector<ZombieBullet>& bullets, float deltaTime);

private:
	PuffEngine::SoundEffect fireEffect_;

	std::string name_;
	int fireRate_; ///< Fire rate in terms of frames
	int bulletPerShot_; ///< like shotgun
	float frameCounter;
	float bulletDamage_;
	float bulletSpeed_;
	float spread_; ///< accuracy
	

	void fire(const glm::vec2& direction, const glm::vec2& position, std::vector<ZombieBullet>& bullets);

};

