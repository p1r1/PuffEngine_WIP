#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <vector>

#include "Vertex.h"

namespace PuffEngine {

// Determines how we should sort the glyphs
enum class GlyphSortType {
	NONE,
	FRONT_TO_BACK,
	BACK_TO_FRONT,
	TEXTURE
};

// A glyph is a single quad. These are added via SpriteBatch::draw
class Glyph {
public:
	Glyph() {};
	Glyph(const glm::vec4& destRect, const glm::vec4& uvRect, GLuint Texture, float Depth, const ColorRGBA8& color);
	Glyph(const glm::vec4& destRect, const glm::vec4& uvRect, GLuint Texture, float Depth, const ColorRGBA8& color, float angle);

	GLuint texture;
	float depth;

	Vertex topLeft;
	Vertex bottomLeft;
	Vertex topRight;
	Vertex bottomRight;
private:
	glm::vec2 rotatePoint(const glm::vec2& pos, float angle);
};

// Each render batch is used for a single draw call
class RenderBatch {
public:
	RenderBatch(GLuint Offset, GLuint NumVertices, GLuint Texture) : offset(Offset),
		numVertices(NumVertices), texture(Texture) {
	}
	GLuint offset;
	GLuint numVertices;
	GLuint texture;
};

// The SpriteBatch class is a more efficient way of drawing sprites
class SpriteBatch {
public:
	SpriteBatch();
	~SpriteBatch();

	// Initializes the spritebatch
	void init();

	// Begins o the  f spritebatch m
	void begin(GlyphSortType sortType = GlyphSortType::TEXTURE);

	// Ends the spritebatch
	void end();

	// Adds a glyph to the spritebatch
	void draw(const glm::vec4& destRect, const glm::vec4& uvRect, GLuint texture, float depth, const ColorRGBA8& color);
	// overloaded with angle
	void draw(const glm::vec4& destRect, const glm::vec4& uvRect, GLuint texture, float depth, const ColorRGBA8& color, float angle);
	// overloaded with direction
	void draw(const glm::vec4& destRect, const glm::vec4& uvRect, GLuint texture, float depth, const ColorRGBA8& color, const glm::vec2& dir);


	// Renders the entire SpriteBatch to the screen
	void renderBatch();

private:
	// Creates all the needed RenderBatches
	void createRenderBatches();

	// Generates our VAO and VBO
	void createVertexArray();

	// Sorts glyphs according to _sortType
	void sortGlyphs();

	// Comparators used by sortGlyphs()
	static bool compareFrontToBack(Glyph* a, Glyph* b);
	static bool compareBackToFront(Glyph* a, Glyph* b);
	static bool compareTexture(Glyph* a, Glyph* b);

	GLuint vbo_;
	GLuint vao_;

	GlyphSortType sortType_;

	std::vector<Glyph> glyphs_;///< actual gly1phs for get rid of new and delete
	std::vector<Glyph*> glyphPointers_;///< for sorting
	std::vector<RenderBatch> renderBatches_;
};

}

