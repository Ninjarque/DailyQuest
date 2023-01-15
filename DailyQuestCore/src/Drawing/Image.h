#pragma once

#include <string>
#include <vector>

#include "Texture.h"
#include "Color.h"

class Image
{
public:
	static Texture* Load(std::string filePath);
	static Texture* Create(int width, int height, std::vector<Color> colors);
};