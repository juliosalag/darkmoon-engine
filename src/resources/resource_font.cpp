#include "resource_font.hpp"

namespace DarkMoon {
    bool Font::load(const char* filePath) {
        this->m_filePath = filePath;
        setupFont(m_filePath);

        isLoaded() ? std::cout << "Load a font (ID: " << m_id << ") -> " << m_filePath << std::endl : std::cout << "Error loading a font" << std::endl;

        return isLoaded();
    }

    void Font::unload() {
        characters.clear();
        std::cout << "Unload a font (ID: " << m_id << ")" << std::endl;
    }

    // PRIVATE

    void Font::setupFont(std::string filePath) {

        // Initialize FreeType
        FT_Library library;
        if (FT_Init_FreeType(&library))
            std::cerr << "Error initializing FreeType" << std::endl;

        // Load the font
        FT_Face face;
        if (FT_New_Face(library, filePath.c_str(), 0, &face)) {
            std::cerr << "Error loading the font" << std::endl;
        }
        else {
            // Set size to load glyphs as
            FT_Set_Pixel_Sizes(face, 0, 48);

            // Disable byte-alignment restriction
            glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

            // Load first 128 characters of ASCII set
            for (unsigned char c = 0; c < 128; c++) {
                if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
                    std::cerr << "Error loading Glyph" << std::endl;
                    continue;
                }
                // Generate texture
                unsigned int texture;
                glGenTextures(1, &texture);
                glBindTexture(GL_TEXTURE_2D, texture);
                glTexImage2D(
                    GL_TEXTURE_2D,
                    0,
                    GL_RED,
                    static_cast<GLint>(face->glyph->bitmap.width),
                    static_cast<GLint>(face->glyph->bitmap.rows),
                    0,
                    GL_RED,
                    GL_UNSIGNED_BYTE,
                    face->glyph->bitmap.buffer
                );
                // Set texture options
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                // Now store character for later use
                Character character = {
                    texture,
                    glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
                    glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
                    static_cast<unsigned int>(face->glyph->advance.x)
                };
                characters.insert(std::pair<char, Character>(c, character));
            }

            // Load special characters
            std::wstring special_chars = L"áéíóúñÁÉÍÓÚÑ¿¡";
            for (wchar_t wc : special_chars) {
                if (FT_Load_Char(face, static_cast<FT_ULong>(wc), FT_LOAD_RENDER)) {
                    std::cerr << "Error loading Glyph" << std::endl;
                    continue;
                }
                // Generate texture
                unsigned int texture;
                glGenTextures(1, &texture);
                glBindTexture(GL_TEXTURE_2D, texture);
                glTexImage2D(
                    GL_TEXTURE_2D,
                    0,
                    GL_RED,
                    static_cast<GLsizei>(face->glyph->bitmap.width),
                    static_cast<GLsizei>(face->glyph->bitmap.rows),
                    0,
                    GL_RED,
                    GL_UNSIGNED_BYTE,
                    face->glyph->bitmap.buffer
                );
                // Set texture options
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                // Now store character for later use
                Character character = {
                    texture,
                    glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
                    glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
                    static_cast<unsigned int>(face->glyph->advance.x)
                };
                characters.insert(std::pair<wchar_t, Character>(wc, character));
            }

            glBindTexture(GL_TEXTURE_2D, 0);

            m_isLoad = (library != nullptr) && (face != nullptr);
        }
        // Destroy freetype once we're finished
        FT_Done_Face(face);
        FT_Done_FreeType(library);
    }
}