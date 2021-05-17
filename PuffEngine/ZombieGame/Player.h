#pragma once
#include "Human.h"
#include <PuffEngine\InputManager.h>
#include <PuffEngine\Camera2D.h>

#include "ZombieBullet.h" ///< not need for declaration because we use actual bullets

class Gun;///< need for declaration on header file because is just pointers 


class Player : public Human{
public:
	Player();
	~Player();

	void init(float speed, 
			  glm::vec2 pos, 
			  PuffEngine::InputManager* inputManager, 
			  PuffEngine::Camera2D* camera,
			  std::vector<ZombieBullet>* bullets);
	void addGun(Gun* gun);

	void update(const std::vector<std::string>& levelData,
				std::vector<Human*>& humans,
				std::vector<Zombie*>& zombies,
				float deltaTime) override;

private:
	//dependency injection sort a like
	PuffEngine::InputManager* inputManager_;
	PuffEngine::Camera2D* camera_;
	std::vector<ZombieBullet>* bullets_;

	std::vector<Gun*> guns_;
	int currentGunIndex;
};

