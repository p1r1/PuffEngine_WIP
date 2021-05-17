#pragma once

#include <glm\glm.hpp>

#include <PuffEngine\SpriteBatch.h>

#include <string>
//for declaration because we dont need to include header files
//prevent circluar includes
class Zombie;
class Human;


const float AGENT_WIDTH = 60.0f;
const float AGENT_RADIUS = AGENT_WIDTH / 2.0f;

class Agent {
public:
	Agent();
	virtual ~Agent(); ///< it will call human, player and zombies destructor
	
	//any class drives from aget has to update this function
	//pure virtual function "=0" vvv
	virtual void update(const std::vector<std::string>& levelData,
						std::vector<Human*>& humans,
						std::vector<Zombie*>& zombies,
						float deltaTime) = 0;
	//collision functions
	bool collideWithLevel(const std::vector<std::string>& levelData);
	bool collideWithAgent(Agent* agent);

	//draw all agents with dependency injection vvv
	void draw(PuffEngine::SpriteBatch& spriteBatch_);
	bool applyDamage(float damage);
	
	//getters
	glm::vec2 getPosition() const { return position_; }

protected:///< all agents have these
	void checkTilePosition(const std::vector<std::string>& levelData,
						   std::vector<glm::vec2>& collideTilePositions,
						   float x, 
						   float y);

	void collideWithTile(glm::vec2 tilePos);

	glm::vec2 position_;
	glm::vec2 direction_ = glm::vec2(1.0f, 0.0f);//point to the right x ekseni
	float speed_;
	float health_;

	PuffEngine::ColorRGBA8 color_;
	GLuint textureID_;
};

