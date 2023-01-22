#include "FontManager.h"

std::unordered_map<Font*, std::string> FontManager::_fontFilePath;
std::unordered_map<Font*, std::vector<Texture*>> FontManager::_fontTextures;
std::unordered_map<Font*, std::vector<msdf_atlas::Charset>> FontManager::_fontCharsets;


Font* FontManager::Create(std::string fontFilePath)
{
    return Create(fontFilePath, msdf_atlas::Charset::ASCII);
}

Font* FontManager::Create(std::string fontFilePath, std::vector<unsigned int> charsetParts)
{
    Font* font = Create(fontFilePath);
    if (font != nullptr)
    {
        for (auto c : charsetParts)
        {
            TryExpand(font, c);
        }
        return font;
    }
    return nullptr;
}

Font* FontManager::Create(std::string fontFilePath, msdf_atlas::Charset baseCharset)
{
    std::unordered_map<unsigned int, CharData> charDatas;
    float baseY, lineSpacing, whiteSpace, tabSpacing, metricRatio;
    Texture* texture;
    if (ExpandFontCharset(fontFilePath, baseCharset, charDatas,
        baseY, lineSpacing, whiteSpace, tabSpacing, metricRatio, texture))
    {
        Font* font = new Font(charDatas, baseY, lineSpacing, whiteSpace, tabSpacing, metricRatio);
        _fontFilePath[font] = fontFilePath;
        _fontCharsets[font].push_back(baseCharset);
        _fontTextures[font].push_back(texture);
        return font;
    }
    return nullptr;
}

void FontManager::Delete(Font* font)
{
    if (font == nullptr)
        return;
    std::vector<Texture*> textures = _fontTextures[font];
    for (auto texture : textures)
        texture->Dispose();
    _fontFilePath.erase(font);
    _fontTextures.erase(font);
    _fontCharsets.erase(font);

    font->Dispose();
}

bool FontManager::TryExpand(Font* font, unsigned int targetChar)
{
    if (font == nullptr) return false;
    unsigned int minChar, maxChar;
    if (!TryGetCharsetBounds(font, targetChar, minChar, maxChar))
    {
        font->Expand(std::unordered_map<unsigned int, CharData>{ { targetChar, CharData() } });
        return false;
    }
    unsigned int distMin = targetChar - minChar;
    unsigned int distMax = maxChar - targetChar;
    msdf_atlas::Charset charSet;
    if (distMin > distMax)
    {
        charSet = msdf_atlas::Charset::MakeForTarget(distMin + 1);
    }
    else
    {
        charSet = msdf_atlas::Charset::MakeForTarget(targetChar);
    }
    _fontCharsets[font].push_back(charSet);
    
    std::unordered_map<unsigned int, CharData> charDatas;
    float a, b, c, d, e;
    Texture* texture;
    if (ExpandFontCharset(_fontFilePath[font], charSet, charDatas,
        a, b, c, d, e, texture))
    {
        _fontTextures[font].push_back(texture);
        font->Expand(charDatas);
        return true;
    }

    return false;
}

bool FontManager::TryGetCharsetBounds(Font* font, unsigned int character, unsigned int& minChar, unsigned int& maxChar)
{
    std::vector<msdf_atlas::Charset> charsets = _fontCharsets[font];
    minChar = -1;
    maxChar = 0;
    for (auto charset : charsets)
    {
        auto begin = charset.first();
        auto end = charset.last();
        if (begin < character && end > character) return false;
        if (end < character && character - end > maxChar)
        {
            maxChar = end;
        }
        if (character < begin && begin - character < minChar)
        {
            minChar = begin;
        }
    }
    return true;
}

