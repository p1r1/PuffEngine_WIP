//#include <iostream>
#include "MainGameTutorials.h"

int	main(int argc, char** argv) {
	SDL_Init(SDL_INIT_EVERYTHING);
	
	MainGame mainGame;
	mainGame.run();

	return 0;
}