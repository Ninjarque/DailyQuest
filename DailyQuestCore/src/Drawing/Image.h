#pragma once

#include <string>
#include <vector>

#include "Components/Drawing.h"
#include "Components/Texture.h"

using namespace Components;

class Image
{
public:
	static Texture* Load(std::string filePath);
	static Texture* Create(int width, int height, std::vector<Color> colors);
	static Texture* Create(int width, int height, float* colors);
};