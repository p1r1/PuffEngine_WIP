#pragma once

#include "Capsule.h"

#include <PuffEngine\InputManager.h>
#include <PuffEngine\SpriteBatch.h>
#include <PuffEngine\TileSheet.h>

enum class PlayerMoveState {
	STANDING,
	RUNNING,
	PUNCHING,
	IN_AIR
};

class PlayerNinja {
public:

	void init(b2World* world, 
			  const glm::vec2& position, 
			  const glm::vec2 drawDims,
			  const glm::vec2& collisionDims, 
			  PuffEngine::ColorRGBA8 color);

	void draw(PuffEngine::SpriteBatch& spriteBatch);
	void drawDebug(PuffEngine::DebugRenderer& debugRenderer);
	void update(PuffEngine::InputManager& inputManager);

	//getters
	const Capsule& getCapsule() const { return capsule_; }

	glm::vec2 getPosition() const {
		glm::vec2 rv;
		rv.x = capsule_.getBody()->GetPosition().x;
		rv.y = capsule_.getBody()->GetPosition().y;
		return rv;
	}
private:
	glm::vec2 drawDims_;
	PuffEngine::ColorRGBA8 color_;
	PuffEngine::TileSheet texture_;
	Capsule capsule_;

	PlayerMoveState moveSatate_ = PlayerMoveState::STANDING;
	float animeTime_ = 0.0f;
	int direction_ = 1; // 1 or -1
	bool onGround_ = false;
	bool isPunching_ = false;
};