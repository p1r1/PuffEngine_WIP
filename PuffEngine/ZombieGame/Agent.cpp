#include "Agent.h"

#include <PuffEngine\ResourceManager.h>
#include "Level.h"

#include <algorithm>

Agent::Agent() {
}


Agent::~Agent() {
}

bool Agent::collideWithLevel(const std::vector<std::string>& levelData) {
	std::vector<glm::vec2> collideTilePositions;

	// check four corners
	//first corner is just position
	checkTilePosition(levelData,
					  collideTilePositions,
					  position_.x,
					  position_.y);
	//second corner 
	checkTilePosition(levelData,
					  collideTilePositions,
					  position_.x + AGENT_WIDTH,
					  position_.y);
	//third corner 
	checkTilePosition(levelData,
					  collideTilePositions,
					  position_.x,
					  position_.y + AGENT_WIDTH);
	//fourth corner 
	checkTilePosition(levelData,
					  collideTilePositions,
					  position_.x + AGENT_WIDTH,
					  position_.y + AGENT_WIDTH);

	// Check if there was no collision
	if (collideTilePositions.size() == 0) {
		return false;
	}

	//do the collision
	for (int i = 0; i < collideTilePositions.size(); i++) {
		collideWithTile(collideTilePositions[i]);
	}
	return true;
}

//circular collision (3d spherical collision)
bool Agent::collideWithAgent(Agent* agent) {
	const float MIN_DISTANCE = AGENT_RADIUS * 2.0f;//agent radius

	glm::vec2 centerPosA = position_ + glm::vec2(AGENT_RADIUS);
	glm::vec2 centerPosB = agent->getPosition() + glm::vec2(AGENT_RADIUS);
	glm::vec2 distVec = centerPosA - centerPosB;

	float distance = glm::length(distVec);
	float collisionDepth = MIN_DISTANCE - distance;
	
	if (collisionDepth > 0) {
		glm::vec2 collisionDepthVec = glm::normalize(distVec) * collisionDepth;
		position_ += collisionDepthVec / 2.0f;
		agent->position_ -= collisionDepthVec / 2.0f;
		return true;
	}


	return false;
}

void Agent::draw(PuffEngine::SpriteBatch & spriteBatch_) {
	const glm::vec4 uvRect(0.0f, 0.0f, 1.0f, 1.0f);
	glm::vec4 destRect(position_.x, position_.y, AGENT_WIDTH, AGENT_WIDTH);

	spriteBatch_.draw(destRect, uvRect, textureID_, 0.0f, color_, direction_);

}

bool Agent::applyDamage(float damage) {
	health_ -= damage;
	if (health_ <= 0) {
		return true;
	}
	return false;
}

void Agent::checkTilePosition(const std::vector<std::string>& levelData, 
							  std::vector<glm::vec2>& collideTilePositions, 
							  float x, 
							  float y) {

	glm::vec2 cornerPos = glm::vec2(floor(x / (float)TILE_WIDTH),
									floor(y / (float)TILE_WIDTH));
	
	//if we are outside the world, just return
	if (cornerPos.x <0 || cornerPos.x >= levelData[0].size() ||
		cornerPos.y <0 || cornerPos.y >= levelData.size()) {
		return;
	}

	//if colliding tile is not equal to . then collide 
	if (levelData[cornerPos.y][cornerPos.x] != '.') {
		collideTilePositions.push_back(cornerPos * (float)TILE_WIDTH + glm::vec2((float)TILE_WIDTH / 2.0f));
	}
}

//AABB (axis aligned bounding boxes) AABB collision
//push the agent with collision
void Agent::collideWithTile(glm::vec2 tilePos) {
	const float AGENT_RADIUS = (float)AGENT_WIDTH / 2.0f;
	const float TILE_RADIUS = (float)TILE_WIDTH / 2.0f;
	const float MIN_DISTANCE = AGENT_RADIUS + TILE_RADIUS;

	glm::vec2 centerPlayerPos = position_ + glm::vec2(AGENT_RADIUS);
	glm::vec2 distVec = centerPlayerPos - tilePos;

	float xDepth = MIN_DISTANCE - abs(distVec.x);
	float yDepth = MIN_DISTANCE - abs(distVec.y);

	//if this is true, we are colliding
	if (xDepth > 0 && yDepth > 0) {
		if (std::max(xDepth, 0.0f) < std::max(yDepth, 0.0f)) {
			if (distVec.x < 0) 
				position_.x -= xDepth;
			else
				position_.x += xDepth;
		}
		else {
			if (distVec.y < 0)
				position_.y -= yDepth;
			else
				position_.y += yDepth;
		}
	}
}
