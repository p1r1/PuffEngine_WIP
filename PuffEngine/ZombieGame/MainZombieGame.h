#pragma once

#include <PuffEngine\Window.h>
#include <PuffEngine\GLSLProgram.h>
#include <PuffEngine\Camera2D.h>
#include <PuffEngine\InputManager.h>
#include <PuffEngine\SpriteBatch.h>
#include <PuffEngine\SpriteFont.h>
#include <PuffEngine\AudioEngine.h>
#include <PuffEngine\ParticleEngine2D.h>
#include <PuffEngine\ParticleBatch2D.h>


#include "Level.h"
#include "Player.h"
#include "ZombieBullet.h"

class Zombie;

enum class GameState {
	PLAY,
	EXIT
};

class MainZombieGame
{
public:
	MainZombieGame();
    ~MainZombieGame();

    /// Runs the game
    void run();

private:
    /// Initializes the core systems
    void initSystems();
	/// Initializes the levels
	void initLevel();
    /// Initializes the shaders
    void initShaders();

    /// Main game loop for the program
    void gameLoop();
	//updates all agents
	void updateAgents(float deltaTime);
	//update al bullets
	void updateBullets(float deltaTime);
	///check victory conditions
	void checkVictory();
    /// Handles input processing
    void processInput();
    /// Renders the game
    void drawGame();
	/// Draw the hud
	void drawHud();
	//add blood particles to particle engine
	void addBlood(const glm::vec2 & position, int numParticles);

    /// Member Variables
    PuffEngine::Window window_; ///< The game window
	PuffEngine::GLSLProgram textureProgram_; ///< The shader program
	PuffEngine::InputManager inputManager_; ///< Handles input
    PuffEngine::Camera2D camera_; ///< Main Camera
	PuffEngine::Camera2D hudCamera_; ///< Main Camera
	PuffEngine::SpriteBatch agentSpriteBatch_; ///< all agents sprite batch
	PuffEngine::SpriteBatch hudSpriteBathch_; ///< spritebatch for fonts
	PuffEngine::SpriteFont* spriFont_; ///< font member
	PuffEngine::AudioEngine audioEngine_; ///< Main audio handling
	PuffEngine::ParticleEngine2D particleEngine_; ///< handles particles
	PuffEngine::ParticleBatch2D* bloodParticleBatch; ///< blood effect particles

	std::vector<Level*> levels_; ///< vector of all levels
	std::vector<Human*> humans_; ///< all humans including player
	std::vector<Zombie*> zombies_; ///< all zombies
	std::vector<ZombieBullet> bullets_; ///< all bullets

	Player* player_;

	int screenWidth_, screeHeight_;
	float fps_;
	int currentLevel_;
	int numHumansKilled_; ///< humans killed by player
	int numZombiesKilled_; ///< zombies killed by player
	
	
	GameState gameState_;
};
