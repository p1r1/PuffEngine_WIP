#include "DebugRenderer.h"
const float PI = 3.14159265359f;

//hard coded shaders
// for prevent name collisions (maybe use this for box2d compiler errors)
namespace {
	const char* VERT_SRC = R"(#version 130
//NINJA PLATFORMER
//The vertex shader operates on each vertex

//input data from the VBO. Each vertex is 2 floats
in vec2 vertexPosition;
in vec4 vertexColor;

out vec2 fragmentPosition;
out vec4 fragmentColor;

uniform mat4 P;

void main() {
    //Set the x,y position on the screen
    gl_Position.xy = (P * vec4(vertexPosition, 0.0, 1.0)).xy;
    //the z position is zero since we are in 2D
    gl_Position.z = 0.0;
    
    //Indicate that the coordinates are normalized
    gl_Position.w = 1.0;
    
    fragmentPosition = vertexPosition;
    
    fragmentColor = vertexColor;
})";


const char* FRAG_SRC = R"(#version 130
//NINJA PLATFORMER
//The fragment shader operates on each pixel in a given polygon

in vec2 fragmentPosition;
in vec4 fragmentColor;

//This is the 3 component float vector that gets outputted to the screen
//for each pixel.
out vec4 color;

void main() {

    color = fragmentColor;
})";
}

namespace PuffEngine {

DebugRenderer::DebugRenderer() {
	//bos
}


DebugRenderer::~DebugRenderer() {
	dispose();
}

void DebugRenderer::init() {
	// init shaders
	program_.compileShadersFromSource(VERT_SRC, FRAG_SRC);
	program_.addAttribute("vertexPosition");
	program_.addAttribute("vertexColor");
	program_.linkShaders();

	// set up buffers
	glGenVertexArrays(1, & vao_);//vertex array object
	glGenBuffers(1, &vbo_);// vertex buffer object
	glGenBuffers(1, &ibo_); //indicies buffer object

	glBindVertexArray(vao_);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_);

	// Vertex attrib pointers
	//position
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(DebugVertex), (void*)offsetof(DebugVertex, position));
	//color
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(DebugVertex), (void*)offsetof(DebugVertex, color));

	glBindVertexArray(0);

}

void DebugRenderer::end() {
	//VBO
	// bind the buffer object
	glBindBuffer(GL_ARRAY_BUFFER, vbo_);
	// orphan buffer	 
	glBufferData(GL_ARRAY_BUFFER, verts_.size() * sizeof(DebugVertex), nullptr, GL_DYNAMIC_DRAW);
	// upload data
	glBufferSubData(GL_ARRAY_BUFFER, 0, verts_.size() * sizeof(DebugVertex), verts_.data());
	//unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	//IBO
	// bind the buffer object
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_);
	// orphan buffer	 
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices_.size() * sizeof(GLuint), nullptr, GL_DYNAMIC_DRAW);
	// upload data
	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, indices_.size() * sizeof(GLuint), indices_.data());
	//unbind
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	numElements_ = indices_.size();
	indices_.clear();
	verts_.clear();
}

glm::vec2 rotatePoint(const glm::vec2& pos, float angle) {
	glm::vec2 newV;
	// rotate a point around agent
	newV.x = pos.x * cos(angle) - pos.y * sin(angle);
	newV.y = pos.x * sin(angle) + pos.y * cos(angle);
	return newV;
}

void DebugRenderer::drawBox(const glm::vec4 & destRect, const ColorRGBA8 & color, float angle) {
	
	int i = verts_.size();
	verts_.resize(verts_.size() + 4);
		
	//destrect x,y and -->	  witdh vvv			height vvv
	glm::vec2 halfDimensions(destRect.z / 2.0f, destRect.w / 2.0f);

	// get points centered  origin
	glm::vec2 tl(-halfDimensions.x, halfDimensions.y);
	glm::vec2 bl(-halfDimensions.x, -halfDimensions.y);
	glm::vec2 br(halfDimensions.x, -halfDimensions.y);
	glm::vec2 tr(halfDimensions.x, halfDimensions.y);

	glm::vec2 positionOffset(destRect.x, destRect.y);

	// rotate the points
	// our convention is "topleft corner is the center"

	verts_[i].position = rotatePoint(tl, angle) + halfDimensions + positionOffset;
	verts_[i + 1].position = rotatePoint(bl, angle) + halfDimensions + positionOffset;
	verts_[i + 2].position = rotatePoint(br, angle) + halfDimensions + positionOffset;
	verts_[i + 3].position = rotatePoint(tr, angle) + halfDimensions + positionOffset;

	for (int j = i; j < i + 4; j++) {
		verts_[j].color = color;
	}
	// reserve a memory for 8 verticies
	indices_.reserve(indices_.size() + 8);

	// draw line counter clock wise from 4 points
	//first line for box
	indices_.push_back(i);
	indices_.push_back(i + 1);

	//second line for box
	indices_.push_back(i + 1);
	indices_.push_back(i + 2);

	//third line for box
	indices_.push_back(i + 2);
	indices_.push_back(i + 3);	
	
	//fourth line for box
	indices_.push_back(i + 3);
	indices_.push_back(i);
}

void DebugRenderer::drawCircle(const glm::vec2& center, const ColorRGBA8& color, float radius) {
	// set up verticies
	static const int NUM_VERTS = 100;
	int start = verts_.size();
	verts_.resize(verts_.size() + NUM_VERTS);

	for (int i = 0; i < NUM_VERTS; i++) {
		// angle
		float angle = ((float)i / NUM_VERTS) * 2.0f * PI;
		// set positions
		verts_[start + i].position.x = cos(angle) * radius + center.x;
		verts_[start + i].position.y = sin(angle) * radius + center.y;
		
		verts_[start + i].color = color;
	}
	// set up indicies for drawing ccw
	indices_.reserve(indices_.size() + NUM_VERTS * 2);

	for (int i = 0; i < NUM_VERTS - 1; i++) {
		indices_.push_back(start + i );
		indices_.push_back(start + i + 1);
	}
	//last one is diffrent
	indices_.push_back(start + NUM_VERTS - 1);
	indices_.push_back(start);
}

void DebugRenderer::render(const glm::mat4 projectionMatrix, float lineWidth) {
	program_.use();

	//camera matrix
	GLint pUniform = program_.getUniformLocation("P");
	glUniformMatrix4fv(pUniform, 1, GL_FALSE, &projectionMatrix[0][0]);

	// set the line
	glLineWidth(lineWidth);

	// bind vao and draw with elements(indicies) not verticies
	glBindVertexArray(vao_);
	glDrawElements(GL_LINES, numElements_, GL_UNSIGNED_INT, 0); //debug vertex offset is 0
	glBindVertexArray(0);

	program_.unUse();
}

void DebugRenderer::dispose() {
	if (vao_) { // != 0
		glDeleteVertexArrays(1, &vao_);
	}
	if (vao_) { // != 0
		glDeleteBuffers(1, &vbo_);
	}
	if (vao_) { // != 0
		glDeleteBuffers(1, &ibo_);
	}

	program_.dispose();
}

}