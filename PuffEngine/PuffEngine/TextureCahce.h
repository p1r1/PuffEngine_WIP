#pragma once
#include <map>
#include <string>
#include "GLTexture.h"

namespace PuffEngine {
class TextureCahce {
public:
	TextureCahce();
	~TextureCahce();

	GLTexture getTexture(std::string texturePath);

private:
	std::map<std::string, GLTexture> textureMap_;

};

}