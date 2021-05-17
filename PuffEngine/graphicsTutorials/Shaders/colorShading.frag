//language glsl
//opengl version 3.0 widely supported modern version
//3.0 colors for vbo_ and interolation
#version 130
//The fragment shader operates on each pixel in a given polygon
//This is the 3 component float vector that gets outputted to the screen
//for each pixel

in vec2 fragmentPosition;
in vec4 fragmentColor;
in vec2 fragmentUV;

//output a color
out vec4 color;

//send a single mesh
uniform float time;
uniform sampler2D mySampler;

void main(){
	vec4 textureColor = texture(mySampler, fragmentUV);

//	color = vec4(fragmentColor.r * (cos(fragmentPosition.x  * 4.0+ time) + 1.0) * 0.5,
//				fragmentColor.g * (cos(fragmentPosition.y * 8.0+ time) + 1.0) * 0.5,
//				fragmentColor.b * (cos(fragmentPosition.x * 0.2 + time) + 1.0) * 0.5,
//				fragmentColor.a) * textureColor ; // rgba
	color = fragmentColor * textureColor; 
}