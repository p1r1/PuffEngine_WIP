#pragma once

#include <PuffEngine/SpriteBatch.h>
#include <PuffEngine/GLSLProgram.h>
#include <vector>
#include <memory>
#include "Ball.h"

// Ball renderer interface
class BallRenderer {
public:
    virtual void renderBalls(PuffEngine::SpriteBatch& spriteBatch, const std::vector<Ball>& balls,
                             const glm::mat4& projectionMatrix);
protected:
    std::unique_ptr<PuffEngine::GLSLProgram> m_program = nullptr;
};

// Visualizes kinetic energy
class MomentumBallRenderer : public BallRenderer {
public:
    virtual void renderBalls(PuffEngine::SpriteBatch& spriteBatch, const std::vector<Ball>& balls,
                             const glm::mat4& projectionMatrix) override;
};

// Visualizes positive X component of velocity, as well as position
class VelocityBallRenderer : public BallRenderer {
public:
    VelocityBallRenderer(int screenWidth, int screenHeight);

    virtual void renderBalls(PuffEngine::SpriteBatch& spriteBatch, const std::vector<Ball>& balls,
                             const glm::mat4& projectionMatrix) override;
private:
    int m_screenWidth;
    int m_screenHeight;
};

// Trippy renderer!
class TrippyBallRenderer : public BallRenderer {
public:
    TrippyBallRenderer(int screenWidth, int screenHeight);

    virtual void renderBalls(PuffEngine::SpriteBatch& spriteBatch, const std::vector<Ball>& balls,
                             const glm::mat4& projectionMatrix) override;
private:
    int m_screenWidth;
    int m_screenHeight;
    float m_time = 0.0f;
};

class FabionacciRenderer : public BallRenderer {
public:
	FabionacciRenderer(int screenWidth, int screenHeight);

	virtual void renderBalls(PuffEngine::SpriteBatch& spriteBatch, const std::vector<Ball>& balls,
							 const glm::mat4& projectionMatrix) override;
private:
	int m_screenWidth;
	int m_screenHeight;
	float m_time = 0.0f;
};