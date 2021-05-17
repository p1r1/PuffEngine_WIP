#pragma once

#include <vector>

namespace PuffEngine {
/// better to have for declaration rather than header file includes
/// this way we need to call these file in cpp
class ParticleBatch2D;
class SpriteBatch;

class ParticleEngine2D {
public:
	ParticleEngine2D();
	~ParticleEngine2D();
	/// After adding a particle batch, the PartcleEngine2D
	/// becomes responsþble for deallocation
	void addParticleBatch(ParticleBatch2D* particleBatch);

	void update(float deltaTime);
	
	void draw(SpriteBatch* spriteBatch);
private:
	std::vector<ParticleBatch2D*> batches_;
};

}

