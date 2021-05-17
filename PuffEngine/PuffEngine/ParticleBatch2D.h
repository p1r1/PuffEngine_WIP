#pragma once

#include <glm\glm.hpp>
#include <functional>///< function pointers ad lambdas

#include "Vertex.h"
#include "SpriteBatch.h"
#include "GLTexture.h"

namespace PuffEngine {

class Particle2D {
public:
	//void update(float deltaTime);
	glm::vec2 position = glm::vec2(0.0f);
	glm::vec2 velocity = glm::vec2(0.0f);
	ColorRGBA8 color;
	float life = 0.0f;
	float width = 0.0f;///< square particles
};
///vvv use this for header file functions
inline void defaultParticleUpdate(Particle2D& particle, float deltaTime) {
	particle.position += particle.velocity * deltaTime;
}

class ParticleBatch2D {
public:
	ParticleBatch2D();
	~ParticleBatch2D();

	void init(int maxParticles, 
			  float decayRate,
			  GLTexture texture,
			  std::function<void(Particle2D&, float)> updateFunc = defaultParticleUpdate); ///< function pointer
	
	void update(float deltaTime);
	
	void draw(SpriteBatch* spriteBatch);

	void addParticle(const glm::vec2& position, 
					 const glm::vec2& velocity,
					 const ColorRGBA8& color,
					 float width);
	
private:
	int findFreeParticle();
	std::function<void(Particle2D&, float)> updateFunc_;///< function poiter

	float decayRate_ = 0.1f;
	/// we know it is gonna be big batch of particles so
	Particle2D* particles_ = nullptr;///< dynamic array cahce friendly way
	int maxParticles_ = 0;
	int lastFreeParticle_ = 0;///< for finding the last free particles

	GLTexture texture_;
};

}
