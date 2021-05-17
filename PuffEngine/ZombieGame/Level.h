#pragma once
#include <string>
#include <vector>
#include <PuffEngine\SpriteBatch.h>

const int TILE_WIDTH = 64;

class Level {
public:
	//load the level
	Level(const std::string& fileName);
	~Level();

	void draw();

	//getterrs
	glm::vec2 getStartPlayerPos() const { return startPlayerPos_; }
	const std::vector<glm::vec2>& getZombieStartPositions() const { return zombieStartPositions_; }
	const std::vector<std::string>& getLevelData() const { return levelData_; }
	int getNumHumans() const { return numHumans_; }
	int getWidth() const { return levelData_[0].size();}
	int getHeight() const { return levelData_.size(); }

private:
	std::vector<std::string> levelData_;
	int numHumans_;
	PuffEngine::SpriteBatch spriteBatch_;
	glm::vec2 startPlayerPos_;
	std::vector<glm::vec2> zombieStartPositions_;
};

