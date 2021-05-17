#pragma once
#include "TextureCahce.h"

namespace PuffEngine {
class ResourceManager {
public:
	static GLTexture getTexture(std::string texturePath);
private:
	static TextureCahce textureCahce_;
};

}