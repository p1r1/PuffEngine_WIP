#pragma once

#include <glm/glm.hpp>
#include <PuffEngine/SpriteBatch.h>
class Bullet {
public:
	Bullet();
	Bullet(glm::vec2 pos, glm::vec2 dir, float speed, int lifeTime);
	~Bullet();

	void draw(PuffEngine::SpriteBatch& spriteBatch);
	//returns true when bullets lifetime is 0
	bool update();
private:
	int lifeTime_;
	float speed_;
	glm::vec2 direction_;
	glm::vec2 position_;

};

