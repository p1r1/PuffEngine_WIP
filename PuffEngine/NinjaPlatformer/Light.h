#pragma once

#include <PuffEngine\Vertex.h>
#include <PuffEngine\SpriteBatch.h>
#include <glm\glm.hpp>

class Light {
	//basic circluar lights
public:
	void draw(PuffEngine::SpriteBatch& spriteBatch) {
		glm::vec4 destRect;
		// position of the boxes are centered so we need to sbtract half dimension
		destRect.x = position.x - size / 2.0f; // positionX
		destRect.y = position.y - size / 2.0f; // positionY
		destRect.z = size; //width
		destRect.w = size; //height

		// uvrect for texture atlas
		spriteBatch.draw(destRect, glm::vec4(-1.0f, -1.0f, 2.0f, 2.0f), 0, 0.0f, color, 0);
	}

	PuffEngine::ColorRGBA8 color;
	glm::vec2 position;
	float size;


};

