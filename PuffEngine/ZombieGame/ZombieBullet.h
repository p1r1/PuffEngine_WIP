#pragma once
#include <glm\glm.hpp>
#include <vector>
#include <string>

#include <PuffEngine\SpriteBatch.h>

class Human;
class Zombie;
class Agent;

const int BULLET_RADIUS = 5;

class ZombieBullet {
public:
	//ZombieBullet();
	ZombieBullet(glm::vec2 position, glm::vec2 direction, float damage, float speed);
	~ZombieBullet();

	bool  update(const std::vector< std::string >& levelData, float deltaTime);

	void draw(PuffEngine::SpriteBatch& spriteBatch);
	bool collideWithAgent(Agent* agent);
	//getters
	float getDamage() const { return m_damage; }
	glm::vec2 getPosition() const{ return m_position; }
private:
	bool collideWithWorld(const std::vector< std::string >& levelData);

	float m_damage;
	float m_speed;
	glm::vec2 m_position;
	glm::vec2 m_direction;
};

