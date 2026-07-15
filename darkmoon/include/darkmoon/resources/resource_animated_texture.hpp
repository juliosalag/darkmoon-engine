#pragma once

#include <glad/glad.h>
#include <vector>

#include "resource.hpp"

struct ResourceAnimatedTexture : public Resource {
public:
    ResourceAnimatedTexture(std::size_t idResource, std::size_t fileType, const char* filePath);
    ~ResourceAnimatedTexture() { unload(); }

    void unload() override;

    int getWidth()      const { return m_width;  }
    int getHeight()     const { return m_height; }
    int getFrameCount() const { return static_cast<int>(m_frames.size()); }

    // Returns the GL texture ID for a given frame index.
    GLuint getFrameTexture(int index) const { return m_frames[static_cast<size_t>(index)].textureID; }

    // Frame delay in seconds (respects GIF centisecond delays).
    float getFrameDelay(int index) const { return m_frames[static_cast<size_t>(index)].delay; }

private:
    struct Frame {
        GLuint textureID { 0 };
        float  delay     { 0.1f };
    };

    std::vector<Frame> m_frames {};
    int m_width  { 0 };
    int m_height { 0 };

    void setup() override {}
};