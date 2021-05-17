#pragma once
#include <GL/glew.h>
#include <string>

#include "GLTexture.h"

namespace PuffEngine {
class Sprite {
public:
	Sprite();
	~Sprite();

	void init(float x, float y, float width, float height, std::string texturePath);
	void draw();

private:
	float x_;
	float y_;
	float width_;
	float height_;

	GLuint  vboID_; //gather all drawings and send it to gpu
	GLTexture texture_;
};
}
