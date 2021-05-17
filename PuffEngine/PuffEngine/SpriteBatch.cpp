#include "SpriteBatch.h"

#include <algorithm>

namespace PuffEngine {

Glyph::Glyph(const glm::vec4& destRect, const glm::vec4& uvRect, GLuint Texture, float Depth, const ColorRGBA8& color) :
	texture(Texture),
	depth(Depth) {

	topLeft.color = color;
	topLeft.setPosition(destRect.x, destRect.y + destRect.w);
	topLeft.setUV(uvRect.x, uvRect.y + uvRect.w);

	bottomLeft.color = color;
	bottomLeft.setPosition(destRect.x, destRect.y);
	bottomLeft.setUV(uvRect.x, uvRect.y);

	bottomRight.color = color;
	bottomRight.setPosition(destRect.x + destRect.z, destRect.y);
	bottomRight.setUV(uvRect.x + uvRect.z, uvRect.y);

	topRight.color = color;
	topRight.setPosition(destRect.x + destRect.z, destRect.y + destRect.w);
	topRight.setUV(uvRect.x + uvRect.z, uvRect.y + uvRect.w);

}

Glyph::Glyph(const glm::vec4& destRect, const glm::vec4& uvRect, GLuint Texture, float Depth, const ColorRGBA8& color, float angle) :
	texture(Texture),
	depth(Depth) {
	//destrect x,y and -->	  witdh vvv			height vvv
	glm::vec2 halfDimensions(destRect.z / 2.0f, destRect.w / 2.0f);

	// get points centered  origin
	glm::vec2 tl(-halfDimensions.x, halfDimensions.y);
	glm::vec2 bl(-halfDimensions.x, -halfDimensions.y);
	glm::vec2 br(halfDimensions.x, -halfDimensions.y);
	glm::vec2 tr(halfDimensions.x, halfDimensions.y);

	// rotate the points
	// our convention is "topleft corner is the center"

	tl = rotatePoint(tl, angle) + halfDimensions;
	bl = rotatePoint(bl, angle) + halfDimensions;
	br = rotatePoint(br, angle) + halfDimensions;
	tr = rotatePoint(tr, angle) + halfDimensions;

	topLeft.color = color;
	topLeft.setPosition(destRect.x + tl.x, destRect.y + tl.y);
	topLeft.setUV(uvRect.x, uvRect.y + uvRect.w);

	bottomLeft.color = color;
	bottomLeft.setPosition(destRect.x + bl.x, destRect.y + bl.y);
	bottomLeft.setUV(uvRect.x, uvRect.y);

	bottomRight.color = color;
	bottomRight.setPosition(destRect.x + br.x, destRect.y + br.y);
	bottomRight.setUV(uvRect.x + uvRect.z, uvRect.y);

	topRight.color = color;
	topRight.setPosition(destRect.x + tr.x, destRect.y + tr.y);
	topRight.setUV(uvRect.x + uvRect.z, uvRect.y + uvRect.w);

}

glm::vec2 Glyph::rotatePoint(const glm::vec2& pos, float angle) {
	glm::vec2 newV;
	// rotate a point around agent
	newV.x = pos.x * cos(angle) - pos.y * sin(angle);
	newV.y = pos.x * sin(angle) + pos.y * cos(angle);
	return newV;
}

SpriteBatch::SpriteBatch() : vbo_(0), vao_(0) {
}


SpriteBatch::~SpriteBatch() {
}


void SpriteBatch::init() {
	createVertexArray();
}

void SpriteBatch::begin(GlyphSortType sortType /* GlyphSortType::TEXTURE */) {
	sortType_ = sortType;
	renderBatches_.clear();
	glyphs_.clear();
}

void SpriteBatch::end() {
	//resize and set up all pointers for better/faster sorting
	glyphPointers_.resize(glyphs_.size());
	for (int i = 0; i < glyphs_.size(); i++) {
		glyphPointers_[i] = &glyphs_[i];
	}

	sortGlyphs();
	createRenderBatches();
}

void SpriteBatch::draw(const glm::vec4& destRect, const glm::vec4& uvRect, GLuint texture, float depth, const ColorRGBA8& color) {
	glyphs_.emplace_back(destRect, uvRect, texture, depth, color);
}

void SpriteBatch::draw(const glm::vec4 & destRect, const glm::vec4 & uvRect, GLuint texture, float depth, const ColorRGBA8 & color, float angle) {
	glyphs_.emplace_back(destRect, uvRect, texture, depth, color, angle);
}

void SpriteBatch::draw(const glm::vec4 & destRect, const glm::vec4 & uvRect, GLuint texture, float depth, const ColorRGBA8 & color, const glm::vec2& dir) {
	const glm::vec2 right(1.0f, 0.0f);// default x ekseni
	float angle = acos(glm::dot(right, dir));
	if (dir.y < 0.0f) {
		angle = -angle;
	}
	glyphs_.emplace_back(destRect, uvRect, texture, depth, color, angle);
}

void SpriteBatch::renderBatch() {

	// Bind our VAO. This sets up the opengl state we need, including the 
	// vertex attribute pointers and it binds the VBO
	glBindVertexArray(vao_);

	for (int i = 0; i < renderBatches_.size(); i++) {
		glBindTexture(GL_TEXTURE_2D, renderBatches_[i].texture);

		glDrawArrays(GL_TRIANGLES, renderBatches_[i].offset, renderBatches_[i].numVertices);
	}

	glBindVertexArray(0);
}

void SpriteBatch::createRenderBatches() {
	// This will store all the vertices that we need to upload
	std::vector <Vertex> vertices;
	// Resize the buffer to the exact size we need so we can treat
	// it like an array
	vertices.resize(glyphPointers_.size() * 6);

	if (glyphPointers_.empty()) {
		return;
	}

	int offset = 0; // current offset
	int cv = 0; // current vertex

				//Add the first batch
	renderBatches_.emplace_back(offset, 6, glyphPointers_[0]->texture);
	vertices[cv++] = glyphPointers_[0]->topLeft;
	vertices[cv++] = glyphPointers_[0]->bottomLeft;
	vertices[cv++] = glyphPointers_[0]->bottomRight;
	vertices[cv++] = glyphPointers_[0]->bottomRight;
	vertices[cv++] = glyphPointers_[0]->topRight;
	vertices[cv++] = glyphPointers_[0]->topLeft;
	offset += 6;

	//Add all the rest of the glyphs
	for (int cg = 1; cg < glyphPointers_.size(); cg++) {

		// Check if this glyph can be part of the current batch
		if (glyphPointers_[cg]->texture != glyphPointers_[cg - 1]->texture) {
			// Make a new batch
			renderBatches_.emplace_back(offset, 6, glyphPointers_[cg]->texture);
		}
		else {
			// If its part of the current batch, just increase numVertices
			renderBatches_.back().numVertices += 6;
		}
		vertices[cv++] = glyphPointers_[cg]->topLeft;
		vertices[cv++] = glyphPointers_[cg]->bottomLeft;
		vertices[cv++] = glyphPointers_[cg]->bottomRight;
		vertices[cv++] = glyphPointers_[cg]->bottomRight;
		vertices[cv++] = glyphPointers_[cg]->topRight;
		vertices[cv++] = glyphPointers_[cg]->topLeft;
		offset += 6;
	}

	// Bind our VBO
	glBindBuffer(GL_ARRAY_BUFFER, vbo_);
	// Orphan the buffer (for speed)
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), nullptr, GL_DYNAMIC_DRAW);
	// Upload the data
	glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(Vertex), vertices.data());

	// Unbind the VBO
	glBindBuffer(GL_ARRAY_BUFFER, 0);

}

