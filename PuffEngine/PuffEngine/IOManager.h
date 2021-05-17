#pragma once
#include <vector>
#include <string>

namespace PuffEngine {
class IOManager {
public:
	//Static class there wil be 1 istance off this class
	static bool readFileToBuffer(std::string filePath, std::vector<unsigned char>& buffer);
	static bool readFileToBuffer(std::string filePath, std::string& buffer);
	
};
}
