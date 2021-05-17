#include "Camera2D.h"


namespace PuffEngine {
Camera2D::Camera2D() :
	screenWidth_(640),
	screenHeight_(480),
	needsMatrixUpdate_(true),
	scale_(1.0f),
	position(0.0f, 0.0f),
	cameraMatrix_(1.0f),
	orthoMatrix_(1.0f)
{
}

Camera2D::~Camera2D() {
}

void Camera2D::init(int screenWidth, int screenHeight) {
	screenWidth_ = screenWidth;
	screenHeight_ = screenHeight;
	orthoMatrix_ = glm::ortho(0.0f, (float)screenWidth_, 0.0f, (float)screenHeight_);

}

void Camera2D::update() {
	if (needsMatrixUpdate_) {
		//camer translation
		glm::vec3 translate(-position.x + screenWidth_/2, -position.y + screenHeight_/2, 0.0f);
		cameraMatrix_ = glm::translate(orthoMatrix_, translate);
			
		//camer Scale
		glm::vec3 scale(scale_, scale_, 0.0f);
		cameraMatrix_ = glm::scale(glm::mat4(1.0f), scale) * cameraMatrix_;
		needsMatrixUpdate_ = false;
	}
}

glm::vec2 Camera2D::convertScreenToWorlds(glm::vec2 screenCoords) {
	//invert y direction
	screenCoords.y = screenHeight_ - screenCoords.y;
	// 0,0 is the center now
	screenCoords -= glm::vec2(screenWidth_ / 2, screenHeight_ / 2);
	//scale then translate
	screenCoords /= scale_;
	screenCoords += position;
	return screenCoords;
}

//AABB for draw object which are in the view of camera
bool Camera2D::isBoxInView(const glm::vec2& position, const glm::vec2& dimension) {
	glm::vec2 scaledScreenDimension = glm::vec2(screenWidth_, screenHeight_) / (scale_); ///< *2.0f for checking
	const float MIN_DISTANCE_X = dimension.x / 2.0f + scaledScreenDimension.x / 2.0f;
	const float MIN_DISTANCE_Y = dimension.y / 2.0f + scaledScreenDimension.y / 2.0f;

	//agent center position
	glm::vec2 centerPos = position + dimension / 2.0f;
	//camera center position
	glm::vec2 cameraCenterPos = position;///< position is already in center
	glm::vec2 distVec = centerPos - cameraCenterPos;

	float xDepth = MIN_DISTANCE_X - abs(distVec.x);
	float yDepth = MIN_DISTANCE_Y - abs(distVec.y);

	//if this is true, we are colliding
	if (xDepth > 0 && yDepth > 0) {
		///there was a collision	
		return true;
	}
	return false;
}

//end of namespace
}