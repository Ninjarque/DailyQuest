#include "FontManager.h"

Font* FontManager::Create(std::string fontFilePath)
{
    bool success = false;
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
            fontGeometry.loadCharset(font, 1.0, msdf_atlas::Charset::ASCII);
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
            // The atlas bitmap can now be retrieved via atlasStorage as a BitmapConstRef.
            // The glyphs array (or fontGeometry) contains positioning data for typesetting text.

            //std::vector<Color> imageData(width * height);
            auto& storage = generator.atlasStorage();
            // Get the pointer to the underlying pixel data
            auto pixels = storage.data();
            //float* pixels = //((msdfgen::Bitmap<float, 3>)generator.atlasStorage()).operator msdfgen::BitmapRef<float, 3>().pixels;
            Texture* texture = nullptr;
            if (pixels != nullptr)
            {
                //for (int i = 0; i < imageData.size(); i++)
                //    imageData[i] = Color(pixels[i * 4] / 1.0f, pixels[i * 4 + 1] / 1.0f, pixels[i * 4 + 2] / 1.0f, pixels[i * 4 + 3] / 1.0f);
                texture = Image::Create(width, height, pixels);
                success = true;//myProject::submitAtlasBitmapAndLayout(generator.atlasStorage(), glyphs);
                double whiteSpace, tabSpacing;
                msdfgen::getFontWhitespaceWidth(whiteSpace, tabSpacing, font);
                msdfgen::FontMetrics metrics;
                msdfgen::getFontMetrics(metrics, font);
                double lineSpacing = metrics.lineHeight;

                double metricRatio = 64 / metrics.emSize;
                whiteSpace *= metricRatio;
                tabSpacing *= metricRatio;
                lineSpacing *= metricRatio;

                std::unordered_map<unsigned int, glm::vec2> positions;
                std::unordered_map<unsigned int, glm::vec2> sizes;
                std::unordered_map<unsigned int, glm::vec2> bearings;
                std::unordered_map<unsigned int, float> advances;
                for (auto glyph : glyphs)
                {
                    if (glyph.isWhitespace())
                        continue;
                    unsigned int character = glyph.getCodepoint();
                    if (!character)
                        continue;
                    double x, y, w, h;
                    glyph.getQuadAtlasBounds(x, y, w, h);
                    positions[character] = glm::vec2(x, y);
                    sizes[character] = glm::vec2(w - x, h - y);
                    double l, b, r, t;
                    glyph.getQuadPlaneBounds(l, b, r, t);

                    msdfgen::Shape shape;
                    msdfgen::Shape::Bounds bounds;
                    double advance;
                    msdfgen::loadGlyph(shape, font, character, &advance);
                    if (shape.validate() && shape.contours.size() > 0)
                    {
                        shape.normalize();
                        //shape.inverseYAxis = true;

                        bounds = shape.getBounds(4);
                    }
                    bearings[character] = glm::vec2((float)bounds.l/64.0f,(float)bounds.t/64.0f);
                    //std::cout << bearings[character].y << std::endl;

                    advances[character] = (float)glyph.getAdvance();
                }

                return new Font(texture, positions, sizes, bearings, advances, (float)lineSpacing, (float)whiteSpace, (float)tabSpacing, (float)metricRatio);
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

	return nullptr;
}