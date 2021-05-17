#pragma once

#include <PuffEngine\DebugRenderer.h>

#include <Box2D\Box2D.h>
#include <glm\glm.hpp>

class Capsule {
public:
	void init(b2World* world, 
			  const glm::vec2& position,
			  const glm::vec2& dimension,
			  float density,
			  float friction,
			  bool fixedRotation);
	void drawDebug(PuffEngine::DebugRenderer& debugRenderer);

	//getters
	b2Body* getBody() const { return body_; }
	b2Fixture* getFixture(int index) { return fixtures_[index]; }
	const glm::vec2& getDimension() const { return dimensions_; }

private:
	b2Body* body_ = nullptr;
	b2Fixture* fixtures_[3];
	glm::vec2 dimensions_;
};

