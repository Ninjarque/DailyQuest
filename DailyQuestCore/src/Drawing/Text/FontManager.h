#pragma once

#include <string>
#include <vector>
#include <unordered_map>

#include "Font.h"
#include "Drawing/Image.h"
#include "Drawing/Shader.h"

#include <msdfgen.h>
#include <msdf-atlas-gen.h>

struct CharData;
class Font;

enum FontDetails
{
	Small,
	Default,
	Better,
	ReallyAccurate,
	Overkill
};

class FontManager
{
public:
	static Font* Create(std::string fontFilePath, 
		FontDetails details = FontDetails::Default);
	static Font* Create(std::string fontFilePath, msdf_atlas::Charset baseCharset, 
		FontDetails details = FontDetails::Default);
	static Font* Create(std::string fontFilePath, std::vector<unsigned int> charsetParts, 
		FontDetails details = FontDetails::Default);
	static void Delete(Font* font);

	static bool TryExpand(Font* font, unsigned int targetChar);

private:
	static bool TryGetCharsetBounds(Font* font, unsigned int character,
		unsigned int& minChar, unsigned int& maxChar);
	static bool ExpandFontCharset(std::string fontFilePath,
		msdf_atlas::Charset newChars, std::unordered_map<unsigned int, CharData>& newCharDatas,
		float& baseY, float& lineSpacing, float& whiteSpace, float& tabSpacing, float& metricRatio,
		Texture*& texture, FontDetails details = FontDetails::Default);

	static std::unordered_map<Font*, std::string> _fontFilePath;
	static std::unordered_map<Font*, std::vector<Texture*>> _fontTextures;
	static std::unordered_map<Font*, std::vector<msdf_atlas::Charset>> _fontCharsets;
	static std::unordered_map<Font*, FontDetails> _fontDetails;
};

