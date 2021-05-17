#include "IOManager.h"

#include <fstream>

namespace PuffEngine {

bool IOManager::readFileToBuffer(std::string filePath, std::vector<unsigned char>& buffer) {
	//read all data in binary formath flag vvv
	std::ifstream file(filePath, std::ios::binary);
	if (file.fail()) {
		perror(filePath.c_str());
		return false;
	}
	//how big file is seekg and teelg
	//seek to the end
	//imleci yazinin sonuna getirip birakti 
	file.seekg(0, std::ios::end);
	//get to the file size
	int fileSize = file.tellg();
	//file okumak icin imleci baslangica geri gotur
	file.seekg(0, std::ios::beg);

	//get rid of the file header
	fileSize -= file.tellg();

	buffer.resize(fileSize);
	// needs char* first elements address of vector  of chars
	// because thee are  binary we can cast it to cahr*
	file.read((char*)&(buffer[0]), fileSize);
	file.close();
	return true;
}

bool IOManager::readFileToBuffer(std::string filePath, std::string & buffer) {
	//read all data in binary formath flag vvv
	std::ifstream file(filePath, std::ios::binary);
	if (file.fail()) {
		perror(filePath.c_str());
		return false;
	}
	//how big file is seekg and teelg
	//seek to the end
	//imleci yazinin sonuna getirip birakti 
	file.seekg(0, std::ios::end);
	//get to the file size
	int fileSize = file.tellg();
	//file okumak icin imleci baslangica geri gotur
	file.seekg(0, std::ios::beg);

	//get rid of the file header
	fileSize -= file.tellg();

	buffer.resize(fileSize);
	// needs char* first elements address of vector  of chars
	// because thee are  binary we can cast it to cahr*
	file.read((char*)&(buffer[0]), fileSize);
	file.close();
	return true;
}


}