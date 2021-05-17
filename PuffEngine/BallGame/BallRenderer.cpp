#include "BallRenderer.h"

void BallRenderer::renderBalls(PuffEngine::SpriteBatch& spriteBatch, const std::vector<Ball>& balls,
                               const glm::mat4& projectionMatrix) {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    // Lazily initialize the program
    if (m_program == nullptr) {
        m_program = std::make_unique<PuffEngine::GLSLProgram>();
        m_program->compileShaders("Shaders/textureShading.vert", "Shaders/textureShading.frag");
        m_program->addAttribute("vertexPosition");
        m_program->addAttribute("vertexColor");
        m_program->addAttribute("vertexUV");
        m_program->linkShaders();
    }
    m_program->use();

    spriteBatch.begin();

    // Make sure the shader uses texture 0
    glActiveTexture(GL_TEXTURE0);
    GLint textureUniform = m_program->getUniformLocation("mySampler");
    glUniform1i(textureUniform, 0);

    // Grab the camera matrix
    GLint pUniform = m_program->getUniformLocation("P");
    glUniformMatrix4fv(pUniform, 1, GL_FALSE, &projectionMatrix[0][0]);

    // Render all the balls
    for (auto& ball : balls) {
        const glm::vec4 uvRect(0.0f, 0.0f, 1.0f, 1.0f);
        const glm::vec4 destRect(ball.position.x - ball.radius, ball.position.y - ball.radius,
                                 ball.radius * 2.0f, ball.radius * 2.0f);
        spriteBatch.draw(destRect, uvRect, ball.textureId, 0.0f, ball.color);
    }

    spriteBatch.end();
    spriteBatch.renderBatch();

    m_program->unUse();
}

void MomentumBallRenderer::renderBalls(PuffEngine::SpriteBatch& spriteBatch, const std::vector<Ball>& balls,
                               const glm::mat4& projectionMatrix) {

    glClearColor(0.0f, 0.1f, 0.5f, 1.0f);

    // Lazily initialize the program
    if (m_program == nullptr) {
        m_program = std::make_unique<PuffEngine::GLSLProgram>();
        m_program->compileShaders("Shaders/textureShading.vert", "Shaders/textureShading.frag");
        m_program->addAttribute("vertexPosition");
        m_program->addAttribute("vertexColor");
        m_program->addAttribute("vertexUV");
        m_program->linkShaders();
    }
    m_program->use();

    spriteBatch.begin();

    // Make sure the shader uses texture 0
    glActiveTexture(GL_TEXTURE0);
    GLint textureUniform = m_program->getUniformLocation("mySampler");
    glUniform1i(textureUniform, 0);

    // Grab the camera matrix
    GLint pUniform = m_program->getUniformLocation("P");
    glUniformMatrix4fv(pUniform, 1, GL_FALSE, &projectionMatrix[0][0]);

    // Render all the balls
    for (auto& ball : balls) {
        const glm::vec4 uvRect(0.0f, 0.0f, 1.0f, 1.0f);
        const glm::vec4 destRect(ball.position.x - ball.radius, ball.position.y - ball.radius,
                                 ball.radius * 2.0f, ball.radius * 2.0f);
        PuffEngine::ColorRGBA8 color;
        GLubyte colorVal = (GLubyte)(glm::clamp(glm::length(ball.velocity) * ball.mass * 12.0f, 0.0f, 255.0f));
        color.r = colorVal;
        color.g = colorVal;
        color.b = colorVal;
        color.a = colorVal;
        spriteBatch.draw(destRect, uvRect, ball.textureId, 0.0f, color);
    }

    spriteBatch.end();
    spriteBatch.renderBatch();

    m_program->unUse();
}

VelocityBallRenderer::VelocityBallRenderer(int screenWidth, int screenHeight) :
    m_screenWidth(screenWidth),
    m_screenHeight(screenHeight) {
    // Empty
}


