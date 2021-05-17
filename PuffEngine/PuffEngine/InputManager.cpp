#include "InputManager.h"


namespace PuffEngine {

InputManager::InputManager() :
	mouseCoords_(0.0f)

{
}


InputManager::~InputManager() {
}

void InputManager::update() {
	for (auto& it : keyMap_) {
		previousKeyMap_[it.first] = it.second;
	}
	/*foreach
	for (auto it = keyMap_.begin(); it != keyMap_.end(); it++) {
		previousKeyMap_[it->first] = it->second;
	}
	*/
}


void InputManager::pressKey(unsigned int keyID) {
	keyMap_[keyID] = true;
}

void InputManager::releaseKey(unsigned int keyID) {
	keyMap_[keyID] = false;
}

void InputManager::setMouseCoords(float x, float y) {
	mouseCoords_.x = x;
	mouseCoords_.y = y;
}

bool InputManager::isKeyDown(unsigned int keyID) {
	auto it = keyMap_.find(keyID);
	if (it != keyMap_.end()) {
		return it->second;
	}
	return false;
}

bool InputManager::isKeyPressed(unsigned int keyID) {
	// check is pressed in this frame but not the last frame
	if (isKeyDown(keyID) && wasKeyDown(keyID) == false) {
		return true;
	}

	return false;
}

bool InputManager::wasKeyDown(unsigned int keyID) {
	auto it = previousKeyMap_.find(keyID);
	if (it != previousKeyMap_.end()) {
		return it->second;
	}
	return false;
}
}