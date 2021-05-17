//language glsl
//opengl version 3.0 widely supported modern version
//3.0 vertex glsl program for vertex buffor object (vbo_)
#version 130
//The vertex shader operates on each vertex
//input data from the vbo. Each vertex is 2 floats

//we just have a position now.
//input x and y float
in vec2 vertexPosition;
in vec4 vertexColor;
in vec2 vertexUV;


out vec2 fragmentPosition;
out vec4 fragmentColor;
out vec2 fragmentUV;

uniform mat4 P; 

void main (){
	//gl_Position is vec4 --> x,y,z,w
	gl_Position.xy = (P * vec4(vertexPosition, 0.0, 1.0)).xy;
	gl_Position.z = 0.0;
	//Indicate that the coordinates are notmalized
	gl_Position.w = 1.0;
	
	fragmentPosition = vertexPosition;
	
	fragmentColor = vertexColor;
	
	fragmentUV = vec2(vertexUV.x, 1.0 - vertexUV.y);
}