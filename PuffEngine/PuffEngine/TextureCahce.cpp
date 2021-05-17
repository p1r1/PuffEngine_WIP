#include "TextureCahce.h"
#include "imageLoader.h"
#include <iostream>

namespace PuffEngine {
TextureCahce::TextureCahce() {
}


TextureCahce::~TextureCahce() {
}

GLTexture TextureCahce::getTexture(std::string texturePath) {

	//std::map<std::string, GLTexture>::iterator mit = textureMap_.find(texturePath);
	auto mit = textureMap_.find(texturePath);
	//check if its not in the map
	if (mit == textureMap_.end()) {
		//load texture
		GLTexture newTexture = imageLoader::loadPNG(texturePath);

		//insert it into the map
		//makepair -> std::pair<std::string, GLTexture> newPair(texturePath, newTexture);
		textureMap_.insert(make_pair(texturePath, newTexture));

		//std::cout << "Loaded texture!\n";
		return newTexture;
	}
	//std::cout << "Used cahced texture!\n";
	return mit->second;//texture

}

}