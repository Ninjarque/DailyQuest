#include "Font.h"

Font::Font(std::unordered_map<unsigned int, CharData> charDatas, 
	float baseY, float lineSpacing, float whiteSpace, float tabSpacing, float metricRatio)
{
	_charDatas = charDatas;

	_baseY = baseY;
	_lineSpacing = lineSpacing;
	_whiteSpace = whiteSpace;
	_tabSpacing = tabSpacing;
	_metricRatio = metricRatio;

	_disposed = false;
}

Font::~Font()
{
	Dispose();
}

void Font::Dispose()
{
	if (_disposed) return;
	_charDatas.clear();
	FontManager::Delete(this);
	_disposed = true;
}

void Font::Render(Camera* camera, Shader* fontShader, Viewport* viewport, std::string text,
	glm::vec2 position, glm::vec2 bounds, float textSize, glm::vec4 color)
{
	std::u32string str = to_utf32(text);
	Render(camera, fontShader, viewport, str, position, bounds, textSize, color);
}

void Font::Render(Camera* camera, Shader* fontShader, Viewport* viewport, std::string text, glm::vec2 position, glm::vec2 bounds, float textSize, glm::vec4 color, float borders, glm::vec2 shadowOffset, glm::vec4 secondColor)
{
	std::u32string str = to_utf32(text);
	Render(camera, fontShader, viewport, str, position, bounds, textSize, color,
		0.0f, glm::vec2(0.0f), glm::vec4(0.0f));
}

void Font::Render(Camera* camera, Shader* fontShader, Viewport* viewport, std::u32string text,
	glm::vec2 position, glm::vec2 bounds, float textSize, glm::vec4 color)
{
	Render(camera, fontShader, viewport, text, position, bounds, textSize, color,
		0.0f, glm::vec2(0.0f), glm::vec4(0.0f));
}

void Font::Render(Camera* camera, Shader* fontShader, Viewport* viewport, std::u32string text,
	glm::vec2 position, glm::vec2 bounds, float textSize, glm::vec4 color,
	float borders, glm::vec2 shadowOffset, glm::vec4 secondColor)
{
	fontShader->Begin();

	Renderer2D::Begin(camera, fontShader, viewport);
	
	float metricsModifier = lerp(1.5f, 1.0f, _metricRatio / 4.0f);
	float char_width = lerp(0.45f, 0.5f, textSize / 200.0f / metricsModifier);
	float char_edge = lerp(0.4f, 0.1f, textSize / 200.0f / metricsModifier);
	fontShader->Set("char_width", char_width);
	fontShader->Set("char_edge", char_edge);

	fontShader->Set("border_width", char_width + borders);
	fontShader->Set("border_edge", char_edge);
	fontShader->Set("second_color", secondColor);

	fontShader->Set("shadow_offset", shadowOffset / _metricRatio);

	fontShader->Set("metrics_ratio", _metricRatio);

	glm::vec4 rect;
	glm::vec4 uvRect;
	float x = 0.0f;
	float y = textSize + _baseY;//_lineSpacing; //* textSize / _metricRatio;//_baseY;
	float advance = 0.0f;
	Texture* texture;

	for (int i = 0; i < text.size(); i++)
	{
		unsigned int character = text[i];

		switch (character)
		{
		case ' ':
			x += _whiteSpace;//* bounds.x / 2.0f;
			break;
		case '\t':
			x += _tabSpacing;//* bounds.x / 2.0f;
			break;
		default:
			bool foundChar = TryGetGlyph(character, rect, uvRect, advance, texture);
			if (!foundChar) foundChar = FontManager::TryExpand(this, character) 
				&& TryGetGlyph(character, rect, uvRect, advance, texture);
			if (foundChar && texture)
			{
				int width = texture->GetWidth();
				int height = texture->GetHeight();

				rect *= textSize / 1.0f;
				advance *= textSize;
				glm::vec2 uvPos = glm::vec2(uvRect.x / width, uvRect.y / height);
				glm::vec2 uvSize = glm::vec2((uvRect.z - uvRect.x) / width, (uvRect.w - uvRect.y) / height);
				uvPos.y += uvSize.y;
				uvSize.y *= -1.0f;
				// /*
				if (x + advance > bounds.x)
				{
					x = 0.0f;
					y += textSize + _baseY;//_lineSpacing * textSize / _metricRatio;
				}
				if (y - (rect.w) > bounds.y)
				{
					i = text.size();
					break;
				}
				// */
				glm::vec2 shownPos = glm::vec2(x + rect.x, y - rect.y);//glm::vec2(x + bearing.x, y + bearing.y + _lineSpacing * textSize / _metricRatio / 2.0f) + position;
				glm::vec2 shownSize = glm::vec2(rect.z - rect.x, -rect.w + rect.y);//size;
				//glm::vec2 shadowPosOffset = glm::vec2(glm::max(0.0f, shadowOffset.x / (float)width), glm::min(0.0f, shadowOffset.y / (float)height));
				//glm::vec2 shadowSizeOffset = glm::vec2(glm::abs(shadowOffset.x / (float)width), -glm::abs(shadowOffset.y / (float)height));
				Renderer2D::DrawQuad(shownPos + position, shownSize, 0.0f, texture, uvPos, uvSize, glm::vec2(0.0f), 0.0f);
				x += advance;//size.x;
			}
			break;
		}
	}
	Renderer2D::End();

	fontShader->End();
}

void Font::Render(Font* font, Camera* camera, Shader* fontShader, Viewport* viewport, std::string text,
	glm::vec2 position, glm::vec2 bounds, float textSize, glm::vec4 color)
{
	if (font != nullptr) font->Render(camera, fontShader, viewport, text, position, bounds, textSize, color);
}

void Font::Render(Font* font, Camera* camera, Shader* fontShader, Viewport* viewport, std::u32string text, glm::vec2 position, glm::vec2 bounds, float textSize, glm::vec4 color)
{
	if (font != nullptr) font->Render(camera, fontShader, viewport, text, position, bounds, textSize, color);
}

bool Font::TryGetGlyph(unsigned int character, 
	glm::vec4& rect, glm::vec4& uvRect, float& advance, Texture*& texture)
{
	if (_charDatas.count(character))
	{
		CharData charData = _charDatas[character];
		rect = charData.rect;
		uvRect = charData.uvRect;
		advance = charData.advance;
		texture = charData.texture;
		return true;
	}
	return false;
}

void Font::Expand(std::unordered_map<unsigned int, CharData> charDatas)
{
	for (auto charData : charDatas)
	{
		_charDatas[charData.first] = charData.second;
	}
}
