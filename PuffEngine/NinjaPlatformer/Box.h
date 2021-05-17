#pragma once
#include <Box2D\Box2D.h>
#include <glm\glm.hpp>
#include <PuffEngine\Vertex.h>
#include <PuffEngine\SpriteBatch.h>
#include <PuffEngine\GLTexture.h>

class Box {
public:
	Box();
	~Box();

	void init(b2World* world, 
			  const glm::vec2& position, 
			  const glm::vec2& dimension, 
			  PuffEngine::GLTexture texture,
			  PuffEngine::ColorRGBA8 color, 
			  bool fixedRotation,
			  glm::vec4 uvRect = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f));
	
	void draw(PuffEngine::SpriteBatch& spriteBatch);
	//getters
	b2Body* getBody() { return body_; }
	b2Fixture* getFixture() { return fixture_; }
	const glm::vec2& getDimension() const { return dimensions_; }
	const PuffEngine::ColorRGBA8& getColor() const { return color_; }

private:
	glm::vec4 uvRect_;
	b2Body* body_ = nullptr;
	b2Fixture* fixture_ = nullptr;
	glm::vec2 dimensions_;
	PuffEngine::ColorRGBA8 color_;
	PuffEngine::GLTexture texture_;
};

