#pragma once

#include <unordered_map>
#include <glm/glm.hpp>

namespace PuffEngine {

class InputManager {
public:
	InputManager();
	~InputManager();

	void update();

	void pressKey(unsigned int keyID);
	void releaseKey(unsigned int keyID);

	void setMouseCoords(float x, float y);
	//returns true if the key held down
	bool isKeyDown(unsigned int keyID);
	//returns true if the key was just pressed
	bool isKeyPressed(unsigned int keyID);

	//Getters
	glm::vec2 getMouseCoords() const { return mouseCoords_; }
private:
	std::unordered_map<unsigned int, bool> keyMap_;
	std::unordered_map<unsigned int, bool> previousKeyMap_;
	glm::vec2 mouseCoords_;


	bool wasKeyDown(unsigned int keyID);
};

}