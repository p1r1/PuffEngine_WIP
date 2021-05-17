#include <SDL/SDL.h>
#include <GL/glew.h>

#include "PuffEngine.h"
namespace PuffEngine {
int init() {
	SDL_Init(SDL_INIT_EVERYTHING);
	//double window buffer for filckering
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1); // 2 windows swap buffer
		
	return 0;
}
}