void SpriteBatch::createVertexArray() {

	// Generate the VAO if it isn't already generated
	if (vao_ == 0) {
		glGenVertexArrays(1, &vao_);
	}

	// Bind the VAO. All subsequent opengl calls will modify it's state.
	glBindVertexArray(vao_);

	//G enerate the VBO if it isn't already generated
	if (vbo_ == 0) {
		glGenBuffers(1, &vbo_);
	}
	glBindBuffer(GL_ARRAY_BUFFER, vbo_);

	//Tell opengl what attribute arrays we need
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	//This is the position attribute pointer
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
	//This is the color attribute pointer
	glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Vertex), (void*)offsetof(Vertex, color));
	//This is the UV attribute pointer
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));

	glBindVertexArray(0);

}

void SpriteBatch::sortGlyphs() {

	switch (sortType_) {
	case GlyphSortType::BACK_TO_FRONT:
		std::stable_sort(glyphPointers_.begin(), glyphPointers_.end(), compareBackToFront);
		break;
	case GlyphSortType::FRONT_TO_BACK:
		std::stable_sort(glyphPointers_.begin(), glyphPointers_.end(), compareFrontToBack);
		break;
	case GlyphSortType::TEXTURE:
		std::stable_sort(glyphPointers_.begin(), glyphPointers_.end(), compareTexture);
		break;
	}
}

bool SpriteBatch::compareFrontToBack(Glyph* a, Glyph* b) {
	return (a->depth < b->depth);
}

bool SpriteBatch::compareBackToFront(Glyph* a, Glyph* b) {
	return (a->depth > b->depth);
}

bool SpriteBatch::compareTexture(Glyph* a, Glyph* b) {
	return (a->texture < b->texture);
}

}
