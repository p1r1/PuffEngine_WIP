#include "PuffErrors.h"

#include <cstdlib>
#include <iostream>
#include <SDL/SDL.h>

namespace PuffEngine {
void fatalError(std::string errorString) {
	std::cout << errorString << std::endl;
	std::cout << "Enter any key to quit...\n\n";
	int tmp;
	std::cin >> tmp;
	SDL_Quit();
	exit(69);
}
}