void VelocityBallRenderer::renderBalls(PuffEngine::SpriteBatch& spriteBatch, const std::vector<Ball>& balls,
                                       const glm::mat4& projectionMatrix) {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    // Lazily initialize the program
    if (m_program == nullptr) {
        m_program = std::make_unique<PuffEngine::GLSLProgram>();
        m_program->compileShaders("Shaders/textureShading.vert", "Shaders/textureShading.frag");
        m_program->addAttribute("vertexPosition");
        m_program->addAttribute("vertexColor");
        m_program->addAttribute("vertexUV");
        m_program->linkShaders();
    }
    m_program->use();

    spriteBatch.begin();

    // Make sure the shader uses texture 0
    glActiveTexture(GL_TEXTURE0);
    GLint textureUniform = m_program->getUniformLocation("mySampler");
    glUniform1i(textureUniform, 0);

    // Grab the camera matrix
    GLint pUniform = m_program->getUniformLocation("P");
    glUniformMatrix4fv(pUniform, 1, GL_FALSE, &projectionMatrix[0][0]);

    // Render all the balls
    for (auto& ball : balls) {
        const glm::vec4 uvRect(0.0f, 0.0f, 1.0f, 1.0f);
        const glm::vec4 destRect(ball.position.x - ball.radius, ball.position.y - ball.radius,
                                 ball.radius * 2.0f, ball.radius * 2.0f);
        PuffEngine::ColorRGBA8 color;

        float mult = 100.0f;
        GLubyte colorVal = (GLubyte)(glm::clamp(ball.velocity.x * mult, 0.0f, 255.0f));
        color.r = 128;
        color.g = (GLubyte)((ball.position.x / m_screenWidth) * 255.0f);
        color.b = (GLubyte)((ball.position.y / m_screenHeight) * 255.0f);
        color.a = colorVal;
        spriteBatch.draw(destRect, uvRect, ball.textureId, 0.0f, color);
    }

    spriteBatch.end();
    spriteBatch.renderBatch();

    m_program->unUse();
}

TrippyBallRenderer::TrippyBallRenderer(int screenWidth, int screenHeight) :
    m_screenWidth(screenWidth),
    m_screenHeight(screenHeight) {
    // Empty
}

void TrippyBallRenderer::renderBalls(PuffEngine::SpriteBatch& spriteBatch, const std::vector<Ball>& balls, const glm::mat4& projectionMatrix) {
    glClearColor(0.1f, 0.0f, 0.0f, 1.0f);

    // Lazily initialize the program
    if (m_program == nullptr) {
        m_program = std::make_unique<PuffEngine::GLSLProgram>();
        m_program->compileShaders("Shaders/textureShading.vert", "Shaders/textureShading.frag");
        m_program->addAttribute("vertexPosition");
        m_program->addAttribute("vertexColor");
        m_program->addAttribute("vertexUV");
        m_program->linkShaders();
    }
    m_program->use();

    spriteBatch.begin();

    // Make sure the shader uses texture 0
    glActiveTexture(GL_TEXTURE0);
    GLint textureUniform = m_program->getUniformLocation("mySampler");
    glUniform1i(textureUniform, 0);

    // Grab the camera matrix
    GLint pUniform = m_program->getUniformLocation("P");
    glUniformMatrix4fv(pUniform, 1, GL_FALSE, &projectionMatrix[0][0]);

    // Change these constants to get cool stuff
    float TIME_SPEED = 0.01f;
    float DIVISOR = 4.0f; // Increase to get more arms
    float SPIRAL_INTENSITY = 10.0f; // Increase to make it spiral more

    m_time += TIME_SPEED;

    // Render all the balls
    for (auto& ball : balls) {
        const glm::vec4 uvRect(0.0f, 0.0f, 1.0f, 1.0f);
        const glm::vec4 destRect(ball.position.x - ball.radius, ball.position.y - ball.radius,
                                 ball.radius * 2.0f, ball.radius * 2.0f);
        PuffEngine::ColorRGBA8 color;
       
        // Get vector from center point
        glm::vec2 centerVec = ball.position - glm::vec2(m_screenWidth / 2, m_screenHeight / 2);
        float centerDist = glm::length(centerVec);

        // Get angle from the horizontal
        float angle = atan2(centerVec.x, centerVec.y) / (3.1415926f / DIVISOR);
        // Move with time
        angle -= m_time;
        // Add the spiral
        angle += (centerDist / m_screenWidth) * SPIRAL_INTENSITY;

        color.r = (GLubyte)(angle * 255.0f);
        color.g = (GLubyte)(angle * 255.0f * cos(m_time));
        color.b = (GLubyte)(angle * 255.0f * sin(m_time));
        color.a = (GLubyte)(glm::clamp(1.0f - (centerDist / (m_screenWidth / 2.0f)), 0.0f, 1.0f) * 255.0f);
        spriteBatch.draw(destRect, uvRect, ball.textureId, 0.0f, color);
    }

    spriteBatch.end();
    spriteBatch.renderBatch();

    m_program->unUse();
}

