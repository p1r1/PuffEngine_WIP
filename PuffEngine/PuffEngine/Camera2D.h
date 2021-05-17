#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace PuffEngine {
class Camera2D {
public:
	Camera2D();
	~Camera2D();

	void init(int screenWidth, int screenHeight);
	void update();

	glm::vec2 convertScreenToWorlds(glm::vec2 screenCoords);

	bool isBoxInView(const glm::vec2& position, const glm::vec2& dimension);

	//setters
	void setPosition(const glm::vec2& newPosition) { position = newPosition; needsMatrixUpdate_ = true; }
	void setScale(float newScale) { scale_ = newScale; needsMatrixUpdate_ = true; }

	//getters
	float getScale() { return scale_; }
	glm::vec2 getPosition() { return position; }
	glm::mat4 getCameraMatrix() { return cameraMatrix_; }
	
private:
	int screenWidth_, screenHeight_;
	bool needsMatrixUpdate_;
	float scale_;
	glm::vec2 position;
	glm::mat4 cameraMatrix_;
	glm::mat4 orthoMatrix_;

	};

}