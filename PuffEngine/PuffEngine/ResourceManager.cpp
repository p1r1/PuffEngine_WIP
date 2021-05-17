#include "ResourceManager.h"
#include <string>

namespace PuffEngine {
//because this is a static class, static variable declare again in cpp file
TextureCahce ResourceManager::textureCahce_;
//wrapper
GLTexture ResourceManager::getTexture(std::string texturePath) {
	return textureCahce_.getTexture(texturePath);
}
}
