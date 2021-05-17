#include "Bullet.h"

#include <PuffEngine/ResourceManager.h>

Bullet::Bullet() {
}

Bullet::Bullet(glm::vec2 pos, glm::vec2 dir, float speed, int lifeTime) {
	lifeTime_ = lifeTime;
	position_ = pos;
	direction_ = dir;
	speed_ = speed;
}


Bullet::~Bullet() {
}

void Bullet::draw(PuffEngine::SpriteBatch& spriteBatch) {
	//bad practice to have static texture
	static PuffEngine::GLTexture texture = PuffEngine::ResourceManager::getTexture("Textures/jimmyJump_pack/\PNG/CharacterRight_Standing.png");
	glm::vec4 uv(0.0f, 0.0f, 1.0f, 1.0f);
	PuffEngine::ColorRGBA8 color;
	color.r = 255;
	color.g = 255;
	color.b = 255;
	color.a = 255;

	glm::vec4 posAndSize = glm::vec4(position_.x, position_.y, 30, 30);

	spriteBatch.draw(posAndSize, uv, texture.id, 0.0f, color);

}

bool Bullet::update() {
	position_ += direction_ * speed_;
	lifeTime_--;
	if (lifeTime_ == 0) {
		return true;
	}
	return false;
}