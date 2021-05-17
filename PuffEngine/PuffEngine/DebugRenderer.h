#pragma once

#include "Vertex.h"
#include "GLSLProgram.h"

#include <glm\glm.hpp>
#include <vector>

namespace PuffEngine {

class DebugRenderer {
public:
	DebugRenderer();
	~DebugRenderer();

	void init();
	//no need for begin
	void end();
	void drawBox(const glm::vec4& destRect, const ColorRGBA8& color, float angle);
	void drawCircle(const glm::vec2& center, const ColorRGBA8& color, float radius);
	void render(const glm::mat4 projectionMatrix, float lineWidth);
	void dispose();

	struct DebugVertex {
		glm::vec2 position;
		ColorRGBA8 color;
	};

private:
	GLSLProgram program_;
	std::vector<DebugVertex> verts_;
	std::vector<GLuint> indices_;
	GLuint vbo_ = 0, vao_ = 0, ibo_ = 0;
	int numElements_ = 0;

};
}
