#pragma once

#include <vector>
#include <unordered_map>
#include <string>
#include <glm/glm.hpp>


#include "Drawing/Renderer2D.h"

class Font
{
public:
	Font(Texture* texture,
	std::unordered_map<unsigned int, glm::vec2> positions,
	std::unordered_map<unsigned int, glm::vec2> sizes,
	std::unordered_map<unsigned int, glm::vec2> bearings,
	float lineSpacing,
	float whiteSpace,
	float tabSpacing)
	{
		_texture = texture;
		_positions = positions;
		_sizes = sizes;
		_bearings = bearings;
		_lineSpacing = lineSpacing;
		_whiteSpace = whiteSpace;
		_tabSpacing = tabSpacing;
	}

	void Render(Shader* fontShader, std::string text, glm::vec2 position, glm::vec2 size, glm::vec4 color)
	{
		fontShader->Begin();

		Renderer2D::Begin(fontShader);

		glm::vec2 pos;
		glm::vec2 s;
		glm::vec2 bearing;
		int width = 1;
		int height = 1;
		float x = 0.0f;
		float y = 0.0f;
		if (_texture != nullptr) 
		{
			width = _texture->GetWidth();
			height = _texture->GetHeight();

			for (int i = 0; i < text.size(); i++)
			{
				unsigned int character = text[i];
				switch (character)
				{
				case ' ':
					x += _whiteSpace * size.x/2.0f;
					break;
				case '\t':
					x += _tabSpacing * size.x/2.0f;
					break;
				default:
					if (TryGetGlyph(character, pos, s, bearing))
					{
						glm::vec2 uvPos = glm::vec2(pos.x / width, pos.y / height);
						glm::vec2 uvSize = glm::vec2(s.x / width, s.y / height);
						uvPos.y += uvSize.y;
						uvSize.y *= -1.0f;
						s = s * size;
						/*if (x + bearing.x + s.x > size.x)
						{
							x = 0.0f;
							y += _lineSpacing;
						}*/
						glm::vec2 shownPos = glm::vec2(x + bearing.x, y + bearing.y - (s.y) + _lineSpacing/2.0f) + position;
						glm::vec2 shownSize = s;
						Renderer2D::DrawQuad(shownPos, shownSize, 0.0f, _texture, uvPos, uvSize);
						x += s.x;
					}
					break;
				}
			}
		}

		Renderer2D::End();

		fontShader->End();
	}

	static void Render(Font* font, Shader* fontShader, std::string text, glm::vec2 position, glm::vec2 size, glm::vec4 color)
	{
		if (font != nullptr) font->Render(fontShader, text, position, size, color);
	}
private:
	Texture* _texture;
	std::unordered_map<unsigned int, glm::vec2> _positions;
	std::unordered_map<unsigned int, glm::vec2> _sizes;
	std::unordered_map<unsigned int, glm::vec2> _bearings;
	float _lineSpacing;
	float _whiteSpace;
	float _tabSpacing;

	bool TryGetGlyph(unsigned int character, glm::vec2& position, glm::vec2& size, glm::vec2& bearing)
	{
		if (_positions.count(character))
		{
			position = _positions[character];
			size = _sizes[character];
			bearing = _bearings[character];
			return true;
		}
		return false;
	}
};