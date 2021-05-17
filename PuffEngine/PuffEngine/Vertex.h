#pragma once
#include <GL/glew.h>

namespace PuffEngine {
//8byte
//float positions[2];
struct Position {//^^^ same thing
	float x;
	float y;
} ;

//unsigned char color[3];
//3bytes
//GLubyte color[4];
struct ColorRGBA8 {//composition
	ColorRGBA8() : r(0), g(0), b(0), a(1.0f) {}
	ColorRGBA8(GLubyte R, GLubyte G, GLubyte B, GLubyte A) :
		r(R), g(G), b(B), a(A) {}
	GLubyte r;
	GLubyte g;
	GLubyte b;
	GLubyte a;
};

struct UV {
	//texture coordinates
	float u;
	float v;
};


//11 bytes is bad for aligment issue.we need 12. %4 = 0
struct Vertex {
	Position position;
	ColorRGBA8 color;
	UV uv;

	void setPosition(float x, float y) {
		position.x = x;
		position.y = y;
	}
	void setColor(GLubyte r, GLubyte g, GLubyte b, GLubyte a) {
		color.r = r;
		color.g = g;
		color.b = b;
		color.a = a;
	}
	void setUV(float u, float v) {
		uv.u = u;
		uv.v = v;
	}
};
}