FabionacciRenderer::FabionacciRenderer(int screenWidth, int screenHeight) :
	m_screenWidth(screenWidth),
	m_screenHeight(screenHeight){
	//empty
}

void FabionacciRenderer::renderBalls(PuffEngine::SpriteBatch & spriteBatch, const std::vector<Ball>& balls, const glm::mat4 & projectionMatrix) {
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	//lazyinit
	if (m_program == nullptr) {
		m_program = std::make_unique<PuffEngine::GLSLProgram>();
		m_program->compileShaders("Shaders/textureShading.vert", "Shaders/textureShading.frag");
		m_program->addAttribute("vertexPosition");
		m_program->addAttribute("vertexColor");
		m_program->addAttribute("vertexUV");
		m_program->linkShaders();
	}

	m_program->use();

	spriteBatch.begin();

	//renderer begin

	// Make sure the shader uses texture 0
	glActiveTexture(GL_TEXTURE0);
	GLint textureUniform = m_program->getUniformLocation("mySampler");
	glUniform1i(textureUniform, 0);

	// Grab the camera matrix
	GLint pUniform = m_program->getUniformLocation("P");
	glUniformMatrix4fv(pUniform, 1, GL_FALSE, &projectionMatrix[0][0]);

	// Change these constants to get cool stuff
	float TIME_SPEED = 0.01f;
	float DIVISOR = 4.0f; // Increase to get more arms
	float SPIRAL_INTENSITY = 10.0f; // Increase to make it spiral more

	m_time += TIME_SPEED;

	int x1 = 0; // 112
	int x2 = 1; //123
	int sumx = 0;//1235

	// Render all the balls
	for (auto& ball : balls) {
		const glm::vec4 uvRect(0.0f, 0.0f, 1.0f, 1.0f);
		const glm::vec4 destRect(ball.position.x - ball.radius, ball.position.y - ball.radius,
								 ball.radius * 2.0f, ball.radius * 2.0f);

		// Get vector from center point
		glm::vec2 centerVec = ball.position - glm::vec2(m_screenWidth / 2, m_screenHeight / 2);
		float centerDist = glm::length(centerVec);
		// Get angle from the horizontal
		float angle = atan2(centerVec.x, centerVec.y) / (3.1415926f / DIVISOR);
		// Move with time
		angle -= m_time;
		// Add the spiral
		angle += (centerDist / m_screenWidth) * SPIRAL_INTENSITY;
		// 0 1 1 2 3 5 8 13 
		PuffEngine::ColorRGBA8 color;

		sumx = x1 + x2;
		x1 = x2;
		x2 = sumx;
		color.r = (GLubyte)(angle * 255.0f);
		color.g = (GLubyte)(angle * 255.0f * cos(m_time * sumx));
		color.b = (GLubyte)(angle * 255.0f * sin(m_time * sumx));
		color.a = (GLubyte)(glm::clamp(1.0f - (centerDist / (m_screenWidth / 2.0f)), 0.0f, 1.0f) * 255.0f);

		spriteBatch.draw(destRect, uvRect, ball.textureId, 0.0f, color);
	}

	//renderer end


	spriteBatch.end();
	spriteBatch.renderBatch();

	m_program->unUse();
}
