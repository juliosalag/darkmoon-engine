#pragma once

#include <freetype2/ft2build.h>
#include FT_FREETYPE_H

#include <map>
#include <GL/glew.h>
#include <GL/gl.h>
#include <glm/glm.hpp>

#include "resource.hpp"

namespace DarkMoon {

    struct Character {
        unsigned int textureID;
        glm::ivec2 size;
        glm::ivec2 bearing;
        unsigned int advance;
    };

    struct Font : public Resource {
    public:
        std::map<wchar_t, Character> characters;

        Font(std::size_t id) { this->m_id = id; };
        ~Font() { unload(); };

        bool load(const char*) override;
        void unload() override;

    private:
        void setupFont(std::string filePath);
    };
}