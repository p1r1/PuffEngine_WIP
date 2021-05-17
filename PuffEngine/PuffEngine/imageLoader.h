#pragma once
#include "GLTexture.h"
#include <string>

namespace PuffEngine {
class imageLoader {
public:
	//static class like iomanager
	static GLTexture loadPNG(std::string filePath);
};
}
