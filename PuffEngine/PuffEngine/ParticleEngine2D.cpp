#include "ParticleEngine2D.h"

#include "ParticleBatch2D.h"
#include "SpriteBatch.h"

namespace PuffEngine {
;

ParticleEngine2D::ParticleEngine2D() {
	///bos
}


ParticleEngine2D::~ParticleEngine2D() {
	for (auto& b : batches_) {
		delete b;
	}
}

void ParticleEngine2D::addParticleBatch(ParticleBatch2D* particleBatch) {
	batches_.push_back(particleBatch);
}

void ParticleEngine2D::update(float deltaTime) {
	for (auto& b : batches_) {
		b->update(deltaTime);
	}
}

void ParticleEngine2D::draw(SpriteBatch* spriteBatch) {
	//spriteBatch->init();

	for (auto& b : batches_) {
		spriteBatch->begin();
		b->draw(spriteBatch);
		spriteBatch->end();
		spriteBatch->renderBatch();
	}
}

}