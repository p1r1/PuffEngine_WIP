#include "Box.h"



Box::Box() {
}


Box::~Box() {
}

void Box::init(b2World * world, 
			   const glm::vec2& position, 
			   const glm::vec2& dimension, 
			   PuffEngine::GLTexture texture,
			   PuffEngine::ColorRGBA8 color, 
			   bool fixedRotation,
			   glm::vec4 uvRect /* = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f)*/) {
	dimensions_ = dimension;
	color_ = color;
	texture_ = texture;
	uvRect_ = uvRect;

	//make the body
	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.position.Set(position.x, position.y);
	bodyDef.fixedRotation = fixedRotation;

	body_ = world->CreateBody(&bodyDef);

	// fixture and shape
	b2PolygonShape boxShape;
	boxShape.SetAsBox(dimension.x / 2.0f, dimension.y / 2.0f);

	b2FixtureDef fixtureDef;
	fixtureDef.shape = &boxShape;
	fixtureDef.density = 1.0f;
	fixtureDef.friction = 0.3f;
	fixture_ = body_->CreateFixture(&fixtureDef);

}

void Box::draw(PuffEngine::SpriteBatch& spriteBatch) {
	glm::vec4 destRect;
	// position of the boxes are centered so we need to sbtract half dimension
	destRect.x = body_->GetPosition().x - dimensions_.x / 2.0f; // positionX
	destRect.y = body_->GetPosition().y - dimensions_.y / 2.0f; // positionY
	destRect.z = dimensions_.x; //width
	destRect.w = dimensions_.y; //height

	// uvrect for texture atlas
	spriteBatch.draw(destRect, uvRect_, texture_.id, 0.0f, color_, body_->GetAngle());

}
