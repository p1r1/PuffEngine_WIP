#include "Sprite.h"
#include "Vertex.h"
#include "ResourceManager.h"

#include <cstddef>

namespace PuffEngine {
Sprite::Sprite() {
	vboID_ = 0;
}


Sprite::~Sprite() {
	if (vboID_ != 0) {
		glDeleteBuffers(1, &vboID_);
	}
}

void Sprite::init(float x, float y, float width, float height, std::string texturePath) {
	x_ = x;
	y_ = y;
	width_ = width;
	height_ = height;

	//texture cahce
	texture_ = ResourceManager::getTexture(texturePath);

	if (vboID_ == 0) {
		// generate a vertex buffer object
		glGenBuffers(1, &vboID_);
	}
	//6 dots for 2 triangle
	Vertex vertexData[6];
	//first triangle interleaved data
	//topright
	vertexData[0].setPosition(x + width, y + height);
	vertexData[0].setUV(1.0f, 1.0f);

	//topleft
	vertexData[1].setPosition(x, y + height);
	vertexData[1].setUV(0.0f, 1.0f);

	//bottomleft
	vertexData[2].setPosition(x, y);
	vertexData[2].setUV(0.0f, 0.0f);

	//second triangle
	//bottomleft
	vertexData[3].setPosition(x, y);
	vertexData[3].setUV(0.0f, 0.0f);

	//bottomright
	vertexData[4].setPosition(x + width, y);
	vertexData[4].setUV(1.0f, 0.0f);

	//topright
	vertexData[5].setPosition(x + width, y + height);
	vertexData[5].setUV(1.0f, 1.0f);

	for (int i = 0; i < 6; i++) {
		vertexData[i].setColor(255, 255, 255, 255);
	}

	//vertexData[1].setColor(0, 0, 255, 255);
	vertexData[4].setColor(0, 255, 0, 255);

	//bind the buffer
	glBindBuffer(GL_ARRAY_BUFFER, vboID_);
	// fill data
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);
	// unbind the buffer
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Sprite::draw() {
	//don't unbind texture maybe it will be used again
	glBindTexture(GL_TEXTURE_2D, texture_.id);

	//bind buffer object
	glBindBuffer(GL_ARRAY_BUFFER, vboID_);

	//draw
	// 0 is begging of data
	//we drawing 6 points 
	glDrawArrays(GL_TRIANGLES, 0, 6);

	//disable the aattribute array 
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

}