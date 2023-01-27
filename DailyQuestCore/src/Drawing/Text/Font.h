#pragma once

#include <vector>
#include <unordered_map>
#include <string>
#include <codecvt>
#include <glm/glm.hpp>

#include "Drawing/Renderer2D.h"
#include "FontManager.h"

#include "Components/Camera.h"

#include "utf8/utf8.h"

struct CharData
{
	glm::vec4 rect;
	glm::vec4 uvRect;
	float advance;
	Texture* texture;

	CharData() { rect = glm::vec4(); uvRect = glm::vec4(); advance = 0.0f; texture = nullptr; }
	CharData(glm::vec4 rect, glm::vec4 uvRect, float advance, Texture* texture)
	{
		this->rect = rect;
		this->uvRect = uvRect;
		this->advance = advance;
		this->texture = texture;
	}
};

class FontManager;

class Font
{
public:
	Font(std::unordered_map<unsigned int, CharData> charDatas,
		float baseY,
		float lineSpacing,
		float whiteSpace,
		float tabSpacing,
		float metricRatio);
	~Font();
	void Dispose();

	void Render(Camera* camera, Shader* fontShader, std::string text,
		glm::vec2 position, glm::vec2 bounds, float textSize, glm::vec4 color);

	void Render(Camera* camera, Shader* fontShader, std::string text,
		glm::vec2 position, glm::vec2 bounds, float textSize, glm::vec4 color,
		float borders, glm::vec2 shadowOffset, glm::vec4 secondColor);

	void Render(Camera* camera, Shader* fontShader, std::u32string text,
		glm::vec2 position, glm::vec2 bounds, float textSize, glm::vec4 color);

	void Render(Camera* camera, Shader* fontShader, std::u32string text,
		glm::vec2 position, glm::vec2 bounds, float textSize, glm::vec4 color,
		float borders, glm::vec2 shadowOffset, glm::vec4 secondColor);

	/*
	Maybe make static Render use std::string and create Font if not already made
	could make it easier to use
	*/
	static void Render(Font* font, Camera* camera, Shader* fontShader, std::string text,
		glm::vec2 position, glm::vec2 bounds, float textSize, glm::vec4 color);

	static void Render(Font* font, Camera* camera, Shader* fontShader, std::u32string text,
		glm::vec2 position, glm::vec2 bounds, float textSize, glm::vec4 color);

	friend class FontManager;
private:
	std::unordered_map<unsigned int, CharData> _charDatas;
	float _baseY;
	float _lineSpacing;
	float _whiteSpace;
	float _tabSpacing;
	float _metricRatio;
	bool _disposed;

	inline float lerp(float x, float y, float t) {
		return x * (1.f - t) + y * t;
	}

	std::u32string to_utf32(std::string s)
	{
		auto end_it = utf8::find_invalid(s.begin(), s.end());
		if (end_it != s.end()) 
		{ 
			Error::warning("Detected invalid utf8 encoding in '" + s + "'!"); 
			return U"";
		}
		std::vector<char32_t> utf32line;
		utf8::utf8to32(s.begin(), end_it, back_inserter(utf32line));
		std::u32string res(utf32line.begin(), utf32line.end());
		return res;
	}

	bool TryGetGlyph(unsigned int character, glm::vec4& rect, glm::vec4& uvRect, float& advance, Texture*& texture);

	void Expand(std::unordered_map<unsigned int, CharData> charDatas);
};