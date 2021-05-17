#include "ParticleBatch2D.h"


namespace PuffEngine {

ParticleBatch2D::ParticleBatch2D() {
	/// Empty
}


ParticleBatch2D::~ParticleBatch2D() {
	delete[] particles_;
}

void ParticleBatch2D::init(int maxParticles, 
						   float decayRate, 
						   GLTexture texture,
						   std::function<void(Particle2D&, float)> updateFunc /*= defaultParticleUpdate*/) {///< func pointer
	texture_ = texture;
	maxParticles_ = maxParticles;
	decayRate_ = decayRate;
	particles_ = new Particle2D[maxParticles];///< allocate memory in the heap
	updateFunc_ = updateFunc;
}


void ParticleBatch2D::update(float deltaTime) {
	for (int i = 0; i < maxParticles_; i++) {
		//check it is active 
		if (particles_[i].life > 0.0f) {
			updateFunc_(particles_[i], deltaTime);
			// decay the particles
			particles_[i].life -= decayRate_ * deltaTime;
		}
	}
}

void ParticleBatch2D::draw(SpriteBatch* spriteBatch) {
	glm::vec4 uvRect(0.0f, 0.0f, 1.0f, 1.0f);
	for (int i = 0; i < maxParticles_; i++) {
		auto& p = particles_[i];
		//check it is active 
		if (particles_[i].life > 0.0f) {
			glm::vec4 destRect(p.position.x, p.position.y, p.width, p.width);

			spriteBatch->draw(destRect, uvRect, texture_.id, 0.0f, p.color);
		
		}
	}

}

void ParticleBatch2D::addParticle(const glm::vec2& position,
								  const glm::vec2& velocity,
								  const ColorRGBA8& color,
								  float width) {
	int particleIndex = findFreeParticle();
	auto& p = particles_[particleIndex];

	p.life = 1.0f;
	p.position = position;
	p.velocity = velocity;
	p.color = color;
	p.width = width;
}

int ParticleBatch2D::findFreeParticle() {
	for (int i = lastFreeParticle_; i < maxParticles_; i++) {
		if (particles_[i].life <= 0.0f) { ///< this is the last free particle
			lastFreeParticle_ = i;
			return i;
		}
	}
	for (int i = 0; i < lastFreeParticle_; i++) {
		if (particles_[i].life <= 0.0f) { ///< this is the last free particle
			lastFreeParticle_ = i;
			return i;
		}
	}
	/// no particles are free
	/// overrite first particle
	return 0;
}

}