#pragma once

#include <string>
#include <vector>
#include <unordered_map>

#include "Font.h"
#include "Drawing/Image.h"
#include "Drawing/Shader.h"

#include <msdfgen.h>
#include <msdf-atlas-gen.h>

class FontManager
{
public:
	static Font* Create(std::string fontFilePath);
};

