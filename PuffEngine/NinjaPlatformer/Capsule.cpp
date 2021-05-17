#include "Capsule.h"

void Capsule::init(b2World* world, 
				   const glm::vec2& position, 
				   const glm::vec2& dimension, 
				   float density,
				   float friction,
				   bool fixedRotation) {
	dimensions_ = dimension;

	//make the body
	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.position.Set(position.x, position.y);
	bodyDef.fixedRotation = fixedRotation;

	body_ = world->CreateBody(&bodyDef);

	// create box fixture and shape
	b2PolygonShape boxShape;
	//shrink box
	boxShape.SetAsBox(dimension.x / 2.0f, (dimension.y - dimension.x) / 2.0f);

	b2FixtureDef fixtureDef;
	fixtureDef.shape = &boxShape;
	fixtureDef.density = density;
	fixtureDef.friction = friction;
	fixtures_[0] = body_->CreateFixture(&fixtureDef);

	//create the circles
	b2CircleShape circleShape;
	circleShape.m_radius = dimension.x / 2.0f;

	b2FixtureDef circleDef;
	circleDef.shape = &circleShape;
	circleDef.density = 1.0f;
	circleDef.friction = 0.3f;

	// BUG: possible bug fr dimension dimension_
	// bottom circle
	circleShape.m_p.Set(0.0f, (-dimensions_.y + dimensions_.x) / 2.0f);
	fixtures_[1] = body_->CreateFixture(&circleDef);

	//top circle
	circleShape.m_p.Set(0.0f, (dimensions_.y - dimensions_.x) / 2.0f);
	fixtures_[2] = body_->CreateFixture(&circleDef);

}

void Capsule::drawDebug(PuffEngine::DebugRenderer& debugRenderer) {
	PuffEngine::ColorRGBA8 color(255, 255, 255, 255);
	// draw box
	glm::vec4 destRect;
	destRect.x = body_->GetPosition().x - dimensions_.x / 2.0f;
	destRect.y = body_->GetPosition().y - (dimensions_.y - dimensions_.x) / 2.0f;
	destRect.z = dimensions_.x;
	destRect.w = dimensions_.y - dimensions_.x;
	debugRenderer.drawBox(destRect, PuffEngine::ColorRGBA8(255, 255, 255, 255), body_->GetAngle());

	// draw circles
	//destrect top left corner
	debugRenderer.drawCircle(glm::vec2(destRect.x + dimensions_.x / 2.0f, destRect.y),
							 color,
							 dimensions_.x / 2.0f);
	debugRenderer.drawCircle(glm::vec2(destRect.x + dimensions_.x / 2.0f, destRect.y + destRect.w),
							 color,
							 dimensions_.x / 2.0f);
}