bool FontManager::ExpandFontCharset(std::string fontFilePath, 
    msdf_atlas::Charset newChars, std::unordered_map<unsigned int, CharData>& newCharDatas,
    float& baseY, float& lineSpacing, float& whiteSpace, float& tabSpacing, float& metricRatio,
    Texture*& texture)
{
    // Initialize instance of FreeType library
    if (msdfgen::FreetypeHandle* ft = msdfgen::initializeFreetype()) {
        // Load font file
        if (msdfgen::FontHandle* font = msdfgen::loadFont(ft, fontFilePath.c_str())) {
            // Storage for glyph geometry and their coordinates in the atlas
            std::vector<msdf_atlas::GlyphGeometry> glyphs;
            // FontGeometry is a helper class that loads a set of glyphs from a single font.
            // It can also be used to get additional font metrics, kerning information, etc.
            msdf_atlas::FontGeometry fontGeometry(&glyphs);
            // Load a set of character glyphs:
            // The second argument can be ignored unless you mix different font sizes in one atlas.
            // In the last argument, you can specify a charset other than ASCII.
            // To load specific glyph indices, use loadGlyphs instead.
            fontGeometry.loadCharset(font, 1.0, newChars);
            // Apply MSDF edge coloring. See edge-coloring.h for other coloring strategies.
            const double maxCornerAngle = 3.0;
            for (msdf_atlas::GlyphGeometry& glyph : glyphs)
                glyph.edgeColoring(&msdfgen::edgeColoringInkTrap, maxCornerAngle, 0);
            // TightAtlasPacker class computes the layout of the atlas.
            msdf_atlas::TightAtlasPacker packer;
            // Set atlas parameters:
            // setDimensions or setDimensionsConstraint to find the best value
            packer.setDimensionsConstraint(msdf_atlas::TightAtlasPacker::DimensionsConstraint::SQUARE);
            // setScale for a fixed size or setMinimumScale to use the largest that fits
            packer.setMinimumScale(32.0);
            // setPixelRange or setUnitRange
            packer.setPixelRange(2.0);
            packer.setMiterLimit(1.0);
            // Compute atlas layout - pack glyphs
            packer.pack(glyphs.data(), glyphs.size());
            // Get final atlas dimensions
            int width = 0, height = 0;
            packer.getDimensions(width, height);
            // The ImmediateAtlasGenerator class facilitates the generation of the atlas bitmap.
            msdf_atlas::ImmediateAtlasGenerator<
                float, // pixel type of buffer for individual glyphs depends on generator function
                4, // number of atlas color channels
                msdf_atlas::mtsdfGenerator, // function to generate bitmaps for individual glyphs
                msdf_atlas::BitmapAtlasStorage<float, 4> // class that stores the atlas bitmap
                // For example, a custom atlas storage class that stores it in VRAM can be used.
            > generator(width, height);
            // GeneratorAttributes can be modified to change the generator's default settings.
            msdf_atlas::GeneratorAttributes attributes;
            generator.setAttributes(attributes);
            generator.setThreadCount(4);
            // Generate atlas bitmap
            generator.generate(glyphs.data(), glyphs.size());
            
            auto& storage = generator.atlasStorage();
            
            // Get the pointer to the underlying pixel data
            auto pixels = storage.data();
            Texture* texture = nullptr;
            if (pixels != nullptr)
            {
                texture = Image::Create(width, height, pixels);

                double wSpace, tSpacing;
                msdfgen::getFontWhitespaceWidth(wSpace, tSpacing, font);
                whiteSpace = (float)wSpace;
                tabSpacing = (float)tSpacing;
                msdfgen::FontMetrics metrics;
                msdfgen::getFontMetrics(metrics, font);
                lineSpacing = metrics.lineHeight;

                metricRatio = 64 / metrics.emSize;
                whiteSpace *= metricRatio;
                tabSpacing *= metricRatio;
                lineSpacing *= metricRatio;

                for (auto glyph : glyphs)
                {
                    if (glyph.isWhitespace())
                        continue;
                    unsigned int character = glyph.getCodepoint();
                    if (!character)
                        continue;

                    msdfgen::Shape shape;
                    msdfgen::Shape::Bounds bounds;
                    double advance;
                    msdfgen::loadGlyph(shape, font, character, &advance);
                    if (shape.validate() && shape.contours.size() > 0)
                    {
                        //shape.normalize();
                        //shape.inverseYAxis = true;

                        bounds = shape.getBounds(0.0f);
                    }
                    double l, b, r, t;
                    glyph.getQuadPlaneBounds(l, b, r, t);
                    double x, y, w, h;
                    glyph.getQuadAtlasBounds(x, y, w, h);
                    l *= metrics.emSize;
                    t *= metrics.emSize;
                    r *= metrics.emSize;
                    b *= metrics.emSize;
                    CharData charData = CharData(
                        glm::vec4((float)l, (float)t, (float)r, (float)b),
                        glm::vec4(x, y, w, h),
                        (float)(glyph.getAdvance() * metrics.emSize),
                        texture
                    );
                    newCharDatas[character] = charData;
                }

                baseY = metrics.ascenderY;

                //_fontFilePath[font] = fontFilePath;
                //_fontTextures[font].push_back(texture);
                //_fontCharsets[font].push_back(newChars);
                return true;
            }
            else
            {
                Error::warning("Couldn't read font atlas's pixels, how?!");
            }


            // Cleanup
            msdfgen::destroyFont(font);
        }
        else
        {
            Error::warning("Could not load font file '" + fontFilePath + "'!");
        }
        msdfgen::deinitializeFreetype(ft);
    }
    return false;
}
