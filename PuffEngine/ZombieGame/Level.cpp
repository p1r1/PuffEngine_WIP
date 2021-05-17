#include "Level.h"
#include <fstream>
#include <PuffEngine\PuffErrors.h>
#include <PuffEngine\ResourceManager.h>


Level::Level(const std::string& fileName) :
	numHumans_(0)
{
	std::ifstream file;
	file.open(fileName);
	//errorcheck
	if (file.fail()) {
		PuffEngine::fatalError(fileName + " failed to open!");
	}

	//read, grab the human numbers from first line
	std::string tmp;
	file >> tmp >> numHumans_;

	std::getline(file, tmp); ///< throw away first empty line
	while (std::getline(file, tmp)) {
		levelData_.push_back(tmp);
	}

	//draw level
	spriteBatch_.init();
	spriteBatch_.begin();

	//uvrect all the same
	glm::vec4 uvRect(0.0f, 0.0f, 1.0f, 1.0f);
	//color
	PuffEngine::ColorRGBA8 whiteColor;
	whiteColor.r = 255;
	whiteColor.b = 255;
	whiteColor.g = 255;
	whiteColor.a = 255;

	//loop and render all tiles
	for (int y = 0; y < levelData_.size(); y++) {
		for (int x = 0; x < levelData_[y].size(); x++) {
			//current tile
			char tile = levelData_[y][x];
			//destination rectangle-destrect (x,y, width, height)
			glm::vec4 destrect(x * TILE_WIDTH, y* TILE_WIDTH, TILE_WIDTH, TILE_WIDTH);

			switch (tile) {
			case'R':
			case'B':
				spriteBatch_.draw(destrect,
								  uvRect,
								  PuffEngine::ResourceManager::getTexture("Textures/red_bricks.png").id,
								  0.0f,
								  whiteColor);
				break;
			case'G':
				spriteBatch_.draw(destrect,
								  uvRect,
								  PuffEngine::ResourceManager::getTexture("Textures/glass.png").id,
								  0.0f,
								  whiteColor);
				break;
			case'L':
				spriteBatch_.draw(destrect,
								  uvRect,
								  PuffEngine::ResourceManager::getTexture("Textures/light_bricks.png").id,
								  0.0f,
								  whiteColor);
				break;
			case'@':
				levelData_[y][x] = '.';
				startPlayerPos_.x = x * TILE_WIDTH;
				startPlayerPos_.y = y * TILE_WIDTH;
				break;
			case'Z':
				levelData_[y][x] = '.';
				zombieStartPositions_.emplace_back(x * TILE_WIDTH, y * TILE_WIDTH);
				break;
			case'.':
				break;
			default:
				std::printf("Unexpected symbol %c, at (%d, %d)\n", tile, x, y);
				break;
			}

		}
	}

	spriteBatch_.end();
}


Level::~Level() {
}

void Level::draw() {
	spriteBatch_.renderBatch();
}
