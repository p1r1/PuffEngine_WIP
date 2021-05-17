#include "PlayerNinja.h"

#include <SDL\SDL.h>

#include <PuffEngine\ResourceManager.h>
#include <PuffEngine\GLTexture.h>


void PlayerNinja::init(b2World* world,
					   const glm::vec2& position,
					   const glm::vec2 drawDims,
					   const glm::vec2& collisionDims,
					   PuffEngine::ColorRGBA8 color) {
	PuffEngine::GLTexture texture = PuffEngine::ResourceManager::getTexture("Assets/blue_ninja.png");

	color_ = color;
	drawDims_ = drawDims;
	capsule_.init(world, position, collisionDims, 1.0f, 0.1f, true);
	
	texture_.init(texture, glm::ivec2(10, 2));
}

void PlayerNinja::draw(PuffEngine::SpriteBatch& spriteBatch) {
	glm::vec4 destRect;
	b2Body* body = capsule_.getBody();
	// position of the boxes are centered so we need to sbtract half dimension
	destRect.x = body->GetPosition().x - drawDims_.x / 2.0f; // positionX
	destRect.y = body->GetPosition().y -  capsule_.getDimension().y / 2.0f; // positionY
	destRect.z = drawDims_.x; //width
	destRect.w = drawDims_.y; //height

	// calculate animation
	int tileIndex;
	int numTiles;
	glm::vec2 velocity (body->GetLinearVelocity().x, body->GetLinearVelocity().y);
	float animSpeed = 0.2f;

	if (onGround_) {
		if (isPunching_) {
			// on the ground punch
			numTiles = 4;
			tileIndex = 1;
			if (moveSatate_ != PlayerMoveState::PUNCHING) {
				moveSatate_ = PlayerMoveState::PUNCHING;
				animeTime_ = 0.0f;
			}
		}
		// check if the velocity good enough to play walking animation
		else if (abs(velocity.x) > 1.0f && ((velocity.x > 0 && direction_ > 0) || ((velocity.x < 0 && direction_ < 0)))) {
			// running 
			// 10.cizimden basliyor ve 6  animasyon var
			numTiles = 6;
			tileIndex = 10;
			animSpeed = abs(velocity.x) * 0.025f;
			if (moveSatate_ != PlayerMoveState::RUNNING) {
				moveSatate_ = PlayerMoveState::RUNNING;
				animeTime_ = 0.0f;
			}
		}
		else {
			// standing stilll
			numTiles = 1;
			tileIndex = 0;
			moveSatate_ = PlayerMoveState::STANDING;
		}
	}
	else {
		//in the air
		if (isPunching_) {
			// in the air kick(puch)
			numTiles = 1;
			tileIndex = 18;
			animSpeed *= 0.25f;// ormal punch is 4 frames
			if (moveSatate_ != PlayerMoveState::PUNCHING) {
				moveSatate_ = PlayerMoveState::PUNCHING;
				animeTime_ = 0.0f;
			}
		}
		else if (abs(velocity.x) > 10.0f) {
			// glading in the air
			// if you fast enough and jump applay diffrent animation
			numTiles = 1;
			tileIndex = 10;
			moveSatate_ = PlayerMoveState::IN_AIR;
		}
		else if (velocity.y <= 0.0f) { // falling
			// falling
			numTiles = 1;
			tileIndex = 17;
			moveSatate_ = PlayerMoveState::IN_AIR;
		}
		else {
			// rising
			numTiles = 1;
			tileIndex = 16;
			moveSatate_ = PlayerMoveState::IN_AIR;
		}
	}

	//Increment animetio time 
	// TODO: add deltatime for time step
	animeTime_ += animSpeed;

	//check for punch end
	if (animeTime_ > numTiles) isPunching_ = false;

	//applay animetion
	tileIndex = tileIndex + (int)animeTime_ % numTiles;

	// get the uv coordiates from the tile index
	glm::vec4  uvRect = texture_.getUV(tileIndex);

	// facing direction right or left
	if (direction_ == -1) {
		uvRect.x += 1.0f / texture_.dims.x;
		uvRect.z *= -1;
	}

	// draw sprite uvrect for texture atlas
	spriteBatch.draw(destRect, uvRect, texture_.texture.id, 0.0f, color_, body->GetAngle());

}

void PlayerNinja::drawDebug(PuffEngine::DebugRenderer & debugRenderer) {
	capsule_.drawDebug(debugRenderer);
}

void PlayerNinja::update(PuffEngine::InputManager& inputManager) {
	b2Body* body = capsule_.getBody();

	if (inputManager.isKeyDown(SDLK_a)) {
		body->ApplyForceToCenter(b2Vec2(-100.0, 0.0), true);
		direction_ = -1;
	}
	else if (inputManager.isKeyDown(SDLK_d)) {
		body->ApplyForceToCenter(b2Vec2(100.0, 0.0), true);
		direction_ = 1;
	}
	else {
		// apply damping
		body->SetLinearVelocity(b2Vec2(body->GetLinearVelocity().x * 0.95, body->GetLinearVelocity().y));
	}
	
	if (inputManager.isKeyPressed(SDLK_SPACE)) {
		isPunching_ = true;
	}

	// MAX PLAYER SPEED
	float MAX_SPEED = 10.0f;
	if (body->GetLinearVelocity().x < -MAX_SPEED) {
		body->SetLinearVelocity(b2Vec2(-MAX_SPEED, body->GetLinearVelocity().y));
	}
	else if (body->GetLinearVelocity().x > MAX_SPEED) {
		body->SetLinearVelocity(b2Vec2(MAX_SPEED, body->GetLinearVelocity().y));
	}
	
	//iterate trough between contact points(std::list)
	onGround_ = false;
	for (b2ContactEdge * ce = body->GetContactList(); ce != nullptr; ce= ce->next) {
		b2Contact* c = ce->contact;
		//touching our player
		if (c->IsTouching()) {
			b2WorldManifold manifold;
			c->GetWorldManifold(&manifold);
			//check if the points are below
			bool isBelow = false;
			for (int i = 0; i < b2_maxManifoldPoints; i++) {
				if (manifold.points[i].y < body->GetPosition().y - capsule_.getDimension().y / 2.0f + capsule_.getDimension().x / 2.0f +0.01f) {
					isBelow = true;
					break;
				}
			}
			if (isBelow){
				onGround_ = true;
				//we can jump
				if (inputManager.isKeyPressed(SDLK_w)) {
					body->ApplyLinearImpulse(b2Vec2(0.0f, 20.0f), b2Vec2(0.0f, 0.0f), true);
					break;
				}
			}
		}
	}
}