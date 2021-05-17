#pragma once
#include <vector>

namespace PuffEngine {
//if you create sperate header and cpp file the you need to use extern

//out_image-> output parameter, this will contain the raw pixels after decoding. rgba
//in_png -> pointer to the buffer of the PNG file in memory. To get it from a file on
extern int decodePNG(std::vector<unsigned char>& out_image, unsigned long& image_width, unsigned long& image_height, const unsigned char* in_png, size_t in_size, bool convert_to_rgba32 = true);
}